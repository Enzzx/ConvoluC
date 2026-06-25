#ifndef TYPES_H
#define TYPES_H

#define FIXED_KERNEL_SIZE 3

typedef enum {
	// - - - only in-pixel change filters - - - 
	ColorShift, // red and blue edge effect
	NegativeColor, // invert the image colors
	Greyscale, // put all colors on monocromatic grey scale grade

	// - - - fixed size filters - - - 
	SobelEdge, // sobel edge detection
	LaplacianEdge, // laplacian edge detection
	Emboss, // emboss image
	Identity, // self value (no change)

	// - - - free size filters - - - 
	Blur, // gaussian distribution for blur
	Uniform, // uniform distribution
	MotionBlur, // blur with motion effect
	Sharpen, // sharp image
} FilterType;

typedef struct {
    int w; // width
    int h; // height
    int c; // channel
    int pS; // paddingSize
	int kt; // keepTrack
    unsigned char* data; // image buffer
} ImgH;

typedef struct {
    float* M; // matrix of convolution (kernel)
    int size; // matrix order
    FilterType filter; // filter enum
} MatrixH;

#endif