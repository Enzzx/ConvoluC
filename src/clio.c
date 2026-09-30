#include <stdio.h>
#include "../include/stb_image.h"
#include "../include/clio.h"
#include "../include/utils.h"

void swapImgRef(ImgH* handler, unsigned char* newData, int posterior) {
    if (!handler || !newData) {
        printf("Erro ao receber dados para realizar swap de image buffer");
        return;
    }

    if (handler->kt) {
        stbi_image_free(handler->data);
    }
    else {
        free(handler->data);
    }
    handler->data = newData;
    handler->kt = posterior;
}

void configMatrix(ImgH* imgHandler, MatrixH* handler) {
    int filterI;
    
    const char* filterNames[] = {
        "ColorShift",
        "NegativeColor",
        "GreyScale",
        "ChromaKey",
        "SobelEdge",
        "LaplacianEdge",
        "Emboss",
        "Identity",
        "Erosion",
        "Dilation",
        "Blur",
        "Uniform",
        "MotionBlur",
        "Sharpen",
        "KuwaharaFilter"
    };

    for (int i = 0; i < 15; i++) {
        printf("(%d) %s\t\t", i, filterNames[i]);
        if (i % 4 == 3) printf("\n");
    }
    printf("\nEscolha um filtro: ");

    scanf("%d", &filterI);
    handler->filter = filterI;

    if (handler->filter > Identity || handler->filter == ColorShift) {
        printf("\nSelecione o tamanho do kernel: ");
        scanf("%d", &handler->size);

        if (handler->size > 100) {
            printf("\nQuer fritar a CPU paezao?!!");
            return;
        }

        if (handler->size % 2 == 0) handler->size++;
    }

    if (handler->filter == ChromaKey) {
        printf("Escolha a nova imagem de fundo: ");
        char backPath[256];
        scanf("%s", backPath);
        
        int x, y, c;
        imgHandler->aux = stbi_load(backPath, &x, &y, &c, 0);
    }

    defineMatrix(imgHandler, handler);
}
