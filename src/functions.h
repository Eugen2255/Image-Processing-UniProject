#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../libs/stb_image.h"
#include "../libs/stb_image_write.h"

int median_filter(char* input_path, char* output_path, int size);

int get_cord(int cord, int max_len);

int gaussian_blur(char* input_path, char* output_path, int size, double sigma);

#endif