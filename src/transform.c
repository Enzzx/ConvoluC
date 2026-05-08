#include <stdlib.h>
#include <stdio.h>
#include "../include/transform.h"
#include "../include/utils.h"


void paddImage(ImgH *H) {
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
            int pixPos = (i * newW + j) * H->c;
            int isPadding = (i < H->pS || i >= H->pS + H->h) || (j < H->pS || j >= H->pS + H->w);

            for (int k = 0; k < H->c; k++) { // percorre cada canal de cor
                newData[pixPos + k] = isPadding ? 150 : H->data[((i - H->pS) * H->w + (j - H->pS)) * H->c + k];
            }
        }
    }

    H->w += 2 * H->pS;
    H->h += 2 * H->pS;

    swapImgRef(H, newData);
}