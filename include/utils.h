#ifndef UTILS_H
#define UTILS_H

#include "types.h"

void swapImgRef(ImgH* handler, unsigned char* newData, int posterior);
void setFilter(MatrixH* handler, ImgH* imgHandler);
void normalize(float* matrix, int size, float divisor);
float gaussianFunc(int x, int y, int sigma, int weight);
float* newQuadMatrix(int size);
float* sobelM();
float* laplaceM();
float* embossM();
float* blurM(int size);
float* uniformM(int size);

#endif