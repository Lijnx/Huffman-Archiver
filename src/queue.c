#include "queue.h"

void init_queue(Queue *queue) {
    /**
     * Инициализирует очередь.
     *
     * Устанавливает длину в 0, а указатели на первый и последний элемент — в NULL.
     *
     * @param queue — указатель на структуру очереди.
     */
    queue->length = 0;
    queue->last = NULL;
    queue->first = NULL;
}

int is_empty(Queue queue) {
    /**
     * Проверяет, пуста ли очередь.
     *
     * @param queue — структура очереди.
     * @return 1 — если очередь пуста; 0 — иначе.
     */
    return (queue.length == 0) ? 1 : 0;
}

Item* new_item(Node *node, unsigned long long priority, Item *prev, Item *next) {
    /**
     * Создаёт новый элемент очереди с заданным значением и приоритетом.
     *
     * @param node — указатель на узел дерева Хаффмана.
     * @param priority — приоритет элемента.
     * @param prev — предыдущий элемент.
     * @param next — следующий элемент.
     * @return указатель на созданный элемент или NULL при ошибке выделения памяти.
     */
    Item* new = (Item*)malloc(sizeof(Item));
    if (new) {
        new->value = node;
        new->priority = priority;
        new->prev = prev;
        new->next = next;
    }
    return new;
}

void enqueue(Queue *queue, Node *node, unsigned long long priority) {
    /**
     * Добавляет элемент в очередь по приоритету (по возрастанию).
     *
     * Вставляет новый элемент в отсортированное место, сохраняя порядок.
     * При нехватке памяти выводит сообщение и удаляет переданный узел.
     *
     * @param queue — указатель на очередь.
     * @param node — узел дерева, связанный с элементом.
     * @param priority — приоритет элемента.
     */
    Item* new = new_item(node, priority, NULL, NULL);
    if (new) {
        if (!is_empty(*queue)) {
            if (new->priority <= queue->first->priority) {
                queue->first->prev = new;
                new->next = queue->first;
                queue->first = new;
                queue->length++;
            }
            else if (new->priority >= queue->last->priority) {
                queue->last->next = new;
                new->prev = queue->last;
                queue->last = new;
                queue->length++;
            }
            else {
                for (Item *l = queue->first->next, *r = queue->last->prev; ; l = l->next, r = r->prev) {
                    if (new->priority <= l->priority) {
                        Item *tmp = l->prev;
                        tmp->next = new;
                        new->prev = tmp;
                        l->prev = new;
                        new->next = l;
                        queue->length++;
                        break;
                    }
                    if (new->priority >= r->priority) {
                        Item *tmp = r->next;
                        tmp->prev = new;
                        new->next = tmp;
                        r->next = new;
                        new->prev = r;
                        queue->length++;
                        break;
                    }
                }
            }
        }
        else {
            queue->first = new;
            queue->last = new;
            queue->length++;
        }
    }
    else {
        fputs("Stack Overflow", stderr);
        delete_tree(node);
    }
}

Node *get_node(Queue queue) {
    /**
     * Возвращает узел дерева из первого элемента очереди.
     *
     * @param queue — очередь.
     * @return указатель на узел, либо NULL, если очередь пуста.
     */
    if (!is_empty(queue)) {
        return queue.first->value;
    }
    else return NULL;
}

unsigned long long get_priority(Queue queue) {
    /**
     * Возвращает приоритет первого элемента в очереди.
     *
     * @param queue — очередь.
     * @return приоритет первого элемента или 0, если очередь пуста.
     */
    if (!is_empty(queue))
        return queue.first->priority;
    else return 0;
}

void dequeue(Queue *queue) {
    /**
     * Удаляет первый элемент из очереди.
     *
     * Корректно освобождает память и обновляет указатели.
     * При попытке удалить из пустой очереди — сообщает об ошибке.
     *
     * @param queue — указатель на очередь.
     */
    if (!is_empty(*queue)) {
        if (queue->first == queue->last) {
            free(queue->first);
            queue->first = queue->last = NULL;
        }
        else {
            Item *tmp = queue->first->next;
            tmp->prev = NULL;
            free(queue->first);
            queue->first = tmp;
        }
        queue->length--;
    }
    else fputs("Queue Underflow", stderr);
}

void delete_queue(Queue *queue) {
    /**
     * Удаляет всю очередь и освобождает память всех связанных узлов.
     *
     * Также вызывает delete_tree() для каждого узла,
     * связанного с элементами очереди.
     *
     * @param queue — указатель на очередь.
     */
    while (!is_empty(*queue)) {
        delete_tree(get_node(*queue));
        dequeue(queue);
    }
}
