#include <stdio.h>
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

void sobelM(int** matrix) {
    matrix[0][0] = -1; matrix[0][1] = 0; matrix[0][2] = 1;
    matrix[1][0] = -2; matrix[1][1] = 0; matrix[1][2] = 2;
    matrix[2][0] = -2; matrix[2][1] = 0; matrix[2][2] = 1;
}

void laplaceM(int** matrix) {
    matrix[0][0] = 0; matrix[0][1] = 1; matrix[0][2] = 0;
    matrix[1][0] = 1; matrix[1][1] = -4; matrix[1][2] = 1;
    matrix[2][0] = 0; matrix[2][1] = 1; matrix[2][2] = 0;
}

void embossM(int** matrix) {
    matrix[0][0] = -1; matrix[0][1] = 0; matrix[0][2] = 0;
    matrix[1][0] = 0; matrix[1][1] = 0; matrix[1][2] = 0;
    matrix[2][0] = 0; matrix[2][1] = 0; matrix[2][2] = 1;
}