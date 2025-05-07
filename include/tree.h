#pragma once
#include <stdlib.h>

/**
 * Структура узла дерева Хаффмана.
 */
typedef struct Node {
    unsigned char value;    ///< Значение узла (символ).
    struct Node* left;      ///< Указатель на левого потомка.
    struct Node* right;     ///< Указатель на правого потомка.
} Node;

/**
 * Проверяет, является ли узел листом.
 */
int is_leaf(Node *node);

/**
 * Создаёт новый узел с заданными значением и потомками.
 */
Node* new_node(unsigned char value, Node *left, Node *right);

/**
 * Рекурсивно удаляет дерево, начиная с указанного узла.
 */
void delete_tree(Node *node);
