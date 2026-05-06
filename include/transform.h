#ifndef TRANSFORM_H
#define TRANSFORM_H

typedef struct {
    unsigned char* data;
    int w;
    int h;
    int c;
    int pS;
} ImgH;

void paddImage(ImgH* H);

#endif