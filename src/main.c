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
    // escolha do filtro
    defineMatrix(&Convo, &Image);

    clock_t clocki = clock();
    // pega imagem
    Image.data = stbi_load(img, &Image.w, &Image.h, &Image.c, 0);
    if (!Image.data) return printf("Não foi encontrada a imagem %s\n", img);

    // faz padding da imagem
    /*paddImage(&Image, Convo.size);
    if (!Image.data) return printf("Erro ao adicionar padding em %s\n", img);*/

    int buffer = Image.w * Image.h * Image.c;
    printf("image buffer size: %d\n", buffer);

    // aplica convolução na imagem
    convoluteImg(&Image, &Convo);

    // salva saída
    stbi_write_png("saida.png", Image.w, Image.h, Image.c, Image.data, Image.w * Image.c);
    printf("Imagem criada com sucesso\n");

    // libera alocações
    free(Convo.M);
    Image.kt ? stbi_image_free(Image.data) : free(Image.data);

    clock_t clockf = clock();
    double tempo_execucao = (double)(clockf - clocki) / CLOCKS_PER_SEC;
    printf("tempo de execução: %f", tempo_execucao);

    return 0;
}
