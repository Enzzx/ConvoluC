#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "types.h"

void paddImage(ImgH *H, int mSize);
unsigned char* convoluteImg(ImgH* img, MatrixH* kernel);

#endif