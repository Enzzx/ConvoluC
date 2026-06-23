#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include "../include/stb_image.h"
#include "../include/transform.h"
#include "../include/utils.h"
#include "../include/types.h"

#define maxChannels 4

void paddImage(ImgH *H, int mSize) {
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

    swapImgRef(H, newData, 0);
}


void convoluteImg(ImgH* img, MatrixH* kernel) {
    float maxVal = 0;
    unsigned char* newMatrix = (unsigned char*)malloc(sizeof(unsigned char) * img->w * img->h * img->c);

    int fullWlen = img->w * img->c;
    int i;

    #pragma omp parallel for schedule(static) private(i) reduction(max: maxVal)
    for (i = img->pS; i < img->h - img->pS; i++) {
        for (int j = img->pS * img->c; j < (img->w - img->pS) * img->c; j += img->c) {
            int pixI = i * fullWlen + j;

            float pMaxVal= applicateKernelP(img, kernel, pixI, &newMatrix[pixI]);
            if (pMaxVal > maxVal) maxVal = pMaxVal;
        }
    }

    /*if (kernel->filter == LaplacianEdge) {
        // tá quebrando por algum motivo
        normalize(newMatrix, img->w, img->h, img->c, maxVal);
    }*/

    swapImgRef(img, newMatrix, 0);
}


static inline float applicateKernelP(ImgH* i, MatrixH* k, int p, unsigned char* nM) {
    switch (k->filter) {
        case SobelEdge:     return appSobel(i, k, p, nM);
        case LaplacianEdge: return appLaplace(i, k, p, nM);
        case Emboss:        return appEmboss(i, k, p, nM);
        case ColorShift:    return appColorShift(i, k, p, nM);
        default:            return appDefault(i, k, p, nM);
    }
}

static inline float appSobel(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel) {
    float newVal[maxChannels] = { 0 };
    float newValT[maxChannels] = { 0 };
    float maxVal = 0;

    int size = MatrixH->size;
    int width = ImgH->w;
    int channels = ImgH->c;
    int half = (size + 1) / 2;

    for (int i = 0; i < size; i++) {
        int offsetY = (i + 1 - half) * width * channels;
        int lineHeight = i * size;

        for (int j = 0; j < size; j++) {
            int offsetX = (j + 1 - half) * channels;
            int pixBase = point + offsetY + offsetX;

            float mVal = MatrixH->M[lineHeight + j];
            float mValT = MatrixH->M[j * size + i];
            for (int k = 0; k < channels; k++) {
                newVal[k] += ImgH->data[pixBase + k] * mVal;
                newValT[k] += ImgH->data[pixBase + k] * mValT;
            }

        }
    }

    for (int k = 0; k < ImgH->c; k++) {
        float kVal = sqrtf(newVal[k] * newVal[k] + newValT[k] * newValT[k]);
        imgPixel[k] = kVal;

        if (kVal > maxVal)
            maxVal = kVal;
    }

    return maxVal;
}

static inline float appLaplace(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel) {
    float newVal[maxChannels] = { 0 };
    float maxVal = 0;

    int size = MatrixH->size;
    int width = ImgH->w;
    int channels = ImgH->c;
    int half = (size + 1) / 2;

    for (int i = 0; i < size; i++) {
        int offsetY = (i + 1 - half) * width * channels;
        int lineHeight = i * size;

        for (int j = 0; j < size; j++) {
            int offsetX = (j + 1 - half) * channels;
            int pixBase = point + offsetY + offsetX;

            float mVal = MatrixH->M[lineHeight + j];
            for (int k = 0; k < channels; k++) {
                newVal[k] += ImgH->data[pixBase + k] * mVal;
            }
        }
    }

    for (int k = 0; k < channels; k++) {
        float kVal = newVal[k] > 40 ? 255 : 0;
        imgPixel[k] = kVal;

        if (kVal > maxVal)
            maxVal = kVal;
    }

    return maxVal;
}

static inline float appEmboss(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel) {
    float newVal[maxChannels] = { 0 };
    float maxVal = 0;

    int size = MatrixH->size;
    int width = ImgH->w;
    int channels = ImgH->c;
    int half = (size + 1) / 2;

    for (int i = 0; i < size; i++) {
        int offsetY = (i + 1 - half) * width * channels;
        int lineHeight = i * size;

        for (int j = 0; j < size; j++) {
            int offsetX = (j + 1 - half) * channels;
            int pixBase = point + offsetY + offsetX;

            float mVal = MatrixH->M[lineHeight + j];
            for (int k = 0; k < channels; k++) {
                newVal[k] += ImgH->data[pixBase + k] * mVal;
            }
        }
    }

    for (int k = 0; k < channels; k++) {
        float kVal = newVal[k] + 128;
        if (kVal < 0) kVal = 0;
        else if (kVal > 255) kVal = 255;

        imgPixel[k] = kVal;

        if (kVal > maxVal)
            maxVal = kVal;
    }

    return maxVal;
}

static inline float appColorShift(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel) {
    float newVal[maxChannels] = { 0 };
    float maxVal = 0;

    int size = MatrixH->size;
    int width = ImgH->w;
    int channels = ImgH->c;
    int half = (size + 1) / 2;

    for (int i = 0; i < size; i++) {
        int offsetY = (i + 1 - half) * width * channels;
        int lineHeight = i * size;

        for (int j = 0; j < size; j++) {
            int offsetX = (j + 1 - half) * channels;
            int pixBase = point + offsetY + offsetX;

            float mVal = MatrixH->M[lineHeight + j];
            for (int k = 0; k < channels; k++) {
                newVal[k] += ImgH->data[pixBase + k] * mVal;
            }
        }
    }

    for (int k = 0; k < channels; k++) {
        float kVal = newVal[k] + 128;
        if (kVal < 0) kVal = 0;
        else if (kVal > 255) kVal = 255;

        imgPixel[k] = kVal;

        if (kVal > maxVal)
            maxVal = kVal;
    }

    return maxVal;
}

static inline float appDefault(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel) {
    float newVal[maxChannels] = { 0 };
    float maxVal = 0;

    int size = MatrixH->size;
    int width = ImgH->w;
    int channels = ImgH->c;
    int half = (size + 1) / 2;

    for (int i = 1; i < size-1; i++) {
        int offsetY = (i + 1 - half) * width * channels;
        int lineHeight = i * size;

        for (int j = 0; j < size; j++) {
            int offsetX = (j + 1 - half) * channels;
            int pixBase = point + offsetY + offsetX;

            float mVal = MatrixH->M[lineHeight + j];
            for (int k = 0; k < channels; k++) {
                newVal[k] += ImgH->data[pixBase + k] * mVal;
            }
        }
    }

    for (int k = 0; k < channels; k++) {
        float kVal = newVal[k];
        imgPixel[k] = kVal;

        if (kVal > maxVal)
            maxVal = kVal;
    }

    return maxVal;
}