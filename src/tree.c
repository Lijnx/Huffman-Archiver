#include "tree.h"

int is_leaf(Node *node) {
    /**
     * @brief Проверяет, является ли данный узел листом дерева Хаффмана.
     *
     * Условие:
     * - Узел считается листом, если у него нет потомков (оба указателя - NULL).
     *
     * @param node Указатель на узел дерева.
     * @return 1 - если это лист; 0 - если не лист; -1 - если узел NULL.
     */
    if (node)
        return (!node->left && !node->right) ? 1 : 0;
    else
        return -1;
}

Node* new_node(unsigned char value, Node *left, Node *right) {
    /**
     * @brief Создаёт новый узел дерева Хаффмана.
     *
     * Выделяет память под новый узел и инициализирует:
     * - значение символа (value),
     * - указатели на левого и правого потомков.
     *
     * @param value Байт (символ), который хранит узел.
     * @param left Указатель на левого потомка.
     * @param right Указатель на правого потомка.
     * @return указатель на созданный узел, либо NULL при ошибке выделения памяти.
     */
    Node* new = (Node*)malloc(sizeof(Node));
    if (new) {
        new->value = value;
        new->left = left;
        new->right = right;
    }
    return new;
}

void delete_tree(Node *node) {
    /**
     * @brief Рекурсивно освобождает память, занятую деревом Хаффмана.
     *
     * Для каждого узла вызывает delete_tree на его потомках,
     * затем освобождает текущий узел.
     *
     * @param node Корень поддерева (или всего дерева), которое нужно удалить.
     */
    if (node) {
        delete_tree(node->left);
        delete_tree(node->right);
        free(node);
    }
}
