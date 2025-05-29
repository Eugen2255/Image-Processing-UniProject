#include "functions.h"

// function for qsort
static int compare(const void *a, const void *b) 
{
    return (*(unsigned char*)a - *(unsigned char*)b);
}

int median_filter(char* input_path, char* output_path, int size) 
{
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

     // reading image
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }

    if (size > height || size > width) 
    {
        stbi_image_free(image);
        printf("Error: Filter size exceeds image dimensions!\n");
        return -1;
    }
    
    // radius of window
    int radius = size / 2; 
    unsigned char* zone = malloc(size * size * sizeof(unsigned char));
    if (!zone) 
    {
        stbi_image_free(image);
        printf("Error: Memory allocation failed!\n");
        return -1;
    }

    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            for (int k = 0; k < channels; k++) 
            {
                int count = 0;
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

                qsort(zone, count, sizeof(unsigned char), compare);
                // taking median value of all pixels in window
                image[(i * width + j) * channels + k] = zone[count / 2];
            }
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
        printf("Wrong format. Use .png or .jpg\n");
        res = 1;
    }

    // freeing the data
    free(zone);
    stbi_image_free(image);

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