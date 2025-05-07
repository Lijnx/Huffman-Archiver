#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

/**
 * Структура элемента очереди с приоритетом.
 */
typedef struct Item {
    Node *value;                    ///< Указатель на узел дерева Хаффмана.
    unsigned long long priority;    ///< Приоритет в очереди.
    struct Item *next;              ///< Указатель на следующий элемент в очереди.
    struct Item *prev;              ///< Указатель на предыдущий элемент в очереди.
} Item;

/**
 * Структура очереди с приоритетом.
 */
typedef struct Queue {
    size_t length;  ///< Длина очереди.
    Item *first;    ///< Указатель на первый элемент очереди.
    Item *last;     ///< Указатель на последний элемент очереди.
} Queue;

/**
 * Инициализирует очередь.
 */
void init_queue(Queue *queue);

/**
 * Проверяет, пуста ли очередь.
 */
int is_empty(Queue queue);

/**
 * Добавляет элемент в очередь по приоритету (по возрастанию).
 */
void enqueue(Queue *queue, Node *node, unsigned long long priority);

/**
 * Возвращает узел дерева из первого элемента очереди.
 */
Node *get_node(Queue queue);

/**
 * Возвращает приоритет первого элемента в очереди.
 */
unsigned long long get_priority(Queue queue);

/**
 * Удаляет первый элемент из очереди.
 */
void dequeue(Queue *queue);

/**
 * Удаляет всю очередь и освобождает память всех связанных узлов.
 */
void delete_queue(Queue *queue);
