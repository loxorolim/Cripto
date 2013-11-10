#ifndef AES_H
#define AES_H

#define ECB 1
#define CBC 0

typedef unsigned char byte;

byte gmul(byte a, byte b);

void mixColumns(byte* val);
void inverseMixColumns(byte* val);

void subBytes(byte *bytes, int count);
void inverseSubBytes(byte *bytes, int count);

void printMatrix(byte *matrix);

void shiftRows(byte* matrix);
void inverseShiftRows(byte* matrix);

void addRoundKey(byte *state, int startingIndex, byte *subkey);

void addRoundKeyTest();

void makeRoundKey(int roundCount, byte **allKeys);
void makeAllRoundKeys(int roundCount, byte **allKeys, byte *firstKey);
void xor(byte * a, byte * b, byte * result);

void encrypt(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv);
void encryptBlock(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type);
void decrypt(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv);
void decryptBlock(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type);

void matrixTransposer(byte* data);
long calculateOnBits(byte b);
long calculateHammingDistance(byte * clearM, byte * criptoM, int arraySize);
//void byteStuffer(byte * b, int size);

void vigenereCipherEncryption(byte *bytes, byte *key);
void encryptBlockVigenere(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte *vigKey);
void encryptVigenere(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv, byte *vigKey);

void vigenereCipherDecryption(byte *bytes, byte *key);
void decryptBlockVigenere(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte *vigKey);
void decryptVigenere(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv, byte *vigKey);

void columnarTransposition(byte *bytes, byte *result, char *key, int dataSize);
void inverseColumnarTransposition(byte *bytes, byte *result, char *key, int dataSize);

void encryptAddRoundKey(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv);
void decryptAddRoundKey(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv);

void decryptBlockAddRoundKey(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type);
void encryptBlockAddRoundKey(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type);

#endif