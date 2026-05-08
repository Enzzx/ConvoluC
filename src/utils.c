#include <stdio.h>
#include "../include/utils.h"
#include "../include/transform.h"
#include "../include/stb_image.h"

void swapImgRef(ImgH* handler, unsigned char* newData) {
    if (!handler || !newData) {
        printf("Erro em swap de image buffer");
        return;
    }

    stbi_image_free(handler->data);
    handler->data = newData;
}