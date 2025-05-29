#include "functions.h"

int rotate_image(char* input_path, char* output_path, double angle_degrees) 
{
    // reading image
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }

    // calcute the centre of image
    double center_x = width / 2.0;
    double center_y = height / 2.0;

    // Convert the angle to radians
    double angle_rad = angle_degrees * PI / 180.0;
    double cos_angle = cos(angle_rad);
    double sin_angle = sin(angle_rad);

    // Create a temporary buffer for the rotated image (initialize with zeros(black))
    unsigned char* rotated_image = (unsigned char*) malloc (width * height * channels);
    memset(rotated_image, 0, width * height * channels);

    // Go through each pixel of the original image
    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            // Shift coordinates relative to the center
            double dx = x - center_x;
            double dy = y - center_y;

            // applying rotation matrix
            int new_x = (int)(dx * cos_angle - dy * sin_angle + center_x);
            int new_y = (int)(dx * sin_angle + dy * cos_angle + center_y);

            // Check if the new coordinates stay within boundaries
            if (new_x >= 0 && new_x < width && new_y >= 0 && new_y < height) 
            {
                // copy pixels
                for (int k = 0; k < channels; k++) 
                {
                    int new_cords = (new_y * width + new_x) * channels + k;
                    int old_cords = (y * width + x) * channels + k;
                    rotated_image[new_cords] = image[old_cords];
                }
            }
        }
    }

    // saving rotated image
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

    // freeing the memory
    stbi_image_free(image);
    free(rotated_image);

    if (!res) 
    {
        printf("Error saving image\n");
        return -1;
    }

    return 0;
}