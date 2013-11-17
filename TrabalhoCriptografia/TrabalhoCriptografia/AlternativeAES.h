#ifndef AES_ALTERNATIVE_H
#define AES_ALTERANTIVE_H

#include "utils.h"

char* getTransKeyFromFile();
byte* getVigKeyFromFile(int* size);

void encryptBlockAlternative(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type, byte *vigKey, char *transKey, int vigKeysize);
void encryptAlternative(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv, int vigKeySize);

void decryptBlockAlternative(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type, byte* vigKey, char* transKey, int vigKeySize);
void decryptAlternative(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv, int vigKeySize);

void vigenereCipherEncryption(byte *bytes, byte *key, int size);
void vigenereCipherDecryption(byte *bytes, byte *key, int size);

void columnarTransposition(byte *bytes, char *key, int dataSize);
void inverseColumnarTransposition(byte *bytes, char *key, int dataSize);

#endif