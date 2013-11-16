#include "AES.h"
#include "Images.h"
#include <IL/il.h>
#include <stdlib.h>
#include <string.h>

typedef void(*CryptFunc)(byte*, int, byte*, byte*, int, int, byte*);

static void printData(byte* data, int size){
	for (int i = 0; i < size; i++)
		printf("%2x ", data[i]);
	printf("\n");
}

void process(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type, CryptFunc f){
	ILuint srcID = ilGenImage();               //gera a imagem de origem
	ilBindImage(srcID);                        //manda trabalhar com essa imagem
	ilLoadImage(srcFile);                      //carrega a imagem de origem do arquivo

	int width = ilGetInteger(IL_IMAGE_WIDTH);            //obtém largura da imagem
	int height = ilGetInteger(IL_IMAGE_HEIGHT);          //obtém altura da imagem
	int bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);    //obtém taxa de "bytes per pixel" da imagem

	int size = width * height * bpp;                     //calcula tamanho de bytes na imagem

	byte *originalData = (byte*)calloc(size, sizeof(byte)); //aloca espaço para uma cópia dos pixels originais
	byte *result = (byte*)calloc(size, sizeof(byte));       //aloca os pixels da imagem resultado
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, originalData);    //obtém uma cópia dos pixels originais

	f(originalData, size, key, result, rounds, type, iv);   //aplica o processo desejado

	ILuint destID = ilGenImage();                           //gera imagem resultante
	ilBindImage(destID);                                    //manda usar essa imagem
	ilLoadDataL(result, size, width, height, 1, bpp);       //preenche a imagem resultante com os pixels calculados
	
	ilSaveImage(destFile);                                  //salva a imagem resultante em disco
	
	ilDeleteImage(srcID);                                   //apaga as duas imagens
	ilDeleteImage(destID);

	free(result);                                           //libera a memória dos pixels processados
	free(originalData);
}


void encryptImage(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type){
	process(srcFile, destFile, key, rounds, iv, type, encrypt);
}

void decryptImage(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type){
	process(srcFile, destFile, key, rounds, iv, type, decrypt);
}

float getImageHammingDistance(const char* srcFile, const char* destFile){
	ILuint srcID = ilGenImage();               //gera a imagem de origem
	ilBindImage(srcID);                        //manda trabalhar com essa imagem
	ilLoadImage(srcFile);                      //carrega a imagem de origem do arquivo

	int width = ilGetInteger(IL_IMAGE_WIDTH);            //obtém largura da imagem
	int height = ilGetInteger(IL_IMAGE_HEIGHT);          //obtém altura da imagem
	int bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);    //obtém taxa de "bytes per pixel" da imagem

	int size = width * height * bpp;                     //calcula tamanho de bytes na imagem

	byte *srcData = ilGetData();

	ILuint destID = ilGenImage();
	ilBindImage(destID);
	ilLoadImage(destFile);
	byte *destData = ilGetData();

	float resp = calculateHammingDistance(srcData, destData, size);

	ilDeleteImage(srcID);
	ilDeleteImage(destID);
	return resp;
}