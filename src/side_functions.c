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