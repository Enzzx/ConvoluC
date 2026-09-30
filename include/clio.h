#ifndef CLIO_H
#define CLIO_H

#include "types.h"

void swapImgRef(ImgH* handler, unsigned char* newData, int posterior);
void configMatrix(ImgH* imgHandler, MatrixH* handler);

#endif
