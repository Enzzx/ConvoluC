#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../include/utils.h"


void swapImgRef(ImgH* handler, unsigned char* newData, int posterior) {
    if (!handler || !newData) {
        printf("Erro ao receber dados para realizar swap de image buffer");
        return;
    }

    handler->data = newData;
}


EXPORT void defineMatrix(ImgH* imgHandler, MatrixH* handler) {

    switch (handler->filter) {
    case ColorShift:
        handler->M = (float*)1;
        break;
    case NegativeColor:
        handler->M = (float*)1;
        handler->size = 1;
        break;
    case Greyscale:
        handler->M = (float*)1;
        handler->size = 1;
        break;

    case SobelEdge:
        handler->M = sobelM();
        handler->size = FIXED_KERNEL_SIZE;
        break;
    case LaplacianEdge:
        handler->M = laplaceM();
        handler->size = FIXED_KERNEL_SIZE;
        break;
    case Emboss:
        handler->M = embossM();
        handler->size = FIXED_KERNEL_SIZE;
        break;
    case Identity:
        handler->M = 0;
        handler->size = 1;
        break;

    case Blur:
        handler->M = blurM(handler->size);
        break;
    case Uniform:
        handler->M = uniformM(handler->size);
        break;
    case MotionBlur:
        handler->M = 0;
        break;
    case Sharpen:
        handler->M = blurM(handler->size);
        break;
    default:
        break;
    }

    imgHandler->pS = (handler->size - 1) / 2;
}


void normalize(float* matrix, int sizeX, int sizeY, int sizeC, float divisor) {
    for (int i = 0; i < sizeY; i++) {
        for (int j = 0; j < sizeX; j++) {
            for (int k = 0; k < sizeC; k++) {
                matrix[(i * sizeX + j) * sizeC + k] /= divisor;

            }
        }
    }
}

float gaussianFunc(int x, int y, int sigma, int weight) {
    double e = exp(1.0);
    double expoent = (pow(x+weight, 2) + pow(y+weight, 2)) / (2 * pow(sigma, 2));
    double result = pow(e, -expoent);

    return result;
}

float* newQuadMatrix(int size) {
    float* matrix = (float*)calloc(size * size, sizeof(float));

    return matrix;
}

float* sobelM() {
    float* mx = newQuadMatrix(FIXED_KERNEL_SIZE);
    float sobelValues[] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };

    for (int i = 0; i < FIXED_KERNEL_SIZE; i++) {
        for (int j = 0; j < FIXED_KERNEL_SIZE; j++) {
            mx[i * FIXED_KERNEL_SIZE + j] = sobelValues[i * FIXED_KERNEL_SIZE + j];
        }
    }

    return mx;
}

float* laplaceM() {
    float* mx = newQuadMatrix(FIXED_KERNEL_SIZE);
    float laplaceValues[] = {
        0, 1, 0,
        1, -4, 1,
        0, 1, 0
    };

    for (int i = 0; i < FIXED_KERNEL_SIZE; i++) {
        for (int j = 0; j < FIXED_KERNEL_SIZE; j++) {
            mx[i * FIXED_KERNEL_SIZE + j] = laplaceValues[i * FIXED_KERNEL_SIZE + j];
        }
    }

    return mx;
}

float* embossM() {
    float* mx = newQuadMatrix(FIXED_KERNEL_SIZE);
    float embossValues[] = {
        -2, 0, 0,
        0, 0, 0,
        0, 0, 2
    };

    for (int i = 0; i < FIXED_KERNEL_SIZE; i++) {
        for (int j = 0; j < FIXED_KERNEL_SIZE; j++) {
            mx[i * FIXED_KERNEL_SIZE + j] = embossValues[i * FIXED_KERNEL_SIZE + j];
        }
    }

    return mx;
}

float* blurM(int size) {
    float* mx = newQuadMatrix(size);

    int half = (size + 1) / 2;
    float sum = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mx[i * size + j] = gaussianFunc(i - half, j - half, 10, 1);
            sum += mx[i * size + j];
        }
    }

    normalize(mx, size, size, 1, sum);

    return mx;
}

float* uniformM(int size) {
    float* mx = newQuadMatrix(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mx[i * size + j] = 1.0 / (size * size);
        }
    }

    return mx;
}

float* motionBlurM(int size) {
    float* mx = newQuadMatrix(size);

    int half = (size + 1) / 2;

    for (int i = 0; i < size; i++) {
        mx[half * size + i] = 1.0 / size;
    }

    return mx;
}