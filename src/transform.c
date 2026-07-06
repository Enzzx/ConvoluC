#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include "../include/transform.h"
#include "../include/utils.h"
#include "../include/types.h"

/* 
EXPORT int sum(int a, int b) {
	return a + b;
} */

void paddImage(ImgH* H, int mSize) {
	int newBuffer = (((H->w) + 2 * H->pS) * ((H->h) + 2 * H->pS)) * H->c;
	unsigned char* newData = (unsigned char*)malloc(sizeof(unsigned char) * newBuffer);

	int newW = H->w + 2 * H->pS;
	int newH = H->h + 2 * H->pS;

	for (int i = 0; i < newH; i++) {
		for (int j = 0; j < newW; j++) {
			int pixI = (i * newW + j) * H->c;
			int isPadding = (i < H->pS || i >= H->pS + H->h) || (j < H->pS || j >= H->pS + H->w);

			for (int k = 0; k < H->c; k++) {
				newData[pixI + k] = isPadding ? 0 : H->data[((i - H->pS) * H->w + (j - H->pS)) * H->c + k];
			}
		}
	}

	H->w += 2 * H->pS;
	H->h += 2 * H->pS;

	swapImgRef(H, newData, 0);
}


static inline float appColorShift(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel) {
	int channels = ImgH->c;
	int pixelsShift = MatrixH->size * channels;

	imgPixel[0] = ImgH->data[point - pixelsShift];
	imgPixel[1] = ImgH->data[point + 1];
	imgPixel[2] = ImgH->data[point + pixelsShift];

	for (int i = 3; i < channels; i++) {
		imgPixel[i] = ImgH->data[point + i];
	}

	return 255;
}

static inline float appNegativeColor(ImgH* ImgH, int point, unsigned char* imgPixel) {
	int channels = ImgH->c;
	for (int k = 0; k < channels; k++) {
		imgPixel[k] = 255 - ImgH->data[point + k];
	}

	return 255;
}

static inline float appGreyScale(ImgH* ImgH, int point, unsigned char* imgPixel) {
	int channels = ImgH->c;

	unsigned char R = ImgH->data[point];
	unsigned char G = ImgH->data[point + 1];
	unsigned char B = ImgH->data[point + 2];

	unsigned char greyScaled = (unsigned char)(0.299 * R + 0.587 * G + 0.114 * B);

	for (int k = 0; k < channels; k++) {
		imgPixel[k] = greyScaled;
	}

	return 255;
}

static inline float appSobel(ImgH* ImgH, int point, unsigned char* imgPixel) {
	float newVal[maxChannels] = { 0 };
	float newValT[maxChannels] = { 0 };
	float maxVal = 0;

	int channels = ImgH->c;
	int wLen = ImgH->w * channels;

	int rowTop = point - wLen;
	int rowMid = point;
	int rowBot = point + wLen;

	for (int k = 0; k < channels; k++) {
		newVal[k] =
			((float)ImgH->data[rowTop - channels + k] * -1.0f) + ((float)ImgH->data[rowTop + channels + k] * 1.0f) +
			((float)ImgH->data[rowMid - channels + k] * -2.0f) + ((float)ImgH->data[rowMid + channels + k] * 2.0f) +
			((float)ImgH->data[rowBot - channels + k] * -1.0f) + ((float)ImgH->data[rowBot + channels + k] * 1.0f);

		newValT[k] =
			((float)ImgH->data[rowTop - channels + k] * -1.0f) + ((float)ImgH->data[rowTop + k] * -2.0f) + ((float)ImgH->data[rowTop + channels + k] * -1.0f) +
			((float)ImgH->data[rowBot - channels + k] * 1.0f) + ((float)ImgH->data[rowBot + k] * 2.0f) + ((float)ImgH->data[rowBot + channels + k] * 1.0f);
	}

	for (int k = 0; k < channels; k++) {
		float kVal = sqrtf(newVal[k] * newVal[k] + newValT[k] * newValT[k]);

		if (kVal > 255.0f)      imgPixel[k] = 255;
		else if (kVal < 0.0f)   imgPixel[k] = 0;
		else                    imgPixel[k] = (unsigned char)kVal;

		if (kVal > maxVal)
			maxVal = kVal;
	}

	return maxVal;
}

static inline float appLaplace(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel) {
	float newVal[maxChannels] = { 0 };
	float maxVal = 0;

	int size = MatrixH->size;
	int width = ImgH->w;
	int channels = ImgH->c;
	int half = (size + 1) / 2;

	for (int i = 0; i < size; i++) {
		int offsetY = (i + 1 - half) * width * channels;
		int lineHeight = i * size;

		for (int j = 0; j < size; j++) {
			int offsetX = (j + 1 - half) * channels;
			int pixBase = point + offsetY + offsetX;

			float mVal = MatrixH->M[lineHeight + j];
			for (int k = 0; k < channels; k++) {
				newVal[k] += ImgH->data[pixBase + k] * mVal;
			}
		}
	}

	for (int k = 0; k < channels; k++) {
		float kVal = newVal[k] > 40 ? 255 : 0;
		imgPixel[k] = kVal;

		if (kVal > maxVal)
			maxVal = kVal;
	}

	return maxVal;
}

