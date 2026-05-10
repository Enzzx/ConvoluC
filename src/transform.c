#include <stdlib.h>
#include <stdio.h>
#include "../include/transform.h"
#include "../include/utils.h"
#include "../include/types.h"


void paddImage(ImgH *H, int mSize) {
    H->pS = (mSize - 1) / 2;
    if (H->pS > 10000) {
        printf("Quer explodir a CPU paezao?!!");
        return;
    }

    int newBuffer = (((H->w) + 2 * H->pS) * ((H->h) + 2 * H->pS)) * H->c;
    unsigned char* newData = (unsigned char*)malloc(sizeof(unsigned char) * newBuffer);

    int newW = H->w + 2 * H->pS;
    int newH = H->h + 2 * H->pS;

    for (int i = 0; i < newH; i++) { // percorre cada fileira
        for (int j = 0; j < newW; j++) { // percorre os pixels da fileira
            int pixI = (i * newW + j) * H->c;
            int isPadding = (i < H->pS || i >= H->pS + H->h) || (j < H->pS || j >= H->pS + H->w);

            for (int k = 0; k < H->c; k++) { // percorre cada canal de cor
                newData[pixI + k] = isPadding ? 0 : H->data[((i - H->pS) * H->w + (j - H->pS)) * H->c + k];
            }
        }
    }

    H->w += 2 * H->pS;
    H->h += 2 * H->pS;

    swapImgRef(H, newData);
}

void defineMatrix(MatrixH* handler) {

    switch (handler->filter) {
        case SobelEdge:
            handler->M = sobelM();
            break;
        case LaplacianEdge:
            handler->M = laplaceM();
            break;
        case Emboss:
            handler->M = embossM();
            break;
        case Identity:
            handler->M = 1;
            break;

        case Blur:
            handler->M = blurM(handler->size);
            break;
        case Uniform:
            handler->M = uniformM(handler->size);
            break;
        case ColorShift:
            break;
        default:
            break;
    }

}

unsigned char applicateKernel(unsigned char* data, float** kernel , int point, int width, int size, int chN) {
    int newVal = 0;
    float half = (size + 1) / 2;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int diff = i+1 - half;
            int shift = diff * width * chN;
            newVal += data[point + shift + (j * chN)] * kernel[i][j];
        }
    }

    return newVal;
}

void convoluteImg(ImgH* img, MatrixH* kernel) {
    if (kernel->filter <= Identity) {

    }
    else {
        unsigned char* newMatrix = malloc(sizeof(img->data));

        for (int i = img->pS; i < img->h - img->pS; i++) {
            for (int j = img->pS; j < (img->w - img->pS) * img->c; j += img->c) {
                int pixI = i * img->w * img->c + j;

                for (int k = 0; k < img->c; k++) {

                    unsigned char newVal = applicateKernel(img->data, kernel->M, pixI + k, img->w, kernel->size, img->c);
                    //printf("kernel no pixel [%d][%d]: %d\n", i, j, newVal);
                    newMatrix[pixI + k] = newVal;
                }
            }
        }

        swapImgRef(&img, newMatrix);
    }
}