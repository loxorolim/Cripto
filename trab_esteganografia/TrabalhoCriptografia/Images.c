#include "AES.h"
#include "Images.h"
#include <IL/il.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef void(*CryptFunc)(byte*, int, byte*, byte*, int, int, byte*);

static void printData(byte* data, int size){
	for (int i = 0; i < size; i++)
		printf("%2x ", data[i]);
	printf("\n");
}


byte normColor2(byte original, int count){
	float MAXIMUM = (float)(pow(2, count) - 1.0);
	float pctg = original / MAXIMUM;
	return (byte)(pctg * 255.0f);
}

byte genInverseMask(int count){
	/*byte dest = 0;
	for (int i = count; i < 8; i++)
		dest = dest | (1 << i);
	return dest;*/
	return genMask(count) ^ 0xFF;
}

byte genMask(int count){
	byte dest = 0;
	for (int i = 0; i < count; i++)
		dest = dest | (1 << i);
	return dest;
}

void process(const char* srcFile, const char* destFile, int bitCount){
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

	byte mask = genMask(bitCount);

	for (int i = 0; i < size; i+=3){
		byte r = originalData[i];
		byte g = originalData[i+1];
		byte b = originalData[i+2];

		r = r & mask;
		g = g & mask;
		b = b & mask;

		result[i] = normColor2(r, bitCount);
		result[i + 1] = normColor2(g, bitCount);
		result[i + 2] = normColor2(b, bitCount);
	}











	//f(originalData, size, key, result, rounds, type, iv);   //aplica o processo desejado

	ILuint destID = ilGenImage();                           //gera imagem resultante
	ilBindImage(destID);                                    //manda usar essa imagem
	ilLoadDataL(result, size, width, height, 1, bpp);       //preenche a imagem resultante com os pixels calculados
	
	ilSaveImage(destFile);                                  //salva a imagem resultante em disco
	
	ilDeleteImage(srcID);                                   //apaga as duas imagens
	ilDeleteImage(destID);

	free(result);                                           //libera a memória dos pixels processados
	free(originalData);
}

byte doMask(byte b, const char *mask){
	byte resp = 0;
	int i;
	for (i = 0; i < 8; i++)
		if (mask[i] == '1')
			resp = resp | (b & (1 << (7 - i)));
	return resp;
}

void aplicaDoge(const char* doge, const char* destFile, const char*tree, int bitCount){
	ILuint dogeImg = ilGenImage();               //gera a imagem de origem
	ilBindImage(dogeImg);                        //manda trabalhar com essa imagem
	ilLoadImage(doge);                      //carrega a imagem de origem do arquivo

	int width = ilGetInteger(IL_IMAGE_WIDTH);            //obtém largura da imagem
	int height = ilGetInteger(IL_IMAGE_HEIGHT);          //obtém altura da imagem
	int bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);    //obtém taxa de "bytes per pixel" da imagem

	int size = width * height * bpp;                     //calcula tamanho de bytes na imagem

	byte *originalData = (byte*)calloc(size, sizeof(byte)); //aloca espaço para uma cópia dos pixels originais
	byte *result = (byte*)calloc(size, sizeof(byte));       //aloca os pixels da imagem resultado
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, originalData);    //obtém uma cópia dos pixels originais

	ILuint treeImg = ilGenImage();
	ilBindImage(treeImg);
	ilLoadImage(tree);

	byte *treeData = (byte*)calloc(size, sizeof(byte)); //aloca espaço para uma cópia dos pixels originais
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, treeData);    //obtém uma cópia dos pixels originais
	
	byte mask1 = genInverseMask(8-bitCount);

	byte mask2 = genInverseMask(bitCount);

	int i;
	for (i = 0; i < size; i += 3){
		byte rDoge = originalData[i];
		byte gDoge = originalData[i + 1];
		byte bDoge = originalData[i + 2];

		byte rMost = (rDoge & mask1) >> (8 - bitCount); //pega os (8-n) bits mais significativos e move pra direita
		byte gMost = (gDoge & mask1) >> (8 - bitCount);
		byte bMost = (bDoge & mask1) >> (8 - bitCount);


		byte rtree = treeData[i] & mask2; //pega os n bits mais significativos e deixa onde estão (na esquerda)
		byte gtree = treeData[i + 1] & mask2;
		byte btree = treeData[i + 2] & mask2;

		result[i] = rtree | rMost; //junta os n bits da imagem auxiliar com os (8-n) da mensagem
		result[i + 1] = gtree | gMost;
		result[i + 2] = btree | bMost;
	}











	//f(originalData, size, key, result, rounds, type, iv);   //aplica o processo desejado

	ILuint destID = ilGenImage();                           //gera imagem resultante
	ilBindImage(destID);                                    //manda usar essa imagem
	ilLoadDataL(result, size, width, height, 1, bpp);       //preenche a imagem resultante com os pixels calculados

	ilSaveImage(destFile);                                  //salva a imagem resultante em disco

	ilDeleteImage(dogeImg);                                   //apaga as duas imagens
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