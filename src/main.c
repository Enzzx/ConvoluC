#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include "../include/transform.h"


void swapImgRef(ImgH* handler, unsigned char* newData) {
    if (!handler || !newData) {
        printf("Erro em swap de image buffer");
        return;
    }

    stbi_image_free(handler->data);
    handler->data = newData;
}


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
    return;

    for (int i = 0; i < buffer; i += Image.c) {
        int greyscale = (Image.data[i] + Image.data[i + 1] + Image.data[i + 2]) / 3;
        Image.data[i] = greyscale;
        Image.data[i + 1] = greyscale;
        Image.data[i + 2] = greyscale;
    }

    printf("Imagem criada com sucesso\n");

    stbi_image_free(Image.data);
}
