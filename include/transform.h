#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "types.h"

void paddImage(ImgH *H, int mSize);
void convoluteImg(ImgH* img, MatrixH* kernel);
float applicateKernelP(ImgH* i, MatrixH* k, int p, unsigned char* nM);
float appColorShift(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel);
float appSobel(ImgH* ImgH, int point, unsigned char* imgPixel);
float appLaplace(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel);
float appEmboss(ImgH* ImgH, int point, unsigned char* imgPixel);
float appMotionBlur(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel);
float appSharpen(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel);
float appDefault(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel);

#endif