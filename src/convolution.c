#include "functions.h"

/*
    mode = 0              mode = 1
    sharpness upscale and edge detection

    using matrix 3x3:   0 -1  0
                       -1  k -1
                        0 -1  0

    by applying matrix on image using Matrix convolution, 
    we make it more sharper/ detect edges

    formule: image_fragment(3x3) * matrix(3x3) * 1/div   (div = 1)
*/

int matrix_convolution(char* input_path, char* output_path, int mode)
{
    // Загрузка изображения
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }

    unsigned char* temp = (unsigned char*) malloc (width * height * channels * sizeof(unsigned char));
    if (!temp) 
    {
        printf("Error: Memory allocation failed!\n");
        return -1;
    }

    int coef;
    if(mode == 0)
    {
        coef = 5; // sharping
    }
    else if (mode == 1)
    {
        coef = 4; // edge detection
        image = gradation_gray(image, height, width, channels);
    }
    else
    {
        printf("Shouldn't have happend...");
        return -1;
    }

    // creating matrix 3x3 
    int** matrix = (int**) malloc (3 * sizeof(int*));
    if (!matrix) 
    {
        printf("Error: Memory allocation failed!\n");
        return -1;
    }

    for(int i = 0; i < 3; i++)
    {
        matrix[i] = (int*) malloc (3 * sizeof(int));
        if (!matrix[i]) 
        {
            for(int k = 0; k < i; k++)
            {
                free(matrix[k]);
            }
            printf("Error: Memory allocation failed!\n");
            return -1;
        }
        for(int j = 0; j < 3; j++)
        {
            if((i + j) % 2 == 0) // corners + centre
            {
                matrix[i][j] = 0;
            }
            else
            {
                matrix[i][j] = -1;
            }
        }
    }
    matrix[1][1] = coef; // place coef in centre of matrix
   
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            for(int k = 0; k < channels; k++)
            {
                double sum = 0;
                for(int dy = -1; dy <= 1; dy++)
                {
                    for(int dx = -1; dx <= 1; dx++)
                    {   
                        int y = get_cord(i + dy, height);
                        int x = get_cord(j + dx, width);
                        sum += image[(y * width + x) * channels + k] * matrix[dy + 1][dx + 1];
                    }
                }
                // fitting value into char size borders
                if (sum > 255) sum = 255;
                else if (sum < 0) sum = 0;
                temp[(i * width + j) * channels + k] = sum;    
            }
        }
    }

    if(!stbi_write_png(output_path, width, height, channels, temp, width * channels))
    {
        printf("Error writing image\n");
        return -1;
    }

    // freeing the data
    for(int i = 0; i < 3; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    free(temp);
    stbi_image_free(image);
    return 0;
}