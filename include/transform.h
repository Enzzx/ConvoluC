#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "types.h"

void paddImage(ImgH *H, int mSize);
void convoluteImg(ImgH* img, MatrixH* kernel);
unsigned char applicateKernel(ImgH* i, MatrixH* k, int p);
float appSobel(ImgH* ImgH, MatrixH* MatrixH, int point);
float appLaplace(ImgH* ImgH, MatrixH* MatrixH, int point);
float appEmboss(ImgH* ImgH, MatrixH* MatrixH, int point);
float appColorShift(ImgH* ImgH, MatrixH* MatrixH, int point);
float appDefault(ImgH* ImgH, MatrixH* MatrixH, int point);

#endif