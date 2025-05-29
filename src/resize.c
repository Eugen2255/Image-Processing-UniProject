#include "functions.h"

// Bicubic weight function (Catmull-Rom)
static double bicubic_weight(double x) {
    double a = -0.5; // for Catmull-Rom, change if needed
    double abs_x = fabs(x);
    double res = 0.0;
    if (abs_x <= 1.0) 
    {
        res = (a + 2.0) * abs_x * abs_x * abs_x - (a + 3.0) * abs_x * abs_x + 1.0;
    } 
    else if (abs_x < 2.0)
    {
        res =  a * abs_x * abs_x * abs_x - 5.0 * a * abs_x * abs_x + 8.0 * a * abs_x - 4.0 * a;
    }
    return res;
}

// Bicubic interpolation for a single channel
static double bicubic_interpolate(unsigned char* src, int width, int height, double x, double y, int channel, int channels) 
{
    int x0 = (int) floor(x) - 1;
    int y0 = (int) floor(y) - 1;
    double dx = x - floor(x);
    double dy = y - floor(y);

    double value = 0.0;
    double sum_weights = 0.0;

    for (int j = -1; j <= 2; j++)
    {
        for (int i = -1; i <= 2; i++) 
        {
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

            // calcute the weigths
            double wx = bicubic_weight(i - dx);
            double wy = bicubic_weight(j - dy);
            double weight = wx * wy;

            int coords = (yj * width + xi) * channels + channel;
            value += src[coords] * weight;
            sum_weights += weight;
        }
    }
    if(sum_weights != 0)
    {
        return value / sum_weights;
    }
    else
    {
        return 0;
    }
}

// Resize image using bicubic interpolation
int resize_bicubic(char* input_path, char* output_path, double scale_x, double scale_y) {
    // reading image
    int width, height, channels;
    unsigned char* image = stbi_load(input_path, &width, &height, &channels, 0);
    if (!image) 
    {
        printf("Error loading image\n");
        return 1;
    }

    int new_width = (int)(width * scale_x);
    int new_height = (int)(height * scale_y);
    unsigned char* dst = (unsigned char*) malloc (sizeof(unsigned char) * new_width * new_height * channels);

    for (int y = 0; y < new_height; y++) 
    {
        for (int x = 0; x < new_width; x++) 
        {
            double src_x = x / scale_x;
            double src_y = y / scale_y;

            for (int k = 0; k < channels; k++) 
            {
                double interpolated = bicubic_interpolate(image, width, height, src_x, src_y, k, channels);
                if(interpolated < 0)
                {
                    interpolated = 0;
                }
                else if(interpolated > 255)
                {
                    interpolated = 255;
                }
                dst[(y * new_width + x) * channels + k] = (unsigned char)(interpolated + 0.5); // more accurate rounding
            }
        }
    }

    // saving rotated image
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

    // freeing the memory
    stbi_image_free(image);
    free(dst);

    if (!res) 
    {
        printf("Error saving image\n");
        return -1;
    }

    return 0;
}
