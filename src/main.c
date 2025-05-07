#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "tree.h"
#include "bitset.h"
#include "bitio.h"

enum {BUFFER_SIZE = 4096};
enum {ALPHABET_SIZE = 256};

size_t get_lbo(const unsigned long long *freq_table, Bitset *code_table) {
    /**
     * @brief Вычисляет кол-во значащих битов в последнем байте закодированного файла.
     * 
     * Для каждого символа вычисляется частота и длина его кода, 
     * их произведение мод 8, затем суммируется.
     * 
     * @param freq_table Массив частот для каждого символа.
     * @param code_table Таблица битовых представлений для каждого символа.
     * @return Кол-во значащих битов в последнем байте закодированного файла.
     */
    size_t result = 0;
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        result += ((freq_table[i] % 8) * (code_table[i].size % 8)) % 8;
    }
    return (result % 8);
}

void encode_node(Writer *writer, Node *node) {
    /**
     * @brief Рекурсивно кодирует дерево Хаффмана в битовый поток.
     * 
     * Прямой обход дерева. Лист — бит 1 и значение, 
     * внутренний узел — бит 0 и рекурсивный вызов для потомков.
     * 
     * @param writer Структура для записи битов в выходной поток.
     * @param node Корень дерева, которое нужно закодировать.
     */
    if (is_leaf(node)) {
        write_bit(writer, 1);
        write_byte(writer, node->value);
    }
    else {
        write_bit(writer, 0);
        encode_node(writer, node->left);
        encode_node(writer, node->right);
    }
}

Node* read_node(Reader *reader) {
    /**
     * @brief Рекурсивно восстанавливает дерево Хаффмана из битового потока.
     * 
     * Читает бит, если 1 — создаёт лист, 
     * если 0 — рекурсивно читает левое и правое поддеревья.
     * 
     * @param reader Структура для чтения битов из входного потока.
     * @return Указатель на считанный узел дерева.
     */
    if (read_bit(reader) == 1)
        return new_node(read_byte(reader), NULL, NULL);
    else {
        Node* left = read_node(reader);
        Node* right = read_node(reader);
        return new_node(0, left, right);
    }
}

void create_freq_table(FILE* input, unsigned long long *freq_table) {
    /**
     * @brief Создает таблицу частот символов из входного файла.
     * 
     * Считывает буферами файл и увеличивает счётчик каждого байта.
     * 
     * @param input Входной файл, из которого извлекаются символы.
     * @param freq_table Массив для хранения частот каждого символа.
     */
    unsigned char buffer[BUFFER_SIZE] = "";
    size_t read = fread(buffer, sizeof(char), BUFFER_SIZE, input);
    while(read != 0) {
        for (size_t i = 0; i < read; i++)
            freq_table[buffer[i]]++;
        read = fread(buffer, sizeof(char), BUFFER_SIZE, input);
    }
}

Node* generate_tree(unsigned long long *freq_table) {
    /**
     * @brief Генерирует дерево Хаффмана на основе таблицы частот.
     * 
     * Постепенно объединяет наименее частотные узлы в дерево, пока не останется один.
     * 
     * @param freq_table Массив частот символов.
     * @return Указатель на корень дерева Хаффмана.
     */
    Queue queue;
    init_queue(&queue);
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        if (freq_table[i] != 0) {
            Node* node = (Node*)malloc(sizeof(Node));
            node->value = i;
            node->left = NULL;
            node->right = NULL;
            enqueue(&queue, node, freq_table[i]);
        }
    }

    if (!is_empty(queue)) {
        while (queue.length > 1) {
            unsigned long long priority = 0;

            priority += get_priority(queue);
            Node* left_node = get_node(queue);
            dequeue(&queue);

            priority += get_priority(queue);
            Node* right_node = get_node(queue);
            dequeue(&queue);

            Node* node = new_node(0, left_node, right_node);
            if (node)
                enqueue(&queue, node, priority);
            else {
                fputs("Stack Overflow", stderr);
                delete_queue(&queue);
                free(left_node);
                free(right_node);
                return NULL;
            }
        }
        Node* root = get_node(queue);
        dequeue(&queue);
        return root;
    }
    else return NULL;
}

void generate_bitsets(Node* node, Bitset bs, Bitset* code_table) {
    /**
     * @brief Генерирует таблицу битовых кодов Хаффмана для каждого символа.
     * 
     * рекурсивный обход дерева. Левое поддерево — бит 0, правое — бит 1. 
     * Коды копируются в таблицу.
     * 
     * @param node Узел дерева Хаффмана.
     * @param bs Промежуточный  битсет.
     * @param code_table Выходная таблица кодов.
     */
    if (node) {
        if (is_leaf(node)) {
            if (bs.size == 0)
                append_bit(&bs, 1);
            copy_bitset(&code_table[node->value], bs);
        }
        else {
            append_bit(&bs, 1);
            generate_bitsets(node->right, bs, code_table);
            rewrite_at(&bs, bs.size - 1, 0);
            generate_bitsets(node->left, bs, code_table);
        }
    }
}

