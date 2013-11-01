#ifndef IMAGES_H
#define IMAGES_H

void encryptImage(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type);
void decryptImage(const char* srcFile, const char* destFile, byte *key, int rounds, byte *iv, int type);

#endif