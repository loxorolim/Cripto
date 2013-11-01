#ifndef AES_H
#define AES_H

#define EBC 1
#define CBC 0

#define ENCRYPT 1
#define DECRYPT 0

typedef unsigned char byte;

unsigned char gmul(unsigned char a, unsigned char b);
void mixColumns(unsigned char* val, int index);

void subBytes(byte *bytes, int count);

void printMatrix(byte *matrix);

void shiftRows(byte* matrix, int startingIndex);

void addRoundKey(byte *state, int startingIndex, byte *subkey);

void addRoundKeyTest();

void makeRoundKey(int roundCount, byte **allKeys);
void makeAllRoundKeys(int roundCount, byte **allKeys, byte *firstKey);
void xor(byte * a, byte * b, byte * result);
void doInitRound(byte * data, byte * result, byte* key, byte* toXor);
void doRounds(byte * src, byte * dst, int rounds, byte ** allKeys,int mode);
void doFinalRound(byte * src, byte * dst, int rounds, byte ** allKeys,int mode);
void doAES(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type,int mode);
void matrixTransposer(byte* data);
void startAES(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv,int mode);
long calculateOnBits(byte b);
long calculateHammingDistance(byte * clearM, byte * criptoM, int arraySize);

#endif