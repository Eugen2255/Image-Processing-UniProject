#include "src/functions.h"

int main(int argc, char* argv[]) {
    if(argc < 4 || argc > 5)
    {
        printf("Not valid command!\n");
        return -1;
    }
    
    char* input = argv[1];
    char* mode = argv[2];
    // Загрузка изображения
    int width, height, channels;
    unsigned char* image = stbi_load(input, &width, &height, &channels, 0);
    if (!image) {
        printf("Error loading image\n");
        return -1;
    }

    // флаг результата, если 0 - все выполнилось
    int res = 0;
    if (argc == 5)
    {
        int val = atoi(argv[3]);
        char* output_path = argv[4];
        if (strcmp(mode, "-median") == 0) 
        {
            res = median_filter(image, output_path, height, width, channels, val);
        }
        else
        {
            printf("In progress\n");
        }
    }
    else // argc == 4
    {
        printf("In progress\n");
    }

    if(res == 0)
    {
        printf("All done!\n");
    }
    
    // Освобождение памяти
    stbi_image_free(image);
    return 0;
}