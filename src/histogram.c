#include "functions.h"

// function for histogram equalization of grayscale image
int histogram_equ(char* input_path, char* output_path)
{
    // reading image
    int width, height, channels;
    unsigned char *image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }

    // creating histogram
    unsigned int histogram[256] = {0};
    for (int i = 0; i < width * height * channels; i += channels) 
    {
        histogram[image[i]]++;
    }

    // calculate cumulative histogram
    unsigned int cdf[256] = {0};
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; i++) 
    {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    // finding the minimum non zero number in CDF
    unsigned int cdf_min = 0;
    for (int i = 0; i < 256; i++) 
    {
        if (cdf[i] != 0) 
        {
            cdf_min = cdf[i];
            break;
        }
    }

    // Applying histogram conversion
    for (int i = 0; i < width * height * channels; i += channels) 
    {
        unsigned char pixel = image[i];
        unsigned int new_pixel = (cdf[pixel] - cdf_min) * 255 / (width * height - cdf_min);
        image[i] = (unsigned char)new_pixel;
        
        // If RGB, convert all channels in the same way (make grayscale)
        if (channels >= 3) 
        {
            image[i + 1] = image[i];
            image[i + 2] = image[i];
        }
    }

    // saving image
    int res;
    if (strstr(output_path, ".png")) 
    {
        res = stbi_write_png(output_path, width, height, channels, image, width * channels);
    }
    else if (strstr(output_path, ".jpg"))
    {
        res = stbi_write_jpg(output_path, width, height, channels, image, 100);
    } 
    else 
    {
        printf("Unsupported format. Use .png or .jpg\n");
        stbi_image_free(image);
        return 1;
    }

    if(!res)
    {
        stbi_image_free(image);
        printf("Error writing image\n");
        return -1;
    }

    // freeing memory
    stbi_image_free(image);
    return 0;

}