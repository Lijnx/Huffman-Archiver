#!/bin/bash

# Проверка наличия GCC в системе
if ! command -v gcc &> /dev/null
then
    echo "GCC не найден. Установите его перед продолжением."
    exit 1
fi

# Компиляция проекта
gcc -Wall -Wextra -O2 -Iinclude src/bitio.c src/bitset.c src/queue.c src/tree.c src/main.c -o huffman_archiver


# Проверка успешности компиляции
if [ $? -eq 0 ]; then
    echo "Сборка успешно завершена. Исполняемый файл: ./huffman_archiver"
else
    echo "Ошибка при сборке."
    exit 1
fi