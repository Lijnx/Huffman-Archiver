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
 * @param writer Указатель на структуру Writer.
 * @param output Указатель на файл, в который будет вестись запись.
 */
void init_writer(Writer *writer, FILE* output);

/**
 * Записывает один бит в выходной поток.
 * @param writer Указатель на Writer.
 * @param bit Значение бита (0 или 1).
 */
void write_bit(Writer *writer, unsigned char bit);

/**
 * Записывает байт в поток, с учётом уже записанных битов.
 * @param writer Указатель на Writer.
 * @param byte Байт для записи.
 */
void write_byte(Writer *writer, unsigned char byte);

/**
 * Записывает целое число побитно, начиная с самого старшего бита.
 * @param writer Указатель на Writer.
 * @param number Число для записи.
 * @param bits Количество битов для записи.
 */
void write_number(Writer *writer, unsigned number, size_t bits);

/**
 * Записывает оставшиеся в буфере биты в файл, дополняя их нулями.
 * Вызывать в конце записи.
 * @param writer Указатель на Writer.
 */
void write_last(Writer *writer);

/**
 * Инициализирует структуру Reader.
 * @param reader Указатель на структуру Reader.
 * @param input Указатель на входной файл.
 */
void init_reader(Reader *reader, FILE* input);

/**
 * Считывает один бит из входного потока.
 * @param reader Указатель на Reader.
 * @return Прочитанный бит (0 или 1).
 */
unsigned char read_bit(Reader *reader);

/**
 * Считывает байт из входного потока с учётом побитового буфера.
 * @param reader Указатель на Reader.
 * @return Прочитанный байт.
 */
unsigned char read_byte(Reader *reader);

/**
 * Считывает целое число, представленное заданным количеством битов.
 * @param reader Указатель на Reader.
 * @param bits Количество битов для чтения.
 * @return Прочитанное число.
 */
unsigned read_number(Reader *reader, size_t bits);
