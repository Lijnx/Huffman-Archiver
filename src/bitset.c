#include "bitset.h"

void init_bitset(Bitset *bs) {
    /**
     * @brief Инициализирует битсет
     * 
     * Устанавливает размер в 0 и очищает массив байтов, зануляя все биты.
     * 
     * @param bs Указатель на структуру Bitset.
     */
    bs->size = 0;
    memset(bs->bitset, 0, MAX_BYTES);
}

int bitset_is_full(Bitset bs) {
    /**
     * @brief Проверяет, достиг ли битсет максимального размера (MAX_SIZE бит).
     *
     * @param bs Структура битсета
     * @return 1 - если битсет полон, 0 - в противном случае.
     */
    return (bs.size == MAX_SIZE) ? 1 : 0;
}

void append_bit(Bitset *bs, unsigned char bit) {
    /**
     * @brief Добавляет один бит в конец битсета, сдвигая все существующие биты влево.
     *
     * Алгоритм:
     * - Определяет байт, с которого начинается сдвиг.
     * - Сдвигает байты влево, начиная с младших к старшим, чтобы освободить место справа.
     * - Добавляет новый бит в крайний правый (младший) байт.
     * - Увеличивает размер битсета.
     * Если битсет уже полон - выводит сообщение об ошибке в stderr.
     * 
     * @param bs Указатель на Bitset.
     * @param bit Бит, который необходимо добавить.
     */
    if (!bitset_is_full(*bs)) {
        size_t first_byte = (MAX_BYTES - 1) - (bs->size >> 3);
        for (size_t i = first_byte; i < MAX_BYTES - 1; i++) {
            bs->bitset[i] <<= 1;
            bs->bitset[i] += ((bs->bitset[i + 1] >> 7) & 1);
        }
        bs->bitset[MAX_BYTES - 1] <<= 1;
        bs->bitset[MAX_BYTES - 1] += (bit & 1);
        bs->size++;
    } else {
        fputs("Bitset Overflow", stderr);
    }
}

int get_bit(Bitset bs, size_t pos) {
    /**
     * @brief Возвращает значение бита по указанной позиции в битсете.
     *
     * Биты считаются от старшего к младшему (слева направо).
     * Алгоритм:
     * - Вычисляется смещение: на какой байт и бит приходится позиция.
     * - Полученный бит извлекается побитовой маской.
     *
     * @param bs Структура битсета.
     * @param pos - индекс бита от 0 до size - 1.
     * @return 0 или 1 - значение бита, либо -1 при выходе за границы.
     */
    if (pos < bs.size) {
        size_t byte = (MAX_SIZE - bs.size + pos) >> 3;
        size_t offset = (MAX_SIZE - bs.size + pos) - (byte << 3);
        return (bs.bitset[byte] >> (7 - offset)) & 1;
    }
    return -1;
}

int rewrite_at(Bitset *bs, size_t pos, unsigned char bit) {
    /**
     * @brief Перезаписывает существующий бит в битсете по указанной позиции.
     *
     * Алгоритм:
     * - Вычисляет байт и бит в этом байте.
     * - Сбрасывает или устанавливает бит с помощью побитовых операций.
     *
     * @param bs Указатель на Bitset.
     * @param pos - позиция бита для перезаписи.
     * @param bit - новое значение (0 или 1).
     * @return новое значение бита (0 или 1), либо -1 при некорректной позиции.
     */
    if (pos < bs->size) {
        size_t byte = (MAX_SIZE - bs->size + pos) >> 3;
        size_t offset = (MAX_SIZE - bs->size + pos) - (byte << 3);
        if (bit)
            bs->bitset[byte] |= (1 << (7 - offset));
        else
            bs->bitset[byte] &= ~(1 << (7 - offset));
        return (bs->bitset[byte] >> (7 - offset)) & 1;
    }
    return -1;
}

void copy_bitset(Bitset *dst, Bitset src) {
    /**
     * @brief Копирует биты из одного битсета в другой.
     *
     * Копирование происходит поэлементно:
     * - Каждый бит читается из исходного битсета.
     * - Добавляется в целевой битсет с сохранением порядка.
     *
     * @param dst Указатель на Bitset, в который происходит копирование.
     * @param src Структура битсета, из которого происходит копирование.
     */
    for (size_t i = 0; i < src.size; i++)
        append_bit(dst, get_bit(src, i));
}

void print_bitset(Bitset bs, Writer *writer) {
    /**
     * @brief Выводит биты битсета в поток записи.
     *
     * Для каждого бита:
     * - Получает значение с помощью get_bit.
     * - Записывает бит через write_bit().
     *
     * @param bs Структура битсета.
     * @param writer Указатель на структуру Writer, в которую будет производиться запись.
     */
    for (size_t i = 0; i < bs.size; i++)
        write_bit(writer, get_bit(bs, i));
}
