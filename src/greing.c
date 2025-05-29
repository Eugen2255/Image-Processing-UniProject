#include "functions.h"

int grey_filter(char* input_path, char* output_path)
{
    // reading image
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return -1;
    }
    
    // apply filter for white & black picture
    image = gradation_gray(image, height, width, channels);
    if(image == NULL)
    {   
        stbi_image_free(image);
        return -1;
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
        return -1;
    }

    if(!res)
    {
        stbi_image_free(image);
        printf("Error writing image\n");
        return -1;
    }
    
    // freeing image
    stbi_image_free(image);
    return 0;
}