/**
 * @file rotation.c
 * @brief Implementation of image rotation function
 */

#include "functions.h"

/**
 * @brief Rotates an image by specified angle around its center
 * @param input_path Path to input image file
 * @param output_path Path to save rotated image
 * @param angle_degrees Rotation angle in degrees (positive = counter-clockwise)
 * @return 0 on success, -1 on error
 * 
 * @details Performs rigid rotation using nearest-neighbor interpolation:
 * 1. Calculates rotation center (image center)
 * 2. Converts angle to radians
 * 3. Creates blank output image
 * 4. For each pixel, computes new position via rotation matrix
 * 5. Copies pixels using nearest-neighbor approach
 * 
 * @note Uses simple nearest-neighbor interpolation (may cause aliasing)
 */
int rotate_image(char* input_path, char* output_path, double angle_degrees) 
{
    // Load input image using stb_image
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }

    // Calculate image center coordinates (rotation pivot point)
    double center_x = width / 2.0;
    double center_y = height / 2.0;

    // Convert angle from degrees to radians and precompute trig values
    double angle_rad = angle_degrees * PI / 180.0;
    double cos_angle = cos(angle_rad);
    double sin_angle = sin(angle_rad);

    // Allocate and initialize output image buffer (filled with zeros/black)
    unsigned char* rotated_image = (unsigned char*)malloc(width * height * channels);
    memset(rotated_image, 0, width * height * channels);

    // Perform rotation using inverse mapping (destination-to-source)
    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            // Convert to coordinates relative to center
            double dx = x - center_x;
            double dy = y - center_y;

            // Apply rotation matrix:
            // [x'] = [cosθ -sinθ][x]
            // [y']   [sinθ  cosθ][y]
            int new_x = (int)(dx * cos_angle - dy * sin_angle + center_x);
            int new_y = (int)(dx * sin_angle + dy * cos_angle + center_y);

            // Verify new coordinates are within image bounds
            if (new_x >= 0 && new_x < width && new_y >= 0 && new_y < height) 
            {
                // Copy all channels using nearest-neighbor interpolation
                for (int k = 0; k < channels; k++) 
                {
                    int new_pos = (new_y * width + new_x) * channels + k;
                    int orig_pos = (y * width + x) * channels + k;
                    rotated_image[new_pos] = image[orig_pos];
                }
            }
            // Else: leaves pixel as black (from memset initialization)
        }
    }

    // Save rotated image in specified format
    int res;
    if (strstr(output_path, ".png")) 
    {
        res = stbi_write_png(output_path, width, height, channels, rotated_image, width * channels);
    } 
    else if (strstr(output_path, ".jpg")) 
    {
        res = stbi_write_jpg(output_path, width, height, channels, rotated_image, 100);
    } 
    else 
    {
        printf("Unsupported format. Use .png or .jpg\n");
        stbi_image_free(image);
        free(rotated_image);
        return -1;
    }

    // Clean up allocated memory
    stbi_image_free(image);
    free(rotated_image);

    if (!res) 
    {
        printf("Error saving image\n");
        return -1;
    }

    return 0;
}