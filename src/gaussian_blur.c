#include "functions.h"


int gaussian_blur(char* input_path, char* output_path, int size, double sigma)
{
     // Загрузка изображения
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }

    if (size <= 0) 
    {
        printf("Error: Filter size must be positive!\n");
        return -1;
    }
    if (size % 2 == 0) 
    {
        printf("Error: Filter size must be odd!\n");
        return -1;
    }
    if (size > height || size > width) 
    {
        printf("Error: Filter size exceeds image dimensions!\n");
        return -1;
    }

    double** kernel = (double**) malloc (size * sizeof(double*));
    if (!kernel) 
    {
        printf("Error: Memory allocation failed!\n");
        return -1;
    }

    unsigned char* temp = (unsigned char*) malloc (width * height * channels * sizeof(unsigned char));
    if (!temp) 
    {
        printf("Error loading image\n");
        return -1;
    }
    
    //memcpy(temp, image, sizeof(image));

    // calcute the kernel
    double sum = 0;        // variable for normalizing
    int center = size / 2;  // coords of the centre of kernel
    sigma *= sigma;         // only using (sigma ^ 2) in expression below

    for (int i = 0; i < size; i++) 
    {
        kernel[i] = (double*) malloc (size * sizeof(double));
        if (!kernel[i]) 
        {
            printf("Error: Memory allocation failed!\n");
            return -1;
        }

        for (int j = 0; j < size; j++)
        {
            // getting coords relatively the centre of kernel
            int x = i - center;         
            int y = j - center;
            kernel[i][j] = exp((x * x + y * y) / (-2 * sigma)) / (2 * 3.1415926535 * sigma); // by fornula
            sum += kernel[i][j];
        }
    }

    // Normalazing (sum of weight = 1)
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++) 
        {
            kernel[i][j] /= sum;
        }
    }
    double term = 0;
    // applying gaus blur
    int radius = size / 2;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < channels; k++) {
                double sum = 0.0;

                for (int dy = -radius; dy <= radius; dy++) {
                    for (int dx = -radius; dx <= radius; dx++) {
                        int y = get_cord(i + dy, height);
                        int x = get_cord(j + dx, width);
                        sum += image[(y * width + x) * channels + k] * kernel[dy + radius][dx + radius];
                    }
                }

                // Обрезаем значение до [0, 255] и записываем
                if (sum < 0) sum = 0;
                if (sum > 255) sum = 255;
                temp[(i * width + j) * channels + k] = sum;
            }
        }
    }

    stbi_write_png(output_path, width, height, channels, temp, width * channels);

    for(int i = 0; i < size; i++)
    {
        free(kernel[i]);
    }
    free(kernel);
    free(temp);
    stbi_image_free(image);
    return 0;
}