#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "types.h"

void paddImage(ImgH *H, int mSize);
void defineMatrix(MatrixH* handler);
unsigned char applicateKernel(unsigned char* data, float* kernel, int point, int width, int size, int chN);
void convoluteImg(ImgH* img, MatrixH* kernel);

#endif