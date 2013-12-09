#ifndef IMAGES_H
#define IMAGES_H

#include "utils.h"


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