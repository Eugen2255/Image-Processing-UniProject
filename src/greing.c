/**
 * @file greing.c
 * @brief Implementation of grey gradation filter
 */

#include "functions.h"

/**
 * @brief Converts a color image to grayscale
 * @param input_path Path to the input image file
 * @param output_path Path to save the grayscale image
 * @return 0 on success, -1 on error
 */
int gray_filter(char* input_path, char* output_path)
{
    // Load image
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }
    
    // Convert image to grayscale using helper function
    // gradation_gray() handles the actual conversion algorithm
    image = gradation_gray(image, height, width, channels);
    if(!image)
    {   
        // Free resources before returning error
        stbi_image_free(image);
        return -1;
    }
    
    // Save processed image in specified format
    int res;
    if (strstr(output_path, ".png")) 
    {
        // Save as PNG format
        // Parameters: path, dimensions, channels, data, stride
        res = stbi_write_png(output_path, width, height, channels, image, width * channels);
    }
    else if (strstr(output_path, ".jpg"))
    {
        // Save as JPG format with maximum quality (100)
        res = stbi_write_jpg(output_path, width, height, channels, image, 100);
    } 
    else 
    {
        // Unsupported format - only PNG/JPG allowed
        printf("Unsupported format. Use .png or .jpg\n");
        stbi_image_free(image);
        return -1;
    }

    // Check if image was saved successfully
    if(!res)
    {
        stbi_image_free(image);
        printf("Error writing image\n");
        return -1;
    }
    
    // Free image resources
    stbi_image_free(image);
    return 0;
}