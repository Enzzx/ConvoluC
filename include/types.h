#ifndef TYPES_H
#define TYPES_H

#define FIXED_KERNEL_SIZE 3

typedef enum {
	// - - - fixed size filters - - - 
	SobelEdge, // sobel edge detection
	LaplacianEdge, // laplacian edge detection
	Emboss, // emboss image
	ColorShift, // red and blue edge effect
	Identity, // self value (no change)

	// - - - free size filters - - - 
	Blur, // gaussian distribution for blur
	Uniform, // uniform distribution
} FilterType;

typedef struct {
    unsigned char* data;
    int w; // width
    int h; // height
    int c; // channel
    int pS; // paddingSize
	int kt; // keepTrack
} ImgH;

typedef struct {
    float* M; // convolution matrix
    int size; // matrix order
    FilterType filter; // filter type
} MatrixH;

typedef struct {
    ImgH* img; // img handler
    MatrixH* kernel; // kernel handler
    int i; // row index
    float* maxVal; // max value of convoluted pixels
    unsigned char* newMatrix; // new img buffer
} rowKernelH;

#endif