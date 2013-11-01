#include "aes.h"
#include "Images.h"
#include <IL/il.h>
#include <stdlib.h>
#include <string.h>


void encryptImage(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type){
	ILuint srcID = ilGenImage();
	ilBindImage(srcID);
	ilLoadImage(srcFile);

	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);

	int size = width * height * 3;
	printf("Image width %d, height %d size: %d\n", width, height, size);

	
	byte *originalData = (byte*)calloc(size, sizeof(byte));
	byte *result = (byte*)calloc(size, sizeof(byte));

	
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, originalData);
	encrypt(originalData, size, key, result, rounds, type, iv);

	ILuint destID = ilGenImage();
	ilBindImage(destID);
	ilLoadDataL(result, size, width, height, 1, 3);
	//ilLoadDataL(originalData, size, width, height, 1, 3);
	ilSaveImage(destFile);

	ilDeleteImage(srcID);
	ilDeleteImage(destID);

	free(originalData);
}

void decryptImage(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type){
	ILuint srcID = ilGenImage();
	ilBindImage(srcID);
	ilLoadImage(srcFile);

	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);

	int size = width * height * 3;
	printf("Image width %d, height %d size: %d\n", width, height, size);


	byte *originalData = (byte*)calloc(size, sizeof(byte));
	byte *result = (byte*)calloc(size, sizeof(byte));


	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, originalData);
	decrypt(originalData, size, key, result, rounds, type, iv);
	/*memset(result, 0, size);
	memcpy(result, originalData, size);*/

	ILuint destID = ilGenImage();
	ilBindImage(destID);
	ilLoadDataL(result, size, width, height, 1, 3);
	ilSaveImage(destFile);

	ilDeleteImage(srcID);
	ilDeleteImage(destID);

	free(originalData);
}