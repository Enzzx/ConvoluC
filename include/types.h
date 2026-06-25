#ifndef TYPES_H
#define TYPES_H

#define FIXED_KERNEL_SIZE 3

typedef enum {
	// - - - only in-pixel change filters - - - 
	ColorShift, // red and blue edge effect


	// - - - fixed size filters - - - 
	SobelEdge, // sobel edge detection
	LaplacianEdge, // laplacian edge detection
	Emboss, // emboss image
	Identity, // self value (no change)

	// - - - free size filters - - - 
	Blur, // gaussian distribution for blur
	Uniform, // uniform distribution
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
	int mag; // magnitude (of any filter who needs an value)
    float* M; // matrix of convolution (kernel)
    int size; // matrix order
    FilterType filter; // filter enum
} MatrixH;

#endif