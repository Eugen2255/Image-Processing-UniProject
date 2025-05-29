/**
 * @file convolution.c
 * @brief Implementation of sharpening and edge detection
 */

#include "functions.h"

/**
 * @brief Performs matrix convolution on an image for sharpening or edge detection
 * @param input_path Path to input image file
 * @param output_path Path to save processed image
 * @param mode Operation mode:
 *             0 - Image sharpening (uses center coefficient 5)
 *             1 - Edge detection (uses center coefficient 4 and converts to grayscale first)
 * @return 0 on success, -1 on error
 * 
 * @details The function applies one of two convolution operations:
 *          - Sharpening: Enhances image details using a 3x3 sharpening kernel
 *          - Edge detection: Highlights edges using a Laplacian-like kernel
 * 
 *          The convolution matrix used:
 *          [ 0 -1  0]
 *          [-1  k -1]  where k=5 for sharpening, k=4 for edge detection
 *          [ 0 -1  0]
 * 
 * @note For edge detection mode, the image is automatically converted to grayscale
 * @warning Input must be a valid image path, output path must have .png or .jpg extension
 */
int matrix_convolution(char* input_path, char* output_path, int mode)
{
    /* Load input image using stb_image library */
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }

    /* Allocate temporary buffer for processed image */
    unsigned char* temp = (unsigned char*)malloc(width * height * channels);
    if (!temp) 
    {
        stbi_image_free(image);
        printf("Error: Memory allocation failed!\n");
        return -1;
    }

    /* Set convolution matrix center coefficient based on operation mode */
    int coef;
    if(mode == 0)
    {
        coef = 5; // Sharpening coefficient
    }
    else if (mode == 1)
    {
        coef = 4; // Edge detection coefficient
        /* Convert to grayscale for edge detection */
        image = gradation_gray(image, height, width, channels);
    }
    else
    {
        printf("Invalid mode selected. Use 0 (sharpening) or 1 (edge detection)\n");
        return -1;
    }

    /* Allocate and initialize 3x3 convolution matrix */
    int** matrix = (int**)malloc(3 * sizeof(int*));
    if (!matrix) 
    {
        free(temp);
        stbi_image_free(image);
        printf("Error: Memory allocation failed!\n");
        return -1;
    }

    /* Create matrix with pattern: 0 at corners and center, -1 elsewhere */
    for(int i = 0; i < 3; i++)
    {
        matrix[i] = (int*)malloc(3 * sizeof(int));
        if (!matrix[i]) 
        {
            /* Cleanup already allocated rows if allocation fails */
            for(int k = 0; k < i; k++) free(matrix[k]);
            free(temp);
            stbi_image_free(image);
            free(matrix);
            printf("Error: Memory allocation failed!\n");
            return -1;
        }
        
        /* Fill matrix with pattern */
        for(int j = 0; j < 3; j++)
        {
            if((i + j) % 2 == 0) // Diagonal and center positions
            {
                matrix[i][j] = 0;
            }
            else // Other positions
            {
                matrix[i][j] = -1;
            }
        }
    }
    matrix[1][1] = coef; // Set center coefficient based on mode

    /* Apply convolution to each pixel */
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            for(int k = 0; k < channels; k++)
            {
                double sum = 0;
                /* Convolve 3x3 neighborhood with matrix */
                for(int dy = -1; dy <= 1; dy++)
                {   
                    for(int dx = -1; dx <= 1; dx++)
                    {   
                        /* Handle image boundaries by clamping coordinates */
                        int y = get_cord(i + dy, height);
                        int x = get_cord(j + dx, width);
                        /* Weighted sum using matrix values */
                        sum += image[(y * width + x) * channels + k] * matrix[dy + 1][dx + 1];
                    }
                }
                /* Clamp result to valid pixel range [0,255] */
                if (sum > 255) sum = 255;
                else if (sum < 0) sum = 0;
                temp[(i * width + j) * channels + k] = sum;    
            }
        }
    }

    /* Save processed image in appropriate format */
    int res;
    if (strstr(output_path, ".png")) 
    {
        res = stbi_write_png(output_path, width, height, channels, temp, width * channels);
    }
    else if (strstr(output_path, ".jpg"))
    {
        res = stbi_write_jpg(output_path, width, height, channels, temp, 100);
    } 
    else 
    {
        printf("Unsupported format. Use .png or .jpg\n");
        res = 0; // Indicate failure
    }

    /* Free all allocated resources */
    for(int i = 0; i < 3; i++) free(matrix[i]);
    free(matrix);
    free(temp);
    stbi_image_free(image);

    /* Check if save operation was successful */
    if(!res)
    {
        printf("Error writing image\n");
        return -1;
    }
    
    return 0;
}