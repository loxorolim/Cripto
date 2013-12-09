#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include "utils.h"

typedef struct {
	int width, height, bpp, byteCount;
	byte *data;
}ImageData;

ImageData* generateEmptyData(int width, int height, int bpp);
ImageData* generateData(const char *fileName);
void saveData(ImageData *d, const char *fileName);
void deleteData(ImageData* d);

#endif