#pragma once
#include <stdlib.h>
#include <stdio.h>

/**
 * Структура для записи битов в файл.
 */
typedef struct {
    FILE* output;             ///< Указатель на выходной файл.
    unsigned char byte;       ///< Буфер для накопления битов.
    size_t bits_filled;       ///< Количество заполненных битов в буфере.
} Writer;

/**
 * Структура для чтения битов из файла.
 */
typedef struct {
    FILE* input;              ///< Указатель на входной файл.
    unsigned char byte;       ///< Буфер для текущего байта при побитовой обработке.
    unsigned char buf;        ///< Следующий байт из файла.
    size_t bits_filled;       ///< Количество оставшихся битов для чтения в текущем байте.
} Reader;

/**
 * Инициализирует структуру Writer.
 */
void init_writer(Writer *writer, FILE* output);

/**
 * Записывает один бит в выходной поток.
 */
void write_bit(Writer *writer, unsigned char bit);

/**
 * Записывает байт в поток, с учётом уже записанных битов.
 */
void write_byte(Writer *writer, unsigned char byte);

/**
 * Записывает целое число побитно, начиная с самого старшего бита.
 */
void write_number(Writer *writer, unsigned number, size_t bits);

/**
 * Записывает оставшиеся в буфере биты в файл, дополняя их нулями.
 */
void write_last(Writer *writer);

/**
 * Инициализирует структуру Reader.
 */
void init_reader(Reader *reader, FILE* input);

/**
 * Считывает один бит из входного потока.
 */
unsigned char read_bit(Reader *reader);

/**
 * Считывает байт из входного потока с учётом побитового буфера.
 */
unsigned char read_byte(Reader *reader);

/**
 * Считывает целое число, представленное заданным количеством битов.
 */
unsigned read_number(Reader *reader, size_t bits);
