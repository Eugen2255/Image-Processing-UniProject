# Image-Processing-UniProject
Project for imperative programming course.
Handles command-line interface for various image processing functions. Supported operations:

  - _Median filter_ (kernel size) ```-median```
  
  - _Image rotation_ (angle) ```-rotate```
  
  - _Gaussian blur_ (kernel size, sigma) ```-gauss```
  
  - _Bicubic resizing_ (scale_x, scale_y) ```-resize```
  
  - _Edge detection_ ```-edge```
  
  - _Sharpening_  ```-sharp```
  
  - _Grayscale conversion_ ```-gray```
  
  - _Histogram equalization_ ```-hist```
  
In brackets - parameters, except input and output paths.

## Building
Use programms from scripts folder **.bat** for windows and **.sh** for linux,

or ```gcc -o imgproc.exe main.c src/median_filter.c src/side_functions.c src/gaussian_blur.c src/convolution.c src/greing.c src/histogram.c src/rotation.c src/resize.c```

**imgproc.exe** will be created.

## Command syntax
### For Windows:

Basic: ``` ./imgproc input_path mode output_path ```

With 1 parameter: ``` ./imgproc input_path mode value output_path ```

With 2 parameters: ``` ./imgproc input_path mode value1 value2 output_path ```



## Testing
After compiling you can use scripts/tests.bat it will show products of filters.

## Examples
### **Histogram equalization** and **Grayscale conversion**
![b w](https://github.com/user-attachments/assets/5778e08c-e1a0-412f-bbed-6f2fb6daf164)

### Using edge detection after **Histogram equalization**
![edge_det](https://github.com/user-attachments/assets/346b4298-66b1-4589-acd1-032ea8698be4)

### **Blurs**
![Blurs](https://github.com/user-attachments/assets/6b788cd6-03d3-485d-bd06-58cc1aa82497)

### **Resize**
![resize](https://github.com/user-attachments/assets/624e8f0f-754e-4d65-b4cf-842611d3ff05)

