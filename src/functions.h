/**
 * @file functions.h
 * @brief Header file for image processing functions
 * @details Contains declarations for various image operations: 
 *          median filtering, Gaussian blur, rotation, resizing, etc.
 * @author Your Name
 * @date Creation Date
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../libs/stb_image.h"
#include "../libs/stb_image_write.h"

#define PI 3.1415926535 ///< Pi constant for mathematical calculations

/**
 * @brief Applies median filter to an image
 * @param input_path Path to the input image file
 * @param output_path Path to save the processed image
 * @param size Filter window size (must be odd)
 * @return 0 on success, -1 on error
 */
int median_filter(char* input_path, char* output_path, int size);

/**
 * @brief Clamps a coordinate to stay within image boundaries
 * @param coord Input coordinate
 * @param max_len Maximum length along the corresponding axis
 * @return Valid coordinate within [0, max_len-1] range
 */
int get_cord(int coord, int max_len);

/**
 * @brief Applies Gaussian blur to an image
 * @param input_path Path to the input image file
 * @param output_path Path to save the processed image
 * @param size Kernel size (must be odd)
 * @param sigma Standard deviation for Gaussian kernel
 * @return 0 on success, -1 on error
 */
int gaussian_blur(char* input_path, char* output_path, int size, double sigma);

/**
 * @brief Performs image convolution with a specified matrix
 * @param input_path Path to the input image file
 * @param output_path Path to save the processed image
 * @param mode Operation mode:
 *             0 - edge detection (Sobel operator),
 *             1 - sharpening,
 * @return 0 on success, -1 on error
 */
int matrix_convolution(char* input_path, char* output_path, int mode);

/**
 * @brief Converts a color image to grayscale
 * @param image Pointer to image data
 * @param height Image height in pixels
 * @param width Image width in pixels
 * @param channels Number of color channels
 * @return Pointer to new grayscale image data
 */
unsigned char* gradation_gray(unsigned char* image, int height, int width, int channels);

/**
 * @brief Applies grayscale filter to an image
 * @param input_path Path to the input image file
 * @param output_path Path to save the processed image
 * @return 0 on success, -1 on error
 */
int gray_filter(char* input_path, char* output_path);

/**
 * @brief Performs histogram equalization on an image
 * @param input_path Path to the input image file
 * @param output_path Path to save the processed image
 * @return 0 on success, -1 on error
 */
int histogram_equ(char* input_path, char* output_path);

/**
 * @brief Rotates an image by specified angle
 * @param input_path Path to the input image file
 * @param output_path Path to save the processed image
 * @param angle_degrees Rotation angle in degrees
 * @return 0 on success, -1 on error
 */
int rotate_image(char* input_path, char* output_path, double angle_degrees);

/**
 * @brief Resizes an image using bicubic interpolation
 * @param input_path Path to the input image file
 * @param output_path Path to save the processed image
 * @param scale_x Horizontal scale factor
 * @param scale_y Vertical scale factor
 * @return 0 on success, -1 on error
 */
int resize_bicubic(char* input_path, char* output_path, double scale_x, double scale_y);

#endif