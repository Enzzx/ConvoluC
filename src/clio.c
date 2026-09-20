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