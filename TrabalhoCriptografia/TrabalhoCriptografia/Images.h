#ifndef IMAGES_H
#define IMAGES_H

#include "AES.h"
#include "Images.h"
#include <IL/il.h>
#include <stdlib.h>
#include <string.h>

typedef void(*CryptFunc)(byte*, int, byte*, byte*, int, int, byte*);

typedef unsigned char byte;

long process(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type, CryptFunc f);

//Encripta uma imagem e retorna a distância de Hamming
long encryptImage(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type);

//Decrypta uma imagem
void decryptImage(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type);

#endif