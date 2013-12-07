#ifndef IMAGES_H
#define IMAGES_H

#include "Images.h"
#include <IL/il.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

//typedef void(*CryptFunc)(byte*, int, byte*, byte*, int, int, byte*);


void encrypt(const char*imageToShow, const char* imageToHide, const char* destImage, int bitCount);
void decrypt(const char* srcFile, const char* destFile, int bitCount);



/*
para n=2, retorna 00000011
*/
byte genMask(int count);

/*
para n=2, retorna 11111100
*/
byte genInverseMask(int count);
#endif