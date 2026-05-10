#include <stdio.h>
#include <math.h>
#include "../include/utils.h"
#include "../include/stb_image.h"

void swapImgRef(ImgH* handler, unsigned char* newData) {
    if (!handler || !newData) {
        printf("Erro em swap de image buffer");
        return;
    }

    stbi_image_free(handler->data);
    handler->data = newData;
}

void setFilter(MatrixH* handler) {
    // criar um scanf pra filter
    handler->filter = Blur;

    if (handler->filter > Identity) {
        // criar um scanf pra size
        handler->size = 5;
        return;
    }

    handler->size = 3;
}

float gaussianFunc(int x, int y, int sigma, int weight) {
    double e = exp(1.0);
    double expoent = (pow(x+weight, 2) + pow(y+weight, 2)) / (2 * pow(sigma, 2));
    double result = pow(e, -expoent);

    return result;
}

float** newQuadMatrix(int size) {
    float** matrix = (float*)malloc(sizeof(float) * size);
    for (int i = 0; i < size; i++) {
        matrix[i] = malloc(sizeof(float) * size);
    }

    return matrix;
}

float** sobelM() {
    float** mx = newQuadMatrix(3);

    mx[0][0] = -1; mx[0][1] = 0; mx[0][2] = 1;
    mx[1][0] = -2; mx[1][1] = 0; mx[1][2] = 2;
    mx[2][0] = -2; mx[2][1] = 0; mx[2][2] = 1;

    return mx;
}

float** laplaceM() {
    float** mx = newQuadMatrix(3);

    mx[0][0] = 0; mx[0][1] = 1; mx[0][2] = 0;
    mx[1][0] = 1; mx[1][1] = -4; mx[1][2] = 1;
    mx[2][0] = 0; mx[2][1] = 1; mx[2][2] = 0;

    return mx;
}

float** embossM() {
    float** mx = newQuadMatrix(3);

    mx[0][0] = -1; mx[0][1] = 0; mx[0][2] = 0;
    mx[1][0] = 0; mx[1][1] = 0; mx[1][2] = 0;
    mx[2][0] = 0; mx[2][1] = 0; mx[2][2] = 1;

    return mx;
}

float** blurM(int size) {
    float** mx = newQuadMatrix(size);

    int half = (size + 1) / 2;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 0; j++) {
            mx[i][j] = gaussianFunc(i - half, j - half, 2, 1) / (size * size);
        }
    }

    return mx;
}

float** uniformM(int size) {
    float** mx = newQuadMatrix(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 0; j++) {
            mx[i][j] = (float)1 / (size * size);
        }
    }
}