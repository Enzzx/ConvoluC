#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "types.h"

void paddImage(ImgH *H, int mSize);
void convoluteImg(ImgH* img, MatrixH* kernel);
int convoluteRow(void* args);
unsigned char applicateKernel(ImgH* ImgH, MatrixH* MatrixH, int point);

#endif