void compress(FILE *input, Writer *writer, Bitset* code_table) {
    /**
     * @brief Сжимает данные, используя коды Хаффмана.
     * 
     * По каждому байту входного файла выбирается соответствующий 
     * битовый код и записывается в поток.
     * 
     * @param input Входной файл для сжатия.
     * @param writer Писатель битов.
     * @param code_table Таблица кодов.
     */
    unsigned char buffer[BUFFER_SIZE] = "";

    size_t read = fread(buffer, sizeof(char), BUFFER_SIZE, input);
    while (read != 0) {
        for (size_t i = 0; i < read; i++)
            print_bitset(code_table[buffer[i]], writer);

        read = fread(buffer, sizeof(char), BUFFER_SIZE, input);
    }
}

void decompress(Reader *reader, FILE* output, Node *h_tree, size_t lbo) {
    /**
     * @brief Распаковывает данные, используя дерево Хаффмана.
     * 
     * @param reader Структура для чтения битов.
     * @param output Файл для записи декодированных данных.
     * @param h_tree Корень дерева Хаффмана.
     * @param lbo Смещение битов для окончания чтения.
     */
    Node *node = h_tree;
    while (!feof(reader->input) || reader->bits_filled != (8 - lbo)) {
        unsigned char bit = read_bit(reader);
        if (!is_leaf(h_tree)) {
            if (bit == 1)
                node = node->right;
            else
                node = node->left;
        }
        if (is_leaf(node)) {
            fputc(node->value, output);
            node = h_tree;
        }
    }
}

void archiver(FILE* input, FILE* output, char mode) {
    /**
     * @brief Универсальная функция: сжатие или распаковка в зависимости от режима.
     * 
     * - В режиме 'c': строит таблицу частот, дерево, кодирует его, пишет LBO и сжимает файл.
     * - В режиме 'd': восстанавливает дерево, считывает LBO и распаковывает данные.
     * 
     * @param input Входной файл для обработки.
     * @param output Выходной файл для записи результата.
     * @param mode Режим работы: 'c' для сжатия и 'd' для восстановления.
     */
    if (mode == 'c') {
        long pos = ftell(input);
        unsigned long long freq_table[ALPHABET_SIZE] = { 0 };
        create_freq_table(input, freq_table);
        fseek(input, pos, SEEK_SET);

        Node* root = generate_tree(freq_table);
        if (root) {
            Bitset code_table[ALPHABET_SIZE];
            for (size_t i = 0; i < ALPHABET_SIZE; i++)
                init_bitset(&code_table[i]);

            Bitset bs;
            init_bitset(&bs);
            generate_bitsets(root, bs, code_table);

            Writer writer;
            init_writer(&writer, output);

            encode_node(&writer, root);
            size_t lbo = (writer.bits_filled + 3 + get_lbo(freq_table, code_table)) % 8;
            write_number(&writer, lbo, 3);
            compress(input, &writer, code_table);
            write_last(&writer);

            delete_tree(root);
        }
    }

    if (mode == 'd') {
        Reader reader;
        init_reader(&reader, input);
        if (!feof(reader.input)) {
            Node* root = read_node(&reader);
            size_t lbo = read_number(&reader, 3);
            lbo = (lbo == 0) ? 8 : lbo;

            decompress(&reader, output, root, lbo);
            delete_tree(root);
        }
    }
}

void console_handler(int argc, char** argv) {
    /**
     * @brief Обрабатывает аргументы командной строки и вызывает архивацию/распаковку.
     * 
     * Проверяет корректность аргументов, открывает файлы и вызывает archiver().
     * 
     * @param argc Количество аргументов командной строки.
     * @param argv Массив строк с аргументами командной строки.
     */
    if (argc == 4) {
        if (strcmp(argv[1], "c") == 0 || strcmp(argv[1], "d") == 0) {
            char mode = argv[1][0];
            FILE* input = fopen(argv[2], "rb");
            FILE* output = fopen(argv[3], "wb");
            if (input && output) {
                archiver(input, output, mode);
            }
            fclose(input);
            fclose(output);
        }
    }
}

int main(int argc, char** argv) {
    /**
     * @brief Точка входа программы.
     * 
     * Вызывает console_handler с аргументами командной строки.
     * 
     * @param argc Количество аргументов командной строки.
     * @param argv Массив строк с аргументами командной строки.
     * @return EXIT_SUCCESS при завершении.
     */
    console_handler(argc, argv);
    return EXIT_SUCCESS;
}
