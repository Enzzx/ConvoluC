#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "types.h"

void paddImage(ImgH *H, int mSize);
void convoluteImg(ImgH* img, MatrixH* kernel);
float applicateKernel(ImgH* i, MatrixH* k, int p, unsigned char* nM);
float appSobel(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel);
float appLaplace(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel);
float appEmboss(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel);
float appColorShift(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel);
float appDefault(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel);

#endif