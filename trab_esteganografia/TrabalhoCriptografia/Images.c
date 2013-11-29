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

byte normColor(byte original, byte factor){
	float pctg = factor / 255.0f;
	float fOriginal = (float)original;
	byte result = (byte)(fOriginal * pctg);
	//printf("original %d, factor %d, pctg %f, result %d\n", original, factor, pctg, result);
	//return result;
	return factor;
}

byte normColor2(byte original, int count){
	float pctg = original / (count + 1.0f);
	return (byte)(pctg * 255.0f);
	/*switch (original){
	case 0:
		return 0;
	case 1:
		return 85;
	case 2:
		return 170;
	}
	return 255;*/
}

char * genInverseMask(char *dest, int count){
	for (int i = 0; i < 8; i++)
		dest[i] = (7 - i) < count ? '0' : '1';
	
	return dest;
}

char * genMask(char *dest, int count){
	for (int i = 0; i < 8; i++)
		dest[i] = (7 - i) < count ? '1' : '0';

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

	char temp[9];
	temp[8] = '\0';
	genMask(temp, bitCount);

	for (int i = 0; i < size; i+=3){
		byte r = originalData[i];
		byte g = originalData[i+1];
		byte b = originalData[i+2];

		byte oldR = r, oldG = g, oldB = b;

		/*r = doMask(r, "00000011");
		g = doMask(g, "00000011");
		b = doMask(b, "00000011");*/

		

		r = doMask(r, temp);
		g = doMask(g, temp);
		b = doMask(b, temp);

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
	
	char mask1[9];
	mask1[8] = '\0';
	genInverseMask(mask1, bitCount);

	char mask2[9];
	mask2[8] = '\0';
	genInverseMask(mask2, 8-bitCount);

	int i;
	for (i = 0; i < size; i += 3){
		byte rDoge = originalData[i];
		byte gDoge = originalData[i + 1];
		byte bDoge = originalData[i + 2];

		byte rMost = doMask(rDoge, mask1) >> (8 - bitCount);
		byte gMost = doMask(gDoge, mask1) >> (8 - bitCount);
		byte bMost = doMask(bDoge, mask1) >> (8 - bitCount);


		byte rtree = doMask(treeData[i],   mask2);
		byte gtree = doMask(treeData[i + 1], mask2);
		byte btree = doMask(treeData[i + 2], mask2);

		result[i] = rMost | rtree;
		result[i+1] = gMost | gtree;
		result[i+2] = bMost | btree;
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