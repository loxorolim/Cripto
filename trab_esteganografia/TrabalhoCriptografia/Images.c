//Bibliotecas padrão
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Biblioteca de images
#include <IL/il.h>

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

	int i;
	for (i = 0; i < size - 2; i+=3){
		byte r = originalData[i];
		byte g = originalData[i+1];
		byte b = originalData[i+2];

		r = r & mask;
		g = g & mask;
		b = b & mask;

		result[i]     = normalizeColor(r, bitCount);
		result[i + 1] = normalizeColor(g, bitCount);
		result[i + 2] = normalizeColor(b, bitCount);
	}


	ILuint destID = ilGenImage();                           //gera imagem resultante
	ilBindImage(destID);                                    //manda usar essa imagem
	ilLoadDataL(result, size, width, height, 1, bpp);       //preenche a imagem resultante com os pixels calculados
	
	ilSaveImage(destFile);                                  //salva a imagem resultante em disco
	
	ilDeleteImage(srcID);                                   //apaga as duas imagens
	ilDeleteImage(destID);

	free(result);                                           //libera a memória dos pixels processados
	free(originalData);
}

void encrypt(const char* imageToShow, const char* imageToHide, const char* destImage, int bitCount){
	//CARREGAMENTO DA IMAGEM SECRETA ----------------------------------------------------------------------------
	ILuint secretImg = ilGenImage();               //gera a imagem de origem
	ilBindImage(secretImg);                        //manda trabalhar com essa imagem
	ilLoadImage(imageToHide);                      //carrega a imagem de origem do arquivo

	int width = ilGetInteger(IL_IMAGE_WIDTH);            //obtém largura da imagem
	int height = ilGetInteger(IL_IMAGE_HEIGHT);          //obtém altura da imagem
	int bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);    //obtém taxa de "bytes per pixel" da imagem

	const int size = width * height * bpp;                     //calcula tamanho de bytes na imagem

	
	byte *secretData = (byte*)calloc(size, sizeof(byte)); 
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, secretData);    
	ilDeleteImage(secretImg);

	//CARREGAMENTO DA IMAGEM EXIBIDA ----------------------------------------------------------------------------
	ILuint shownImg = ilGenImage();
	ilBindImage(shownImg);
	ilLoadImage(imageToShow);
	byte *shownData = (byte*)calloc(size, sizeof(byte)); //aloca espaço para uma cópia dos pixels originais
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, shownData);    //obtém uma cópia dos pixels originais
	ilDeleteImage(shownImg);

	

	//PROCESSAMENTO --------------------------------------------------------------------------------------------
	byte *result = (byte*)calloc(size, sizeof(byte));       //aloca os pixels da imagem resultado

	byte mask1 = genInverseMask(8 - bitCount);
	byte mask2 = genInverseMask(bitCount);

	int i;
	for (i = 0; i < size - 2; i += 3){
		byte rDoge = secretData[i];
		byte gDoge = secretData[i + 1];
		byte bDoge = secretData[i + 2];

		byte rMost = (rDoge & mask1) >> (8 - bitCount); //pega os (8-n) bits mais significativos e move pra direita
		byte gMost = (gDoge & mask1) >> (8 - bitCount);
		byte bMost = (bDoge & mask1) >> (8 - bitCount);


		byte rShown = shownData[i] & mask2; //pega os n bits mais significativos e deixa onde estão (na esquerda)
		byte gShown = shownData[i + 1] & mask2;
		byte bShown = shownData[i + 2] & mask2;

		result[i] = rShown | rMost; //junta os n bits da imagem auxiliar com os (8-n) da mensagem
		result[i + 1] = gShown | gMost;
		result[i + 2] = bShown | bMost;
	}

	ILuint destID = ilGenImage();                           //gera imagem resultante
	ilBindImage(destID);                                    //manda usar essa imagem
	ilLoadDataL(result, size, width, height, 1, bpp);       //preenche a imagem resultante com os pixels calculados

	ilSaveImage(destImage);                                  //salva a imagem resultante em disco
	ilDeleteImage(destID);                                   //apaga as imagens

	free(result);                                           //libera a memória dos pixels processados
	free(secretData);
	free(shownData);
}