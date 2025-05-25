#include "functions.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb_image_write.h"

int get_cord(int cord, int max_len)
{
    if(cord < 0)
        return 0;
    else if (cord >= max_len)
        return max_len - 1;
    else 
        return cord;
}

unsigned char* gradation_gray(unsigned char* image, int height, int width, int channels)
{
    unsigned char* temp = (unsigned char*) malloc (height * width * channels * sizeof(unsigned char));
    if (!temp) 
    {
        printf("Error: Memory allocation failed!\n");
        return NULL;
    }
    memcpy(temp, image,  height * width * channels * sizeof(unsigned char));
    
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {   
            int sum = 0;
            for(int k = 0; k < channels; k++)
            {
                sum += temp[(i * width + j) * channels + k];
            }
            // rounding division
            unsigned char gray_value = (unsigned char)((sum + channels/2) / channels);

            for(int k = 0; k < channels; k++)
            {
                image[(i * width + j) * channels + k] = gray_value;
            }
        }
    }
    free(temp);
    return image;
}