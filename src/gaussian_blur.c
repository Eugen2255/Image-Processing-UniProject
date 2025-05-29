/**
 * @file gaussian_blur.c
 * @brief Implementation of gauss filter
 */
#include "functions.h"

/**
 * @brief Applies Gaussian blur filter to an image
 * @param input_path Path to input image file (supported formats: JPG, PNG)
 * @param output_path Path to save blurred image (supported formats: JPG, PNG)
 * @param size Size of the Gaussian kernel (must be odd and positive)
 * @param sigma Standard deviation of Gaussian distribution
 * @return 0 on success, -1 on error
 */
int gaussian_blur(char* input_path, char* output_path, int size, double sigma)
{
    // Validate kernel size parameters
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

    // Load image using stb_image library
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }

    // Check if kernel size is larger than image dimensions
    if (size > height || size > width) 
    {
        stbi_image_free(image);
        printf("Error: Filter size exceeds image dimensions!\n");
        return -1;
    }

    // Allocate memory for Gaussian kernel (size x size matrix)
    double** kernel = (double**) malloc (size * sizeof(double*));
    if (!kernel) 
    {
        stbi_image_free(image);
        printf("Error: Memory allocation failed!\n");
        return -1;
    }

    // Allocate temporary buffer for processed image
    unsigned char* temp = (unsigned char*) malloc (width * height * channels * sizeof(unsigned char));
    if (!temp) 
    {
        stbi_image_free(image);
        free(kernel);
        printf("Error: Memory allocation failed!\n");
        return -1;
    }
    
    // Compute Gaussian kernel values
    double sum = 0;         // For kernel normalization
    int center = size / 2;  // Center position of the kernel
    sigma *= sigma;         // Using sigma squared for calculation

    // Calculate each element of the Gaussian kernel
    for (int i = 0; i < size; i++) 
    {
        kernel[i] = (double*) malloc (size * sizeof(double));
        if (!kernel[i]) 
        {   
            // Cleanup already allocated memory if allocation fails
            for(int k = 0; k < i; k++)
            {
                free(kernel[k]);
            }
            free(temp);
            stbi_image_free(image);
            printf("Error: Memory allocation failed!\n");
            return -1;
        }

        // Fill kernel row with Gaussian values
        for (int j = 0; j < size; j++)
        {
            // Coordinates relative to kernel center
            int x = i - center;         
            int y = j - center;
            // Gaussian function formula
            kernel[i][j] = exp((x * x + y * y) / (-2 * sigma)) / (2 * PI * sigma);
            sum += kernel[i][j];
        }
    }

    // Normalize kernel so that sum of all elements equals 1
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++) 
        {
            kernel[i][j] /= sum;
        }
    }

    // Apply Gaussian blur to image
    int radius = size / 2;
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            for (int k = 0; k < channels; k++) 
            {
                double sum = 0.0;

                // Convolve kernel with image neighborhood
                for (int dy = -radius; dy <= radius; dy++) 
                {
                    for (int dx = -radius; dx <= radius; dx++) 
                    {
                        // Handle edges by clamping coordinates
                        int y = get_cord(i + dy, height);
                        int x = get_cord(j + dx, width);
                        // Weighted sum using kernel values
                        sum += image[(y * width + x) * channels + k] * kernel[dy + radius][dx + radius];
                    }
                }

                // Clamp result to valid pixel value range [0,255]
                if (sum > 255) sum = 255;
                temp[(i * width + j) * channels + k] = sum;
            }
        }
    }

    // Save processed image in appropriate format
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
        res = 1;
    }

    // Free allocated memory
    for(int i = 0; i < size; i++)
    {
        free(kernel[i]);
    }
    free(kernel);
    free(temp);
    stbi_image_free(image);

    // Check if image was saved successfully
    if(!res)
    {
        printf("Error writing image\n");
        return -1;
    }
    else
    {
        return 0;    
    }
}