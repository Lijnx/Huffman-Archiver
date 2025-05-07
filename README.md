# Huffman Archiver

Простой архиватор, использующий алгоритм сжатия Хаффмана.

## 🔹 Описание
Проект реализует сжатие и распаковку файлов на основе алгоритма Хаффмана — эффективного метода сжатия данных без потерь.

## 🔹 Сборка

### Linux
1. Убедитесь, что установлены `gcc` и `make`.
2. Клонируйте репозиторий:
  ```sh
  git clone https://github.com/Lijnx/Huffman-Archiver.git
  cd Huffman-Archiver
  ```
3. Соберите проект с помощью скрипта `build_linux.sh`.

### Windows
1. Установите `MinGW` или аналогичный компилятор.
2. Клонируйте репозиторий (через Git Bash или вручную).
3. Соберите проект с помощью скрипта `build_windows.bat`.

## 🔹 Использование
  ```sh
  # Сжатие файла
  ./huffman_archiver c input.txt output.huff
  
  # Распаковка файла
  ./huffman_archiver d output.huff decompressed.txt
  ```
