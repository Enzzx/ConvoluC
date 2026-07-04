# ConvoluC
This project is a simple CLI tool developed in C using stb_image "lib" to manipulate png images for filter application.
It works applicating a matrix (kernel) to each pixel of a image buffer, which is the process of convolution, it also use omp for parallelise the convolution process.

## Filters
A filter is simply a some order pre-defined matrix who is applied to a pixel and its neighbors, this matrix is also called as kernel on image processing. The result on the image dependes of the properties of the kernel (his order, values, etc), and the convolution time, just like the result, is highly variable.

There are 11 filters implemented so far, they are:
- ColorShift
- NegativeColor
- GreyScale
- SobelEdge
- LaplacianEdge
- Emboss
- Identity
- Blur
- Uniform
- MotionBlur
- Sharpen

## How to run
`make` to make

`make run` to run

`make clean` to clean

shoud works fine to both windows and linux.