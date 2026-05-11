#ifndef TYPES_H
#define TYPES_H

typedef enum {
	// - - - fixed size filters - - - 
	SobelEdge, // sobel edge detection
	LaplacianEdge, // laplacian edge detection
	Emboss, // emboss image
	Identity, // self value (no change)

	// - - - free size filters - - - 
	Blur, // gaussian distribution for blur
	Uniform, // uniform distribution
	ColorShift, // red and blue edge effect
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
    float* M;
    int size;
    FilterType filter;
} MatrixH;

#endif