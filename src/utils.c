#include <stdio.h>
#include <math.h>
#include "../include/utils.h"
#include "../include/stb_image.h"


void swapImgRef(ImgH* handler, unsigned char* newData, int posterior) {
    if (!handler || !newData) {
        printf("Erro ao receber dados para realizar swap de image buffer");
        return;
    }

    if (handler->kt) {
        stbi_image_free(handler->data);
        handler->kt = posterior;
    }
    else {
        free(handler->data);
    }
    handler->data = newData;
}

void defineMatrix(MatrixH* handler, ImgH* imgHandler) {
    int filterI;
    printf("\nSobel (0)\tLaplacian (1)\tEmboss (2)\tColorShift (3)\tIdentity (4)\tBlur (5)\tUniform (6)\nSelecione um filtro: ");
    scanf_s("%d", &filterI);
    handler->filter = filterI;

    if (handler->filter > Identity) {
        printf("\nSelecione o tamanho do kernel: ");
        scanf_s("%d", &handler->size);
    }

    switch (handler->filter) {
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
    case ColorShift:
        handler->M = colorShiftM();
        handler->size = 5;
        break;
    case Identity:
        handler->M = 1;
        handler->size = 1;
        break;

    case Blur:
        handler->M = blurM(handler->size);
        break;
    case Uniform:
        handler->M = uniformM(handler->size);
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
    float* matrix = (float*)malloc(sizeof(float) * size * size);

    return matrix;
}

float* sobelM() {
    float* mx = newQuadMatrix(FIXED_KERNEL_SIZE);
    float sobelValues[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };

    for (int i = 0; i < FIXED_KERNEL_SIZE; i++) {
        for (int j = 0; j < FIXED_KERNEL_SIZE; j++) {
            mx[i * FIXED_KERNEL_SIZE + j] = sobelValues[i * FIXED_KERNEL_SIZE + j];
        }
    }

    return mx;
}

float* laplaceM() {
    float* mx = newQuadMatrix(FIXED_KERNEL_SIZE);
    float laplaceValues[] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };

    for (int i = 0; i < FIXED_KERNEL_SIZE; i++) {
        for (int j = 0; j < FIXED_KERNEL_SIZE; j++) {
            mx[i * FIXED_KERNEL_SIZE + j] = laplaceValues[i * FIXED_KERNEL_SIZE + j];
        }
    }

    return mx;
}

float* embossM() {
    float* mx = newQuadMatrix(FIXED_KERNEL_SIZE);
    float embossValues[] = { -2, 0, 0, 0, 0, 0, 0, 0, 2 };

    for (int i = 0; i < FIXED_KERNEL_SIZE; i++) {
        for (int j = 0; j < FIXED_KERNEL_SIZE; j++) {
            mx[i * FIXED_KERNEL_SIZE + j] = embossValues[i * FIXED_KERNEL_SIZE + j];
        }
    }

    return mx;
}

float* colorShiftM() {
    float* mx = newQuadMatrix(5);
    float colorShiftValues[] = { -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2 };

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            mx[i * 5 + j] = colorShiftValues[i * 5 + j];
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