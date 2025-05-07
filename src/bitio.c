#include "bitio.h"

void init_writer(Writer *writer, FILE* output) {
    /**
     * @brief Инициализирует структуру Writer.
     *
     * Устанавливает выходной поток и сбрасывает буфер записи.
     *
     * @param writer Указатель на структуру Writer.
     * @param output Указатель на открытый файл для записи битов.
     */
    writer->output = output;
    writer->byte = 0;
    writer->bits_filled = 0;
}

void write_bit(Writer *writer, unsigned char bit) {
    /**
     * @brief Записывает один бит в буфер Writer.
     *
     * Когда буфер накапливает 8 бит, он сбрасывается в файл.
     *
     * @param writer Указатель на Writer.
     * @param bit Бит для записи (только младший значащий бит учитывается).
     */
    writer->byte <<= 1;
    writer->byte += (bit & 1);
    writer->bits_filled++;
    if (writer->bits_filled == 8) {
        fputc(writer->byte, writer->output);
        writer->byte = 0;
        writer->bits_filled = 0;
    }
}

void write_byte(Writer *writer, unsigned char byte) {
    /**
     * @brief Записывает байт с учётом состояния Writer.
     *
     * Если буфер Writer пуст — байт записывается напрямую.
     * Иначе данные сливаются побитово с текущим байтом в буфере.
     *
     * @param writer Указатель на Writer.
     * @param byte Байт для записи.
     */
    if (writer->bits_filled == 0) {
        fputc(byte, writer->output);
    }
    else {
        size_t shift = 8 - writer->bits_filled;
        writer->byte <<= shift;
        writer->byte += (byte >> writer->bits_filled);
        fputc(writer->byte, writer->output);
        writer->byte = (byte << shift) >> shift;
    }
}

void write_number(Writer *writer, unsigned number, size_t bits) {
    /**
     * @brief Записывает число побитно.
     *
     * Число разбивается на отдельные биты (старший бит первым) и записывается по одному.
     *
     * @param writer Указатель на Writer.
     * @param number Число для записи.
     * @param bits Количество бит, используемых для записи.
     */
    for (int j = (int)(bits - 1); j >= 0; j--) {
        write_bit(writer, (number >> j) & 1);
    }
}

void write_last(Writer *writer) {
    /**
     * @brief Завершает побитовую запись.
     *
     * Если в буфере остались биты, дополняет до целого байта нулями и записывает.
     *
     * @param writer Указатель на Writer.
     */
    if (writer->bits_filled != 0) {
        writer->byte <<= (8 - writer->bits_filled);
        fputc(writer->byte, writer->output);
        writer->byte = writer->bits_filled;
        writer->byte = 0;
        writer->bits_filled = 0;
    }
}

void init_reader(Reader *reader, FILE* input) {
    /**
     * @brief Инициализирует структуру Reader.
     *
     * Подготавливает Reader для побитового чтения из файла.
     *
     * @param reader Указатель на Reader.
     * @param input Указатель на открытый файл для чтения.
     */
    reader->input = input;
    reader->byte = 0;
    reader->buf = fgetc(reader->input);
    reader->bits_filled = 0;
}

unsigned char read_bit(Reader *reader) {
    /**
     * @brief Считывает один бит из Reader.
     *
     * Загружает новый байт из входа, если нужно.
     *
     * @param reader Указатель на Reader.
     * @return Прочитанный бит (0 или 1).
     */
    if (reader->bits_filled == 0) {
        reader->byte = reader->buf;
        reader->buf = fgetc(reader->input);
        reader->bits_filled = 8;
    }
    unsigned char bit = (reader->byte >> 7) & 1;
    reader->byte <<= 1;
    reader->bits_filled--;
    return bit;
}

unsigned char read_byte(Reader *reader) {
    /**
     * @brief Считывает один байт из Reader.
     *
     * Если буфер выровнен — возвращает байт напрямую.
     * Иначе собирает байт из двух фрагментов.
     *
     * @param reader Указатель на Reader.
     * @return Прочитанный байт.
     */
    if (reader->bits_filled == 0) {
        unsigned char tmp = reader->buf;
        reader->buf = fgetc(reader->input);
        return tmp;
    }
    else {
        reader->byte += (reader->buf >> reader->bits_filled);
        unsigned char tmp = reader->byte;
        reader->byte = reader->buf << (8 - reader->bits_filled);
        reader->buf = fgetc(reader->input);
        return tmp;
    }
}

unsigned read_number(Reader *reader, size_t bits) {
    /**
     * @brief Считывает заданное количество бит и собирает число.
     *
     * Биты считываются последовательно от старшего к младшему.
     *
     * @param reader Указатель на Reader.
     * @param bits Количество бит для считывания.
     * @return Прочитанное число.
     */
    unsigned result = 0;
    for (size_t i = 0; i < bits; i++) {
        result <<= 1;
        result += read_bit(reader);
    }
    return result;
}
