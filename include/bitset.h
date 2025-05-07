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
 * @param bs Указатель на Bitset.
 */
void init_bitset(Bitset *bs);

/**
 * Проверяет, достиг ли битсет своего максимального размера.
 * @param bs Битсет для проверки.
 * @return 1 — если битсет полон, 0 — иначе.
 */
int bitset_is_full(Bitset bs);

/**
 * Добавляет бит в конец битсета, сдвигая предыдущие биты влево.
 * @param bs Указатель на Bitset.
 * @param bit Добавляемый бит (0 или 1).
 */
void append_bit(Bitset *bs, unsigned char bit);

/**
 * Получает бит по позиции в битсете.
 * @param bs Битсет.
 * @param pos Позиция бита (от 0 до size - 1).
 * @return Значение бита (0 или 1), либо -1 при ошибке.
 */
int get_bit(Bitset bs, size_t pos);

/**
 * Перезаписывает бит в заданной позиции новым значением.
 * @param bs Указатель на Bitset.
 * @param pos Позиция перезаписываемого бита.
 * @param bit Новое значение бита (0 или 1).
 * @return Новое значение бита, либо -1 при ошибке.
 */
int rewrite_at(Bitset *bs, size_t pos, unsigned char bit);

/**
 * Копирует содержимое одного битсета в другой.
 * @param dst Указатель на битсет-приёмник.
 * @param src Исходный битсет.
 */
void copy_bitset(Bitset *dst, Bitset src);

/**
 * Записывает биты из битсета в поток, используя Writer.
 * @param bs Битсет для записи.
 * @param writer Указатель на Writer.
 */
void print_bitset(Bitset bs, Writer *writer);
