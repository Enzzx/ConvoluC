#include <bits/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include "../include/transform.h"
#include "../include/utils.h"
#include "../include/types.h"
#include "../include/clio.h"

#define REFPATH "assets"

int main() {
    char imgPath[256];
    char filename[128];
    printf("Digite o nome do arquivo: (path base -> '/%s')\n", REFPATH);
    scanf("%s", filename);
    sprintf(imgPath, "%s/%s", REFPATH, filename);

    MatrixH Convo;
    ImgH Image;
    configMatrix(&Image, &Convo);   // escolha do filtro

    Image.data = stbi_load(imgPath, &Image.w, &Image.h, &Image.c, 0);   // pega imagem
    if (!Image.data) return printf("Não foi encontrada a imagem %s\n", imgPath);
    Image.kt = 1;

    int buffer = ((Image.w + 2 * Image.pS) * (Image.h + 2 * Image.pS)) * Image.c;
    printf("image buffer size: %d\n", buffer);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    //paddImage(&Image, Convo.size);    // faz padding da imagem
    unsigned char* newImgBuffer = convoluteImg(&Image, &Convo);   // aplica convolução na imagem
    swapImgRef(&Image, newImgBuffer, 0);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double tempo_execucao = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("tempo de execução: %f\n", tempo_execucao);


    sprintf(imgPath, "%s-saida.png", filename);
    stbi_write_png(imgPath, Image.w, Image.h, Image.c, Image.data, Image.w * Image.c);  // salva saída
    printf("Imagem criada com sucesso\n");

    if (Convo.M)
        free(Convo.M);  // libera alocações
    Image.kt ? stbi_image_free(Image.data) : free(Image.data);

    return 0;
}
