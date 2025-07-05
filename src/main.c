/**
 * @file main.c
 * @brief Main program for image processing operations
 * 
 * @details Handles command-line interface for various image processing functions
 * Supported operations:
 * - Median filter
 * - Image rotation
 * - Gaussian blur
 * - Bicubic resizing
 * - Edge detection
 * - Sharpening
 * - Grayscale conversion
 * - Histogram equalization
 */

#include "src/functions.h"

/**
 * @brief Main function for image processing application
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 on success, -1 on error
 * 
 * @details Command syntax:
 * Basic: ./program input_path mode output_path
 * With 1 parameter: ./program input_path mode value output_path
 * With 2 parameters: ./program input_path mode val1 val2 output_path
 */
int main(int argc, char* argv[]) {
    // Validate argument count (3-6 arguments expected)
    if(argc < 3 || argc > 6)
    {
        printf("Invalid command format!\n");
        return -1;
    }
    
    // Extract required arguments
    char* input_path = argv[1];  // Input image file path
    char* mode = argv[2];       // Processing mode flag

    // Result flag (0 indicates success)
    int res = 1;

    // Handle commands with 1 numeric parameter (4 total args)
    if (argc == 5)
    {
        double val = atof(argv[3]);  // Convert parameter to double
        char* output_path = argv[4]; // Output file path

        if (strcmp(mode, "-median") == 0) 
        {
            // Apply median filter with window size = val
            res = median_filter(input_path, output_path, (int)val);
        }
        else if (strcmp(mode, "-rotate") == 0)
        {
            // Rotate image by val degrees
            res = rotate_image(input_path, output_path, val);
        }
        else
        {
            printf("Invalid command!\n");
        }
    }
    // Handle commands with 2 numeric parameters (5 total args)
    else if (argc == 6)
    {
        double val1 = atof(argv[3]); // First parameter
        double val2 = atof(argv[4]); // Second parameter
        char* output_path = argv[5]; // Output file path

        if (strcmp(mode, "-gaus") == 0) 
        {
            // Apply Gaussian blur with kernel size=val1, sigma=val2
            res = gaussian_blur(input_path, output_path, (int)val1, val2);
        }
        else if(strcmp(mode, "-resize") == 0)
        {
            // Resize image with scale factors val1(x), val2(y)
            res = resize_bicubic(input_path, output_path, val1, val2);
        }
        else
        {
            printf("Invalid command!\n");
        }
    }
    // Handle simple commands with no numeric parameters (3 total args)
    else if (argc == 4)
    {
        char* output_path = argv[3]; // Output file path

        if (strcmp(mode, "-edge") == 0)
        {
            // Apply edge detection filter
            res = matrix_convolution(input_path, output_path, 1);
        }
        else if(strcmp(mode, "-sharp") == 0)
        {
            // Apply sharpening filter
            res = matrix_convolution(input_path, output_path, 0);
        }
        else if(strcmp(mode, "-gray") == 0)
        {
            // Convert to grayscale
            res = gray_filter(input_path, output_path);
        }
        else if(strcmp(mode, "-hist") == 0)
        {
            // Apply histogram equalization
            res = histogram_equ(input_path, output_path);
        }
        else
        {
            printf("Invalid command!\n");
        }
    }
    else
    {
        printf("Invalid command format!\n");
    }

    // Output final status message
    if(res == 0)
    {
        printf("Operation completed successfully!\n");
    }
    else
    {
        printf("Operation failed!\n");
    }
    
    return 0;
}