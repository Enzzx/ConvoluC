#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include "../include/transform.h"
#include "../include/utils.h"
#include "../include/types.h"


int main() {
    char* img = "assets/mushroom.png";
    MatrixH Convo;

    // pega imagem
    ImgH Image;
    Image.data = stbi_load(img, &Image.w, &Image.h, &Image.c, 0);

    if (!Image.data) return printf("Não foi encontrada a imagem %s\n", img);

    int buffer = Image.w * Image.h * Image.c;
    printf("image buffer size: %d\n", buffer);

    // define filtro e faz padding da imagem
    setFilter(&Convo);
    paddImage(&Image, Convo.size);
    if (!Image.data) return printf("Erro ao adicionar padding em %s\n", img);

    // cria matriz de convoluçao
    defineMatrix(&Convo);

    // aplica convolução na imagem
    convoluteImg(&Image, &Convo);

    // salva saída
    stbi_write_png("saida.png", Image.w, Image.h, Image.c, Image.data, Image.w * Image.c);
    printf("Imagem criada com sucesso\n");


    // libera alocações
    for (int i = 0; i < Convo.size; i++) {
        free(Convo.M[i]);
    }
    free(Convo.M);
    Image.kt ? stbi_image_free(Image.data) : free(Image.data);
    return 0;
}
