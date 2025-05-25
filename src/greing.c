#include "functions.h"

int grey_filter(char* input_path, char* output_path)
{
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }

    image = gradation_gray(image, height, width, channels);

    if(!stbi_write_png(output_path, width, height, channels, image, width * channels))
    {
        printf("Error writing image\n");
        return -1;
    }
    
    stbi_image_free(image);

    return 0;
}