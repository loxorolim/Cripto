
#define AES_ADDROUND_H

#include "utils.h"

void encryptAddRoundKey(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv);
void decryptAddRoundKey(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv);

void decryptBlockAddRoundKey(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type);
void encryptBlockAddRoundKey(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type);