#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include "../include/transform.h"


int main() {
    char* img = "assets/mushroom.png";
    ImgH Image;
    Image.data = stbi_load(img, &Image.w, &Image.h, &Image.c, 0);

    if (!Image.data) return printf("Não foi encontrada a imagem %s\n", img);

    int buffer = Image.w * Image.h * Image.c;
    printf("image buffer size: %d\n", buffer);

    Image.pS = 20;
    paddImage(&Image);
    if (!Image.data) return printf("Erro ao adicionar padding em %s\n", img);

    stbi_write_png("saida.png", Image.w, Image.h, Image.c, Image.data, Image.w * Image.c);
    printf("Imagem criada com sucesso\n");

    stbi_image_free(Image.data);
    return 0;
}
