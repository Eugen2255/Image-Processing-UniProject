#include "src/functions.h"

int main(int argc, char* argv[]) {
    if(argc < 3 || argc > 6)
    {
        printf("Not valid command!\n");
        return -1;
    }
    
    char* input_path = argv[1];
    char* mode = argv[2];


    // флаг результата, если 0 - все выполнилось
    int res = 1;
    if (argc == 5)
    {
        double val = atof(argv[3]);
        char* output_path = argv[4];
        if (strcmp(mode, "-median") == 0) 
        {
            res = median_filter(input_path, output_path, val);
        }
        else if (strcmp(mode, "-rotate") == 0)
        {
            res = rotate_image(input_path, output_path, val);
        }
        else
        {
            printf("In progress\n");
        }
    }
    else if (argc == 6)
    {
        double val1 = atof(argv[3]);
        double val2 = atof(argv[4]);
        
        char* output_path = argv[5];
        if (strcmp(mode, "-gaus") == 0) 
        {
            res = gaussian_blur(input_path, output_path, val1, val2);
        }
        else if(strcmp(mode, "-resize") == 0)
        {
            res = resize_bicubic(input_path, output_path, val1, val2);
        }
        else
        {
            printf("In progress\n");
        }
    }
    else if (argc == 4)
    {
        char* output_path = argv[3];
        if (strcmp(mode, "-edge") == 0)
        {
            res = matrix_convolution(input_path, output_path, 1);
        }
        else if(strcmp(mode, "-sharp") == 0)
        {
            res = matrix_convolution(input_path, output_path, 0);
        }
        else if(strcmp(mode, "-grey") == 0)
        {
            res = grey_filter(input_path, output_path);
        }
        else if(strcmp(mode, "-hist") == 0)
        {
            res = histogram_equ(input_path, output_path);
        }
        else
        {
            printf("In progress\n");
        }
    }
    else
    {
        printf("Unknow command!\n");
    }

    if(res == 0)
    {
        printf("All done!\n");
    }
    else
    {
        printf("Something went wrong!\n");
    }
    
    return 0;
}