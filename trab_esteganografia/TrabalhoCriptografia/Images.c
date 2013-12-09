	//Bibliotecas padrão
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Biblioteca de images
#include "ImageData.h"

//Nossos headers
#include "Images.h"

byte normalizeColor(byte original, int bitCount){
	float MAXIMUM = (float)(pow(2, bitCount) - 1.0);
	float pctg = original / MAXIMUM;
	return (byte)(pctg * 255.0f);
}

byte genInverseMask(int count){
	return genMask(count) ^ 0xFF;
}

byte genMask(int count){
	const byte masks[9] = {
		0,   //00000000
		1,   //00000001
		3,   //00000011
		7,   //00000111
		15,  //00001111
		31,  //00011111
		63,  //00111111
		127, //01111111
		255  //11111111
	};
	return masks[count];
}

void decrypt(const char* srcFile, const char* destFile, int bitCount){
	ImageData *original = generateData(srcFile);
	ImageData *result = generateEmptyData(original->width, original->height, original->bpp);

	byte mask = genMask(bitCount);

	int i, size = original->byteCount;
	for (i = 0; i < size - 2; i+=3){
		byte r = original->data[i];
		byte g = original->data[i + 1];
		byte b = original->data[i + 2];

		r = r & mask;
		g = g & mask;
		b = b & mask;

		result->data[i]     = normalizeColor(r, bitCount);
		result->data[i + 1] = normalizeColor(g, bitCount);
		result->data[i + 2] = normalizeColor(b, bitCount);
	}                             //salva a imagem resultante em disco
	saveData(result, destFile);

	deleteData(original);
	deleteData(result);
}

void encrypt(const char* imageToShow, const char* imageToHide, const char* destImage, int bitCount){
	//CARREGAMENTO DA IMAGEM SECRETA ----------------------------------------------------------------------------
	ImageData* secretImg = generateData(imageToHide);

	//CARREGAMENTO DA IMAGEM EXIBIDA ----------------------------------------------------------------------------
	ImageData* shownImg = generateData(imageToShow);

	//PROCESSAMENTO --------------------------------------------------------------------------------------------
	int size = shownImg->byteCount;
	ImageData *result = generateEmptyData(shownImg->width, shownImg->height, shownImg->bpp);

	byte mask1 = genInverseMask(8 - bitCount);
	byte mask2 = genInverseMask(bitCount);

	int i;
	for (i = 0; i < size - 2; i += 3){
		byte rDoge = secretImg->data[i];
		byte gDoge = secretImg->data[i + 1];
		byte bDoge = secretImg->data[i + 2];

		byte rMost = (rDoge & mask1) >> (8 - bitCount); //pega os (8-n) bits mais significativos e move pra direita
		byte gMost = (gDoge & mask1) >> (8 - bitCount);
		byte bMost = (bDoge & mask1) >> (8 - bitCount);


		byte rShown = (shownImg->data[i]) & mask2; //pega os n bits mais significativos e deixa onde estão (na esquerda)
		byte gShown = (shownImg->data[i + 1]) & mask2;
		byte bShown = (shownImg->data[i + 2]) & mask2;

		result->data[i] = rShown | rMost; //junta os n bits da imagem auxiliar com os (8-n) da mensagem
		result->data[i + 1] = gShown | gMost;
		result->data[i + 2] = bShown | bMost;
	}

	saveData(result, destImage);

	deleteData(result);
	deleteData(shownImg);
	deleteData(secretImg);
}