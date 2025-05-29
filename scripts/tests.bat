@echo off

REM Переход в директорию проекта (на уровень выше скрипта)
cd /d %~dp0\..

REM Проверка resize
imgproc inputs/snow.jpg -resize 0.5 0.5 tests/resize_0.png
imgproc inputs/forestcat.jpg -resize 2 2 tests/resize_1.png
imgproc inputs/pole.jpg -resize 6 1 tests/resize_2.jpg

REM проверка median filter
imgproc inputs/snp0.jpg -median 3 tests/median_0.jpg
imgproc inputs/snp1.png -median 5 tests/median_1.png
imgproc inputs/snp3.jpg -median 11 tests/median_2.png

REM проверка gaus filter
imgproc inputs/snp2.jpg -gaus 3 10 tests/gaus_0.png
imgproc inputs/snp4.jpg -gaus 5 50 tests/gaus_1.jpg
imgproc inputs/snp1.png -gaus 7 70 tests/gaus_2.png

REM проверка rotation
imgproc inputs/birb.jfif -rotate 45 tests/rotate_0.png
imgproc inputs/pole.jpg -rotate 90 tests/rotate_1.png
imgproc inputs/cats_.jfif -rotate 270 tests/rotate_2.png

REM проверка grayscale conversion
imgproc inputs/town.jpg -gray tests/gray_0.png
imgproc inputs/train.jpg -gray tests/gray_1.png

REM проверка Sharpening
imgproc inputs/forestcat.jpg -sharp tests/sharp_0.png
imgproc tests/gaus_0.png -sharp tests/sharp_1.png

REM проверка Histogram equalization
imgproc inputs/bnw0.png -hist tests/hist_0.png
imgproc inputs/bnw1.jpg -hist tests/hist_1.png
imgproc inputs/bnw2.jfif -hist tests/hist_2.png

REM проверка Edge detection(просто на фото)
imgproc inputs/town.jpg -edge tests/edge_0.png
imgproc inputs/bnw3.jpg -edge tests/edge_1.png
imgproc inputs/forestcat.jpg -edge tests/edge_2.png
imgproc inputs/train.jpg -edge tests/edge_3.png
imgproc inputs/snow.jpg -edge tests/edge_4.png

REM проверка Edge detection(с заранее применённой Histogram equalization)
imgproc inputs/town.jpg -hist tests/hist_3.png
imgproc tests/hist_3.png -edge tests/edge_hist_0.png

imgproc inputs/train.jpg -hist tests/hist_4.png
imgproc tests/hist_4.png -edge tests/edge_hist_1.png

imgproc inputs/snow.jpg -hist tests/hist_4.png
imgproc tests/hist_4.png -edge tests/edge_hist_2.png
