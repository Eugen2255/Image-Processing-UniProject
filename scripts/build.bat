@echo off
REM Переход в директорию проекта (на уровень выше скрипта)
cd /d %~dp0\..

REM Компиляция проекта
echo Compilation...
gcc -o imgproc.exe ^
    main.c ^
    src\median_filter.c ^
    src\side_functions.c ^
    src\gaussian_blur.c ^
    src\convolution.c ^
    src\greing.c ^
    src\histogram.c ^
    src\rotation.c ^
    src\resize.c ^
    -Iinclude

REM Проверка успешности компиляции
if %errorlevel% equ 0 (
    echo Build completed. File: imgproc.exe
) else (
    echo Error occured.
    exit /b 1
)