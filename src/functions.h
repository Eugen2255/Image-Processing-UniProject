#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../libs/stb_image.h"
#include "../libs/stb_image_write.h"

int median_filter(unsigned char* input_image, char* output_path, int height, int width, int channels, int size);


int get_cord(int cord, int max_len);

#endif