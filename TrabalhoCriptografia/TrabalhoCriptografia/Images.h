#ifndef IMAGES_H
#define IMAGES_H

#include "AES.h"
#include "Images.h"
#include <IL/il.h>
#include <stdlib.h>
#include <string.h>

typedef void(*CryptFunc)(byte*, int, byte*, byte*, int, int, byte*);

typedef unsigned char byte;

void process(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type, CryptFunc f);
void encryptImage(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type);
void decryptImage(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type);

#endif