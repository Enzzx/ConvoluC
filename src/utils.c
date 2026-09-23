#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/utils.h"

EXPORT void freeFrame(unsigned char* frameBuffer) {
  free(frameBuffer);
}

EXPORT void defineMatrix(ImgH* imgHandler, MatrixH* handler) {
    switch (handler->filter) {
    case ColorShift:
        handler->M = (float*)1;
        break;
    case NegativeColor:
        handler->M = (float*)1;
        break;
    case Greyscale:
        handler->M = (float*)1;
        break;

    case SobelEdge:
        handler->M = sobelM();
        handler->size = FIXED_KERNEL_SIZE;
        break;
    case LaplacianEdge:
        handler->M = laplaceM();
        handler->size = FIXED_KERNEL_SIZE;
        break;
    case Emboss:
        handler->M = embossM();
        handler->size = FIXED_KERNEL_SIZE;
        break;
    case Identity:
        handler->M = 0;
        break;

    case Erosion:
        handler->M = 0;
        break;
    case Dilation:
        handler->M = 0;
        break;
    case Blur:
        handler->M = blurM(handler->size);
        break;
    case Uniform:
        handler->M = uniformM(handler->size);
        break;
    case MotionBlur:
        handler->M = 0;
        break;
    case Sharpen:
        handler->M = blurM(handler->size);
        break;
    case KuwaharaFilter:
        handler->M = 0;
        break;
    default:
        break;
    }

    imgHandler->pS = (handler->size - 1) / 2;
}

void configMatrix(ImgH* imgHandler, MatrixH* handler) {
    int filterI;
    
    const char* filterNames[] = {
        "ColorShift",
        "NegativeColor",
        "GreyScale",
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

    for (int i = 0; i < 14; i++) {
        printf("(%d) %s\t\t", i, filterNames[i]);
        if (i % 4 == 3) printf("\n");
    }
    printf("\nEscolha um filtro: ");

    scanf("%d", &filterI);
    handler->filter = filterI;

    if (handler->filter > Identity) {
        printf("\nSelecione o tamanho do kernel: ");
        scanf("%d", &handler->size);

        if (handler->size > 100) {
            printf("\nQuer fritar a CPU paezao?!!");
            return;
        }

        if (handler->size % 2 == 0) handler->size++;
    }

    switch (handler->filter) {
    case ColorShift:
        handler->M = 0;
        handler->size = 1;
        printf("\nEscolha o tamanho de desvio: ");
        scanf("%d", &handler->size);
        break;
    case NegativeColor:
        handler->M = 0;
        handler->size = 1;
        break;
    case Greyscale:
        handler->M = 0;
        handler->size = 1;
        break;

    case SobelEdge:
        handler->M = sobelM();
        handler->size = FIXED_KERNEL_SIZE;
        break;
    case LaplacianEdge:
        handler->M = laplaceM();
        handler->size = FIXED_KERNEL_SIZE;
        break;
    case Emboss:
        handler->M = embossM();
        handler->size = FIXED_KERNEL_SIZE;
        break;
    case Identity:
        handler->M = 0;
        handler->size = 1;
        break;

    case Erosion:
        handler->M = 0;
        handler->size = handler->size;
        break;
    case Blur:
        handler->M = blurM(handler->size);
        break;
    case Uniform:
        handler->M = uniformM(handler->size);
        break;
    case MotionBlur:
        handler->M = 0;
        break;
    case Sharpen:
        handler->M = blurM(handler->size);
        break;
    case KuwaharaFilter:
        handler->M = 0;
        break;
    default:
        break;
    }

    imgHandler->pS = (handler->size - 1) / 2;
}

void rgb2hsv(int r, int g, int b, float* h, float* s, float* v) {
    float nR = (float)r/255;
    float nG = (float)g/255;
    float nB = (float)b/255;

    char Cmax = nR > nG && nR > nB ? 'r' : nG > nB ? 'g' : 'b';
    char Cmin = nR < nG && nR < nB ? 'r' : nG < nB ? 'g' : 'b';
    float delta = (Cmax == 'r' ? nR : Cmax == 'g' ? nG : nB) - (Cmin == 'r' ? nR : Cmin == 'g' ? nG : nB);
    int max = nR > nG && nR > nB ? nR : nG > nB ? nG : nB;

    if (h != NULL) {
        if (Cmax == 'r') {
            *h = 60 * ((int)((nG - nB) / delta) % 6);
        } else if (Cmax == 'g') {
            *h = 60 * ((int)((nB - nR) / delta) + 2);
        } else {
            *h = 60 * ((int)((nR - nG) / delta) + 4);
        }
    }
    if (s != NULL) {
        *s = max == 0 ? 0 : delta/max * 100;
    }
    if (v != NULL) {
        *v = max * 100;
    }
}

void normalize(float* matrix, int sizeX, int sizeY, int sizeC, float divisor) {
    for (int i = 0; i < sizeY; i++) {
        for (int j = 0; j < sizeX; j++) {
            for (int k = 0; k < sizeC; k++) {
                matrix[(i * sizeX + j) * sizeC + k] /= divisor;

            }
        }
    }
}

float gaussianFunc(int x, int y, int sigma, int weight) {
    double e = exp(1.0);
    double expoent = (pow(x+weight, 2) + pow(y+weight, 2)) / (2 * pow(sigma, 2));
    double result = pow(e, -expoent);

    return result;
}

float stdDeviation(float data[], int n /*array size*/) {
  if(n <= 0) return 0.0f;

  float mean = 0, sum = 0, squaredDiffSum = 0, deviation = 0;
  
  //find mean value
  for(int i = 0; i < n; i++){
    sum += data[i];
  }

  mean = sum / n;
  
  for(int i = 0; i < n; i++){
    float deltaMean = data[i] - mean; //diference between each value and mean value
    squaredDiffSum += deltaMean * deltaMean; //summation of the difference squared
  }

  deviation = sqrtf(squaredDiffSum / n);

  return deviation;
}

float* newQuadMatrix(int size) {
    float* matrix = (float*)calloc(size * size, sizeof(float));

    return matrix;
}

float* sobelM() {
    float* mx = newQuadMatrix(FIXED_KERNEL_SIZE);
    float sobelValues[] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };

    for (int i = 0; i < FIXED_KERNEL_SIZE; i++) {
        for (int j = 0; j < FIXED_KERNEL_SIZE; j++) {
            mx[i * FIXED_KERNEL_SIZE + j] = sobelValues[i * FIXED_KERNEL_SIZE + j];
        }
    }

    return mx;
}

