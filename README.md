# Image-Processing-UniProject
Project for imperative programming course.
Handles command-line interface for various image processing functions. Supported operations:

  - _Median filter_ (kernel size)
  
  - _Image rotation_ (angle)
  
  - _Gaussian blur_ (kernel size, sigma)
  
  - _Bicubic resizing_ (scale_x, scale_y)
  
  - _Edge detection_
  
  - _Sharpening_
  
  - _Grayscale conversion_
  
  - _Histogram equalization_
  
In brackets - parameters, except input and output paths.

## Builing
Use programms from scripts folder **.bat** for windows and **.sh** for linux 

**imgproc.exe** will be created.

## Command syntax
### For Windows:

Basic: ``` ./imgproc input_path mode output_path ```

With 1 parameter: ``` ./imgproc input_path mode value output_path ```

With 2 parameters: ``` ./imgproc input_path mode value1 value2 output_path ```



## Testing
After compiling you can use scripts/tests.bat it will show products of filters.
