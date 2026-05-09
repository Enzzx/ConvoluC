#ifndef UTILS_H
#define UTILS_H

#include "types.h"

void swapImgRef(ImgH* handler, unsigned char* newData);
void setFilter(MatrixH* handler);
void sobelM(int** matrix);
void laplaceM(int** matrix);
void embossM(int** matrix);

#endif