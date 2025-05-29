#include "functions.h"

/**
 * @brief Computes bicubic interpolation weight using Catmull-Rom spline
 * @param x Distance from the interpolation point
 * @return Weight value for the given distance
 * 
 * @details Implements the Catmull-Rom spline weighting function:
 * - Piecewise cubic polynomial with a = -0.5 (standard Catmull-Rom parameter)
 * - Smooth interpolation between points
 * - Zero weight beyond 2 units distance
 */
static double bicubic_weight(double x) {
    double a = -0.5; // Catmull-Rom spline parameter
    double abs_x = fabs(x);
    double res = 0.0;
    
    // First segment (0 <= |x| <= 1)
    if (abs_x <= 1.0) 
    {
        res = (a + 2.0) * abs_x * abs_x * abs_x - (a + 3.0) * abs_x * abs_x + 1.0;
    } 
    // Second segment (1 < |x| < 2)
    else if (abs_x < 2.0)
    {
        res =  a * abs_x * abs_x * abs_x - 5.0 * a * abs_x * abs_x + 8.0 * a * abs_x - 4.0 * a;
    }
    return res;
}

/**
 * @brief Performs bicubic interpolation for a single channel
 * @param src Source image data
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param x X-coordinate to interpolate (floating point)
 * @param y Y-coordinate to interpolate (floating point)
 * @param channel Color channel to interpolate (0-R, 1-G, 2-B, etc.)
 * @param channels Total number of color channels
 * @return Interpolated pixel value
 * 
 * @details Uses 4x4 neighborhood around the target point:
 * 1. Calculates weights for each neighbor using bicubic_weight()
 * 2. Clamps coordinates to image boundaries
 * 3. Computes weighted sum of pixel values
 * 4. Normalizes by total weight
 */
static double bicubic_interpolate(unsigned char* src, int width, int height, double x, double y, int channel, int channels) 
{   
    // Determine the 4x4 neighborhood
    int x0 = (int) floor(x) - 1;
    int y0 = (int) floor(y) - 1;
    double dx = x - floor(x);
    double dy = y - floor(y);

    double value = 0.0;
    double sum_weights = 0.0;

    // Iterate over 4x4 neighborhood
    for (int j = -1; j <= 2; j++)
    {
        for (int i = -1; i <= 2; i++) 
        {
            // Calculate neighbor coordinates
            int xi = x0 + i;
            int yj = y0 + j;

             // Clamp to image boundaries
            if(xi < 0)
            {
                xi = 0;
            }
            else if(xi >= width)
            {
                xi = width - 1;
            }
        
            if(yj < 0)
            {
                yj = 0;
            }
            else if (yj >= height)
            {
                yj = height - 1;
            }

            // Calculate weights in x and y directions
            double wx = bicubic_weight(i - dx);
            double wy = bicubic_weight(j - dy);
            double weight = wx * wy;

            // Add weighted contribution
            int coords = (yj * width + xi) * channels + channel;
            value += src[coords] * weight;
            sum_weights += weight;
        }
    }
    
    // Return normalized value (avoid division by zero)
    if(sum_weights != 0)
    {
        return value / sum_weights;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Resizes image using bicubic interpolation
 * @param input_path Path to input image
 * @param output_path Path to save resized image
 * @param scale_x Horizontal scaling factor
 * @param scale_y Vertical scaling factor
 * @return 0 on success, -1 on error
 * 
 * @details Performs high-quality image resizing:
 * 1. Validates scaling factors
 * 2. Loads source image
 * 3. Creates destination buffer
 * 4. For each output pixel:
 *    - Calculates corresponding source coordinates
 *    - Performs bicubic interpolation for each channel
 *    - Clamps values to [0,255] range
 *    - Applies proper rounding
 * 5. Saves result image
 */
int resize_bicubic(char* input_path, char* output_path, double scale_x, double scale_y) {
    // Validate scaling factors
    if(scale_x <= 0 || scale_y <= 0)
    {
        printf("Scaling factors must be positive!\n");
        return -1;
    }

    // Load source image
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return 1;
    }

    // Calculate new dimensions
    int new_width = (int)(width * scale_x);
    int new_height = (int)(height * scale_y);
    unsigned char* dst = (unsigned char*) malloc (sizeof(unsigned char) * new_width * new_height * channels);

    // Process each output pixel
    for (int y = 0; y < new_height; y++) 
    {
        for (int x = 0; x < new_width; x++) 
        {
            // Calculate corresponding source coordinates
            double src_x = x / scale_x;
            double src_y = y / scale_y;

            // Interpolate each channel
            for (int k = 0; k < channels; k++) 
            {
                double interpolated = bicubic_interpolate(image, width, height, src_x, src_y, k, channels);
                // Clamp to valid pixel range
                if(interpolated < 0)
                {
                    interpolated = 0;
                }
                else if(interpolated > 255)
                {
                    interpolated = 255;
                }
                // Round to nearest integer
                dst[(y * new_width + x) * channels + k] = (unsigned char)(interpolated + 0.5);
            }
        }
    }

    // Save result image
    int res;
    if (strstr(output_path, ".png")) 
    {
        res = stbi_write_png(output_path, new_width, new_height, channels, dst, new_width * channels);
    } 
    else if (strstr(output_path, ".jpg")) 
    {
        res = stbi_write_jpg(output_path, new_width, new_height, channels, dst, 100);
    } 
    else 
    {
        printf("Unsupported format. Use .png or .jpg\n");
        stbi_image_free(image);
        free(dst);
        return -1;
    }

    // Clean up
    stbi_image_free(image);
    free(dst);

    if (!res) 
    {
        printf("Error saving image\n");
        return -1;
    }

    return 0;
}