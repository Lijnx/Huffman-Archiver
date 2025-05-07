@echo off

:: Проверка наличия GCC в системе
where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo Ошибка: GCC не установлен или не добавлен в PATH.
    echo Установите MinGW или аналогичный пакет с GCC.
    pause
    exit /b 1
)

:: Компиляция проекта
gcc -Wall -Wextra -O2 -Iinclude src/bitio.c src/bitset.c src/queue.c src/tree.c src/main.c -o huffman_archiver.exe

:: Проверка успешности компиляции
if %errorlevel% equ 0 (
    echo Сборка успешно завершена. Исполняемый файл: huffman_archiver.exe
) else (
    echo Ошибка при сборке.
    pause
    exit /b 1
)

pause