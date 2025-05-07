#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitio.h"

/// Максимальное количество байтов в битсете.
enum { MAX_BYTES = 32 };

/// Максимальное количество битов в битсете.
enum { MAX_SIZE = 256 };

/**
 * Структура для хранения динамического набора битов фиксированной длины.
 */
typedef struct Bitset {
    size_t size;                       ///< Текущее количество битов в битсете.
    unsigned char bitset[MAX_BYTES];  ///< Массив байтов, хранящий биты.
} Bitset;

/**
 * Инициализирует битсет, обнуляя все биты и устанавливая размер в 0.
 */
void init_bitset(Bitset *bs);

/**
 * Проверяет, достиг ли битсет своего максимального размера.
 */
int bitset_is_full(Bitset bs);

/**
 * Добавляет бит в конец битсета, сдвигая предыдущие биты влево.
 */
void append_bit(Bitset *bs, unsigned char bit);

/**
 * Получает бит по позиции в битсете.
 */
int get_bit(Bitset bs, size_t pos);

/**
 * Перезаписывает бит в заданной позиции новым значением.
 */
int rewrite_at(Bitset *bs, size_t pos, unsigned char bit);

/**
 * Копирует содержимое одного битсета в другой.
 */
void copy_bitset(Bitset *dst, Bitset src);

/**
 * Записывает биты из битсета в поток, используя Writer.
 */
void print_bitset(Bitset bs, Writer *writer);
