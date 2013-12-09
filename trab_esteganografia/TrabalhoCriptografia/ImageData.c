//header de definições
#include "ImageData.h"

//headers da biblioteca padrão
#include <stdlib.h>

//biblioteca de imagens
#include <IL/il.h>

ImageData* generateEmptyData(int width, int height, int bpp){
	ImageData* resp = (ImageData*)malloc(sizeof(ImageData));

	resp->width = width;
	resp->height = height;
	resp->bpp = bpp;

	resp->byteCount = width * height * bpp;                     //calcula tamanho de bytes na imagem
	resp->data = (byte*)calloc(resp->byteCount, sizeof(byte)); //aloca espaço para uma cópia dos pixels originais

	return resp;
}

ImageData* generateData(const char *fileName){
	ILuint srcID = ilGenImage();               //gera a imagem de origem
	ilBindImage(srcID);                        //manda trabalhar com essa imagem
	ilLoadImage(fileName);                      //carrega a imagem de origem do arquivo

	int width = ilGetInteger(IL_IMAGE_WIDTH);            //obtém largura da imagem
	int height = ilGetInteger(IL_IMAGE_HEIGHT);          //obtém altura da imagem
	int bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);    //obtém taxa de "bytes per pixel" da imagem

	ImageData *resp = generateEmptyData(width, height, bpp);
	ilCopyPixels(0, 0, 0, resp->width, resp->height, 1, IL_RGB, IL_UNSIGNED_BYTE, resp->data);    //obtém uma cópia dos pixels originais

	ilDeleteImage(srcID);
	return resp;
}

void saveData(ImageData *d, const char *fileName){
	ILuint destID = ilGenImage();                           //gera imagem resultante
	ilBindImage(destID);                                    //manda usar essa imagem
	ilLoadDataL(d->data, d->byteCount, d->width, d->height, 1, d->bpp);       //preenche a imagem resultante com os pixels calculados

	ilSaveImage(fileName);                                  //salva a imagem resultante em disco
	ilDeleteImage(destID);                                   //apaga as imagens
}

void deleteData(ImageData* d){
	free(d->data);
	free(d);
}