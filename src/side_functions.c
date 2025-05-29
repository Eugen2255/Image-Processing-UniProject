/**
 * @file side_functions.c
 * @brief Implementation of repeating functions
 */
#include "functions.h"

// Required for stb_image implementation
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
// Required for stb_image_write implementation
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb_image_write.h"

/**
 * @brief Handles coordinate clamping for image boundaries
 * @param cord Input coordinate (may be out of bounds)
 * @param max_len Maximum valid coordinate (image dimension)
 * @return Clamped coordinate within [0, max_len-1] range
 * 
 * @details Uses mirroring approach for coordinates outside bounds:
 * - Negative coordinates are mirrored positively
 * - Coordinates beyond max_len are mirrored back into range
 * 
 * @note Useful for convolution operations at image edges
 */
int get_cord(int cord, int max_len)
{
    if(cord < 0)
        return (-cord);  // Mirror negative coordinates
    else if (cord >= max_len)
        return 2 * max_len - cord - 2;  // Mirror overflow coordinates
    else 
        return cord;  // Return unchanged if within bounds
}

/**
 * @brief Converts color image to grayscale
 * @param image Pointer to image data (RGB or RGBA)
 * @param height Image height in pixels
 * @param width Image width in pixels
 * @param channels Number of color channels (3 for RGB, 4 for RGBA)
 * @return Pointer to grayscale image (same buffer)
 * 
 * @details Performs conversion by:
 * 1. Creating temporary copy of original image
 * 2. Calculating luminance via channel averaging
 * 3. Applying rounded division for proper grayscale values
 * 4. Setting all channels to same grayscale value
 * 
 * @note Uses simple averaging (equal weights for all channels)
 * @warning Allocates temporary memory - caller must free original buffer
 */
unsigned char* gradation_gray(unsigned char* image, int height, int width, int channels)
{   
    // Create temporary copy of original image
    unsigned char* temp = (unsigned char*)malloc(height * width * channels);
    if (!temp) 
    {
        printf("Error: Memory allocation failed!\n");
        return NULL;
    }
    memcpy(temp, image, height * width * channels);

    // Process each pixel
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {   
            // Calculate sum of all color channels
            int sum = 0;
            for(int k = 0; k < channels; k++)
            {
                sum += temp[(i * width + j) * channels + k];
            }

            // Compute grayscale value with proper rounding
            // Formula: (sum + channels/2) / channels for rounding
            unsigned char gray_value = (unsigned char)((sum + channels/2) / channels);

            // Set all channels to grayscale value
            for(int k = 0; k < channels; k++)
            {
                image[(i * width + j) * channels + k] = gray_value;
            }
        }
    }

    // Free temporary buffer
    free(temp);
    return image;  // Return modified original buffer
}