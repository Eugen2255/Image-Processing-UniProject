#!/bin/bash

# Переход в директорию проекта (на уровень выше скрипта)
cd "$(dirname "$0")/.."



# Компиляция проекта
echo "Compilation..."
gcc -o imgproc.exe \
    main.c \
    src/median_filter.c \
    src/side_functions.c \
    src/gaussian_blur.c \
    src/convolution.c \
    src/greing.c \
    src/histogram.c \
    src/rotation.c \
    src/resize.c \
    -Iinclude

# Проверка успешности компиляции
if [ $? -eq 0 ]; then
    echo "Build completed. File: imgproc.exe"
else
    echo "Error occured."
    exit 1
fi