#include "aes.h"
#include "Images.h"
#include <IL/il.h>
#include <stdlib.h>

typedef void(*CryptFunc)(byte*, int, byte*, byte*, int, int, byte*);

static void printData(byte* data, int size){
	for (int i = 0; i < size; i++)
		printf("%2x ", data[i]);
}

static void process(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type, CryptFunc f){
	ILuint srcID = ilGenImage();
	ilBindImage(srcID);
	ilLoadImage(srcFile);

	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);
	int bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
	printf("Bpp: %d\n", bpp);

	int size = width * height * bpp;
	printf("Image width %d, height %d size: %d\n", width, height, size);

	//byte *originalData = ilGetData();
	byte *originalData = (byte*)calloc(size, sizeof(byte));
	byte *result = (byte*)calloc(size, sizeof(byte));
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, originalData);

	f(originalData, size, key, result, rounds, type, iv);

	ILuint destID = ilGenImage();
	ilBindImage(destID);
	//ilTexImage(width, height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, originalData);
	//ilTexImage(width, height, 1, bpp, IL_RGB, IL_UNSIGNED_BYTE, result);
	//ilSetPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, result);
	//ilSetPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, originalData);
	ilLoadDataL(result, size, width, height, 1, bpp);
	//ilLoadDataL(originalData, size, width, height, 1, 3);
	
	
	ilSaveImage(destFile);

	
	ilDeleteImage(srcID);
	ilDeleteImage(destID);

	printf("Encrypted\n");
	//printData(originalData, size);
		
	
	printf("\nTo\n");
	//printData(result, size);
	free(result);
	free(originalData);


	printf("\n\n");
}


void encryptImage(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type){
	process(srcFile, destFile, key, rounds, iv, type, encrypt);
}

void decryptImage(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type){
	process(srcFile, destFile, key, rounds, iv, type, decrypt);
}