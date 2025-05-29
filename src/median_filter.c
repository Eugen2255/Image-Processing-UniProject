/**
 * @file median_filter.c
 * @brief Implementation of median filter
 */
#include "functions.h"

/**
 * @brief Comparison function for qsort
 * @param a Pointer to first element to compare
 * @param b Pointer to second element to compare
 * @return Negative if a < b, positive if a > b, zero if equal
 * @note This is a static helper function for median_filter
 */
static int compare(const void *a, const void *b) 
{
    return (*(unsigned char*)a - *(unsigned char*)b);
}

/**
 * @brief Applies median filter to an image
 * @param input_path Path to the input image file (supported formats: JPG, PNG)
 * @param output_path Path to save the processed image (supported formats: JPG, PNG)
 * @param size Filter window size (must be positive odd number)
 * @return 0 on success, -1 on error
 * @details 
 * - Performs noise reduction by replacing each pixel with the median of neighboring pixels
 * - Handles edge pixels by clamping coordinates
 * - Supports multi-channel images (RGB/RGBA)
 * 
 * @note 
 * - Processing time increases with window size (O(n²) where n is window size)
 * - Large window sizes may cause loss of detail
 * 
 * @warning 
 * - Window size must be smaller than image dimensions
 * - Allocates temporary memory proportional to window size
 */
int median_filter(char* input_path, char* output_path, int size) 
{
    // Validate filter size
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

    // Load image using stb_image
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }

    // Check if filter size is appropriate for image dimensions
    if (size > height || size > width) 
    {
        stbi_image_free(image);
        printf("Error: Filter size exceeds image dimensions!\n");
        return -1;
    }
    
    // Calculate window radius and allocate memory for pixel neighborhood
    int radius = size / 2; 
    unsigned char* zone = malloc(size * size * sizeof(unsigned char));
    if (!zone) 
    {
        stbi_image_free(image);
        printf("Error: Memory allocation failed!\n");
        return -1;
    }

    // Process each pixel channel independently
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            for (int k = 0; k < channels; k++) 
            {
                int count = 0;
                // Collect neighboring pixels
                for (int dy = -radius; dy <= radius; dy++) 
                {
                    for (int dx = -radius; dx <= radius; dx++) 
                    {
                        int y = get_cord(i + dy, height);
                        int x = get_cord(j + dx, width);
                        zone[count] = image[(y * width + x) * channels + k];
                        count++;
                    }
                }

                // Sort pixels and take median value
                qsort(zone, count, sizeof(unsigned char), compare);
                image[(i * width + j) * channels + k] = zone[count / 2];
            }
        }
    }

    // Save processed image
    int res;
    const char* ext = strrchr(output_path, '.');
    if (ext) {
        if (strcmp(ext, ".png") == 0) 
        {
            res = stbi_write_png(output_path, width, height, channels, image, width * channels);
        }
        else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
        {
            res = stbi_write_jpg(output_path, width, height, channels, image, 100);
        } 
        else 
        {
            printf("Unsupported format. Use .png or .jpg\n");
            res = 0;
        }
    } else {
        printf("Output path missing extension\n");
        res = 0;
    }

    // Clean up resources
    free(zone);
    stbi_image_free(image);

    if(!res)
    {
        printf("Error writing image\n");
        return -1;
    }
    
    return 0;
}