float* laplaceM() {
    float* mx = newQuadMatrix(FIXED_KERNEL_SIZE);
    float laplaceValues[] = {
        0, 1, 0,
        1, -4, 1,
        0, 1, 0
    };

    for (int i = 0; i < FIXED_KERNEL_SIZE; i++) {
        for (int j = 0; j < FIXED_KERNEL_SIZE; j++) {
            mx[i * FIXED_KERNEL_SIZE + j] = laplaceValues[i * FIXED_KERNEL_SIZE + j];
        }
    }

    return mx;
}

float* embossM() {
    float* mx = newQuadMatrix(FIXED_KERNEL_SIZE);
    float embossValues[] = {
        -2, 0, 0,
        0, 0, 0,
        0, 0, 2
    };

    for (int i = 0; i < FIXED_KERNEL_SIZE; i++) {
        for (int j = 0; j < FIXED_KERNEL_SIZE; j++) {
            mx[i * FIXED_KERNEL_SIZE + j] = embossValues[i * FIXED_KERNEL_SIZE + j];
        }
    }

    return mx;
}

float* blurM(int size) {
    float* mx = newQuadMatrix(size);

    int half = (size + 1) / 2;
    float sum = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mx[i * size + j] = gaussianFunc(i - half, j - half, 10, 1);
            sum += mx[i * size + j];
        }
    }

    normalize(mx, size, size, 1, sum);

    return mx;
}

float* uniformM(int size) {
    float* mx = newQuadMatrix(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mx[i * size + j] = 1.0 / (size * size);
        }
    }

    return mx;
}

float* motionBlurM(int size) {
    float* mx = newQuadMatrix(size);

    int half = (size + 1) / 2;

    for (int i = 0; i < size; i++) {
        mx[half * size + i] = 1.0 / size;
    }

    return mx;
}
