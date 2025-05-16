#include "functions.h"

int main(void) {
    // Загрузка изображения
    int width, height, channels;
    unsigned char *image = stbi_load("input.png", &width, &height, &channels, 0);
    if (!image) {
        printf("Error loading image\n");
        return -1;
    }

    // тестовый прикол, меням цвета местами
    for(int i = 0; i < height * width * channels; i += channels)
    {
        char temp = image[i];
        char temp_2 = image[i + 1];

        image[i] = image[i + 2];
        image[i + 1] = temp_2;
        image[i + 2] = temp;

    }
    // Сохранение картинки
    stbi_write_png("output.jpg", width, height, channels, image, width * channels);
    // Освобождение памяти
    stbi_image_free(image);

    return 0;
}