static inline float appEmboss(ImgH* ImgH, int point, unsigned char* imgPixel) {
	float maxVal = 0;
	int width = ImgH->w;
	int channels = ImgH->c;

	int topLeftPos = point - width * channels - channels;
	int bottomRightPos = point + width * channels + channels;

	for (int k = 0; k < channels; k++) {
		float kVal = ImgH->data[topLeftPos + k] * -2 + ImgH->data[bottomRightPos + k] * 2;
		kVal += 128;

		if (kVal < 0) kVal = 0;
		else if (kVal > 255) kVal = 255;

		imgPixel[k] = kVal;

		if (kVal > maxVal)
			maxVal = kVal;
	}

	return maxVal;
}

static inline float appMotionBlur(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel) {
	float newVal[maxChannels] = { 0 };
	int channels = ImgH->c;
	int size = MatrixH->size;
	int half = (size + 1) / 2;

	for (int i = 0; i < size; i++) {
		int shift = (i - half) * channels;

		for (int k = 0; k < channels; k++) {
			newVal[k] += ImgH->data[point + shift + k] * 1.0/size;
		}
	}

	for (int k = 0; k < channels; k++) {
		imgPixel[k] = newVal[k];
	}

	return 255;
}

static inline float appSharpen(ImgH* ImgH, int point, unsigned char* imgPixel) {
	float newVal[maxChannels] = { 0 };
	float maxVal = 0;

	int channels = ImgH->c;
	int wLen = ImgH->w * channels;

	int rowTop = point - wLen;
	int rowMid = point;
	int rowBot = point + wLen;

	for (int k = 0; k < channels; k++) {
		newVal[k] =
			((float)ImgH->data[rowTop + k] * -1.0f) +
			((float)ImgH->data[rowMid - channels + k] * -1.0f) +
			((float)ImgH->data[rowMid + k] * 5.0f) +
			((float)ImgH->data[rowMid + channels + k] * -1.0f) +
			((float)ImgH->data[rowBot + k] * -1.0f);
	}

	for (int k = 0; k < channels; k++) {
		float kVal = newVal[k];

		if (kVal > 255.0f)      imgPixel[k] = 255;
		else if (kVal < 0.0f)   imgPixel[k] = 0;
		else                    imgPixel[k] = (unsigned char)kVal;

		if (kVal > maxVal)
			maxVal = kVal;
	}

	return maxVal;
}

static inline float appDefault(ImgH* ImgH, MatrixH* MatrixH, int point, unsigned char* imgPixel) {
	float newVal[maxChannels] = { 0 };
	float maxVal = 0;

	int size = MatrixH->size;
	int width = ImgH->w;
	int channels = ImgH->c;
	int half = (size + 1) / 2;

	for (int i = 1; i < size - 1; i++) {
		int offsetY = (i + 1 - half) * width * channels;
		int lineHeight = i * size;

		for (int j = 0; j < size; j++) {
			int offsetX = (j + 1 - half) * channels;
			int pixBase = point + offsetY + offsetX;

			float mVal = MatrixH->M[lineHeight + j];
			for (int k = 0; k < channels; k++) {
				newVal[k] += ImgH->data[pixBase + k] * mVal;
			}
		}
	}

	for (int k = 0; k < channels; k++) {
		float kVal = newVal[k];
		imgPixel[k] = kVal;

		if (kVal > maxVal)
			maxVal = kVal;
	}

	return maxVal;
}


static inline float applicateKernelP(ImgH* i, MatrixH* k, int p, unsigned char* nM) {
	switch (k->filter) {
	case ColorShift:    return appColorShift(i, k, p, nM);
	case NegativeColor:	return appNegativeColor(i, p, nM);
	case Greyscale:		return appGreyScale(i, p, nM);
	case Identity:		return 0.0;
	case SobelEdge:     return appSobel(i, p, nM);
	case LaplacianEdge: return appLaplace(i, k, p, nM);
	case Emboss:        return appEmboss(i, p, nM);
	case MotionBlur:    return appMotionBlur(i, k, p, nM);
	case Sharpen:       return appSharpen(i, p, nM);
	default:            return appDefault(i, k, p, nM);
	}
}


EXPORT void convoluteImg(ImgH* img, MatrixH* kernel) {
	float maxVal = 0;
	unsigned char* newMatrix = (unsigned char*)malloc(sizeof(unsigned char) * img->w * img->h * img->c);

	int fullWlen = img->w * img->c;
	int i;

#pragma omp parallel for schedule(static) private(i) reduction(max: maxVal)
	for (i = img->pS; i < img->h - img->pS; i++) {
		for (int j = img->pS * img->c; j < (img->w - img->pS) * img->c; j += img->c) {
			int pixI = i * fullWlen + j;

			float pMaxVal = applicateKernelP(img, kernel, pixI, &newMatrix[pixI]);
			if (pMaxVal > maxVal) maxVal = pMaxVal;
		}
	}

	/*if (kernel->filter == LaplacianEdge) {
		// tá quebrando por algum motivo
		normalize(newMatrix, img->w, img->h, img->c, maxVal);
	}*/

	swapImgRef(img, newMatrix, 0);
}