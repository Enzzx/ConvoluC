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
    ImgH* img;                 // Ponteiro para a imagem original
    MatrixH* kernel;           // Ponteiro para o kernel do filtro
    int startRow;              // Linha inicial que ESTA thread vai processar
    int endRow;                // Linha final (limite) que ESTA thread vai processar
    float localMax;            // Valor máximo encontrado apenas por esta thread
    unsigned char* newMatrix;  // Buffer compartilhado da nova imagem
} rowKernelH;


#endif