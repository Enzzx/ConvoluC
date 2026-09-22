#ifndef UTILS_H
#define UTILS_H

#include "types.h"

void freeFrame(unsigned char* frameBuffer);
void defineMatrix(ImgH* imgHandler, MatrixH* handler);
void configMatrix(ImgH* imgHandler, MatrixH* handler);
void normalize(float* matrix, int sizeX, int sizeY, int sizeC, float divisor);
float gaussianFunc(int x, int y, int sigma, int weight);
float stdDeviation(float data[], int n);
float* newQuadMatrix(int size);
float* sobelM();
float* laplaceM();
float* embossM();
float* blurM(int size);
float* uniformM(int size);
float* motionBlurM(int size);

#endif
