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


int main() {
    char* img = "assets/mario.png";
    MatrixH Convo;
    ImgH Image;
    defineMatrix(&Convo, &Image);   // escolha do filtro

    Image.data = stbi_load(img, &Image.w, &Image.h, &Image.c, 0);   // pega imagem
    if (!Image.data) return printf("Não foi encontrada a imagem %s\n", img);

    int buffer = ((Image.w + 2 * Image.pS) * (Image.h + 2 * Image.pS)) * Image.c;
    printf("image buffer size: %d\n", buffer);



    clock_t clocki = clock();
    /*paddImage(&Image, Convo.size);    // faz padding da imagem
    if (!Image.data) return printf("Erro ao adicionar padding em %s\n", img);*/

    convoluteImg(&Image, &Convo);   // aplica convolução na imagem

    clock_t clockf = clock();
    double tempo_execucao = (double)(clockf - clocki) / CLOCKS_PER_SEC;
    printf("tempo de execução: %f\n", tempo_execucao);



    stbi_write_png("saida.png", Image.w, Image.h, Image.c, Image.data, Image.w * Image.c);  // salva saída
    printf("Imagem criada com sucesso\n");

    free(Convo.M);  // libera alocações
    Image.kt ? stbi_image_free(Image.data) : free(Image.data);

    return 0;
}
