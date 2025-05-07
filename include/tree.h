#pragma once
#include <stdlib.h>

/**
 * Структура узла дерева Хаффмана.
 * 
 * Каждый узел может содержать:
 * - значение символа (value),
 * - указатели на левого и правого потомков.
 */
typedef struct Node {
    unsigned char value;
    struct Node* left;
    struct Node* right;
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
