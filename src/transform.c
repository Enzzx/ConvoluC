#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <threads.h>
#include "../include/stb_image.h"
#include "../include/transform.h"
#include "../include/utils.h"
#include "../include/types.h"


void paddImage(ImgH *H, int mSize) {
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

    swapImgRef(H, newData, 0);
}

#define NUM_THREADS 16 // Defina baseado nos núcleos da sua CPU

void convoluteImg(ImgH* img, MatrixH* kernel) {
    float maxVal = 0;
    unsigned char* newMatrix = (unsigned char*)malloc(sizeof(unsigned char) * img->w * img->h * img->c);

    thrd_t threads[NUM_THREADS];
    rowKernelH handlers[NUM_THREADS]; // Cada thread precisa da sua própria struct para evitar conflitos

    int totalRows = (img->h - img->pS) - img->pS;
    int rowsPerThread = totalRows / NUM_THREADS;

    // Dispara todas as threads simultaneamente
    for (int t = 0; t < NUM_THREADS; t++) {
        handlers[t].img = img;
        handlers[t].kernel = kernel;
        handlers[t].newMatrix = newMatrix;

        // Cada thread ganha um intervalo de linhas independente
        handlers[t].startRow = img->pS + (t * rowsPerThread);
        handlers[t].endRow = (t == NUM_THREADS - 1) ? (img->h - img->pS) : (handlers[t].startRow + rowsPerThread);

        // Cada thread calcula seu maxVal local para evitar Race Condition
        handlers[t].localMax = 0;

        thrd_create(&threads[t], convoluteRow, &handlers[t]);
    }

    // Agora sim: espera todas terminarem (elas rodaram juntas!)
    for (int t = 0; t < NUM_THREADS; t++) {
        thrd_join(threads[t], NULL);
        if (handlers[t].localMax > maxVal) {
            maxVal = handlers[t].localMax;
        }
    }

    // Se o seu normalize usa o maxVal, ele deve ser chamado aqui
    if (kernel->filter == LaplacianEdge) {
        normalize(newMatrix, img->w, img->h, img->c, maxVal);
    }

    swapImgRef(img, newMatrix, 0);
}


int convoluteRow(void* args) {
    rowKernelH* data = (rowKernelH*)args;

    // Modificado para rodar do startRow até o endRow da thread
    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = data->img->pS; j < (data->img->w - data->img->pS) * data->img->c; j += data->img->c) {
            int pixI = i * data->img->w * data->img->c + j;

            for (int k = 0; k < data->img->c; k++) {
                unsigned char newVal = applicateKernel(data->img, data->kernel, pixI + k);

                // Correção da comparação do MaxVal usando variável local da struct
                data->localMax = data->localMax >= newVal ? data->localMax : (float)newVal;
                data->newMatrix[pixI + k] = newVal;
            }
        }
    }
    return 0;
}


unsigned char applicateKernel(ImgH* ImgH, MatrixH* MatrixH, int point) {
    float newVal = 0;
    float newValT = 0;
    int half = (MatrixH->size + 1) / 2;

    for (int i = 0; i < MatrixH->size; i++) {
        for (int j = 0; j < MatrixH->size; j++) {
            int diff = i+1 - half;
            int shift = diff * ImgH->w * ImgH->c;

            int index = point + shift + (j * ImgH->c);
            if (index < 0 || index >= ImgH->w * ImgH->h * ImgH->c) continue;

            newVal += ImgH->data[point + shift + (j * ImgH->c)] * MatrixH->M[i * MatrixH->size + j];
            newValT += ImgH->data[point + shift + (j * ImgH->c)] * MatrixH->M[j * MatrixH->size + i];
        }
    }

    switch (MatrixH->filter) {
    case SobelEdge:
        return sqrt(newVal * newVal + newValT * newValT);
        break;
    case LaplacianEdge:
        return newVal > 40 ? 255 : 0;
        break;
    case Emboss:
        newVal += 128;
        if (newVal < 0) return 0;
        else if (newVal > 255) return 255;
        else return newVal;
        break;
    case ColorShift:
        newVal += 128;
        if (newVal < 0) return 0;
        else if (newVal > 255) return 255;
        else return newVal;
        break;
    default:
        return newVal;
       
    }
}