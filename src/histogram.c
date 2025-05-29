/**
 * @file histrogram.c
 * @brief Implementation of histogram equalization
 */
#include "functions.h"

/**
 * @brief Performs histogram equalization on a grayscale image
 * @param input_path Path to the input image file
 * @param output_path Path to save the processed image
 * @return 0 on success, -1 on error, 1 for unsupported format
 * 
 * @details This function:
 *          1. Loads the input image
 *          2. Computes the histogram of pixel intensities
 *          3. Calculates cumulative distribution function (CDF)
 *          4. Applies histogram equalization transform
 *          5. Saves the result image
 * 
 * @note Works on both grayscale and color images (converts color to grayscale)
 * @warning Input should preferably be grayscale for best results
 */
int histogram_equ(char* input_path, char* output_path)
{
    // Load image
    int width, height, channels;
    unsigned char *image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }

    // Initialize histogram array (256 bins for 8-bit image)
    unsigned int histogram[256] = {0};
    
    // Compute histogram by counting pixel intensities
    // For color images, only use the first channel (converting to grayscale)
    for (int i = 0; i < width * height * channels; i += channels) 
    {
        histogram[image[i]]++;
    }

    // Compute cumulative distribution function (CDF)
    unsigned int cdf[256] = {0};
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; i++) 
    {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    // Find minimum non-zero value in CDF for normalization
    unsigned int cdf_min = 0;
    for (int i = 0; i < 256; i++) 
    {
        if (cdf[i] != 0) 
        {
            cdf_min = cdf[i];
            break;
        }
    }

    // Apply histogram equalization transform:
    // new_pixel = round((cdf[pixel] - cdf_min) * 255 / (N - cdf_min))
    // where N = total number of pixels
    for (int i = 0; i < width * height * channels; i += channels) 
    {
        unsigned char pixel = image[i];
        unsigned int new_pixel = (cdf[pixel] - cdf_min) * 255 / (width * height - cdf_min);
        image[i] = (unsigned char)new_pixel;
        
        // For color images, make all channels equal (convert to grayscale)
        if (channels >= 3) 
        {
            image[i + 1] = image[i];  // Green channel
            image[i + 2] = image[i];  // Blue channel
        }
    }

    // Save processed image
    int res;
    if (strstr(output_path, ".png")) 
    {
        // Save as PNG format
        res = stbi_write_png(output_path, width, height, channels, image, width * channels);
    }
    else if (strstr(output_path, ".jpg"))
    {
        // Save as JPG with maximum quality
        res = stbi_write_jpg(output_path, width, height, channels, image, 100);
    } 
    else 
    {
        // Unsupported format error
        printf("Unsupported format. Use .png or .jpg\n");
        stbi_image_free(image);
        return 1;
    }

    // Check if save operation succeeded
    if(!res)
    {
        stbi_image_free(image);
        printf("Error writing image\n");
        return -1;
    }

    // Free allocated image memory
    stbi_image_free(image);
    return 0;
}