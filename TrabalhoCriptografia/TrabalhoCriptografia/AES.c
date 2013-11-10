#include "AES.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>
#include <math.h>

const byte sbox[] =
// 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
{ 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0
0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1
0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2
0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3
0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4
0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5
0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6
0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7
0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8
0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9
0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //A
0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //B
0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //C
0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //D
0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //E
0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }; //F

const byte sbox_inverse[] =
{ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

/* Values used for key schedule */
const byte rcon[] =
{ 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb };

byte gmul(byte a, byte b) {
	byte p = 0;
	int counter;
	byte carry;
	for (counter = 0; counter < 8; counter++) {
	        if (b & 1) 
	                p ^= a;
	        carry = a & 0x80;
	        a <<= 1;
	        if (carry) 
	                a ^= 0x001B;
	        b >>= 1;
	}
	return p;
}

void mixColumns(byte* val)
{
	unsigned char n1 = 0;
	unsigned char n2 = 0;
	unsigned char n3 = 0;
	unsigned char n4 = 0;
	unsigned char n5[] = { 0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0 };


	for (int i = 0; i < 4; i++)
	{

		n1 = gmul(*(val + i + 0), 0x02);
		n2 = gmul(*(val + i + 4), 0x03);
		n3 = gmul(*(val + i + 8), 0x01);
		n4 = gmul(*(val + i + 12), 0x01);

		n5[0 + i] = n1^n2^n3^n4;

		n1 = gmul(*(val + i + 0), 0x01);
		n2 = gmul(*(val + i + 4), 0x02);
		n3 = gmul(*(val + i + 8), 0x03);
		n4 = gmul(*(val + i + 12), 0x01);

		n5[4 + i] = n1^n2^n3^n4;


		n1 = gmul(*(val + i + 0), 0x01);
		n2 = gmul(*(val + i + 4), 0x01);
		n3 = gmul(*(val + i + 8), 0x02);
		n4 = gmul(*(val + i + 12), 0x03);

		n5[8 + i] = n1^n2^n3^n4;


		n1 = gmul(*(val + i + 0), 0x03);
		n2 = gmul(*(val + i + 4), 0x01);
		n3 = gmul(*(val + i + 8), 0x01);
		n4 = gmul(*(val + i + 12), 0x02);

		n5[12 + i] = n1^n2^n3^n4;
	}
	memcpy(val, n5, sizeof(unsigned char)* 16);
}
void inverseMixColumns(byte* val)
{
	unsigned char n1 = 0;
	unsigned char n2 = 0;
	unsigned char n3 = 0;
	unsigned char n4 = 0;
	unsigned char n5[] = { 0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0 };


	for (int i = 0; i < 4; i++)
	{

		n1 = gmul(*(val + i + 0), 14);
		n2 = gmul(*(val + i + 4), 11);
		n3 = gmul(*(val + i + 8), 13);
		n4 = gmul(*(val + i + 12), 9);

		n5[0 + i] = n1^n2^n3^n4;

		n1 = gmul(*(val + i + 0), 9);
		n2 = gmul(*(val + i + 4), 14);
		n3 = gmul(*(val + i + 8), 11);
		n4 = gmul(*(val + i + 12), 13);

		n5[4 + i] = n1^n2^n3^n4;


		n1 = gmul(*(val + i + 0), 13);
		n2 = gmul(*(val + i + 4), 9);
		n3 = gmul(*(val + i + 8), 14);
		n4 = gmul(*(val + i + 12), 11);

		n5[8 + i] = n1^n2^n3^n4;


		n1 = gmul(*(val + i + 0), 11);
		n2 = gmul(*(val + i + 4), 13);
		n3 = gmul(*(val + i + 8), 9);
		n4 = gmul(*(val + i + 12), 14);

		n5[12 + i] = n1^n2^n3^n4;
	}
	memcpy(val, n5, sizeof(unsigned char)* 16);
}

void subBytes(byte *bytes, int count){
	int i;
	for (i = 0; i < count; i++)
		bytes[i] = sbox[bytes[i]];
}
void inverseSubBytes(byte *bytes, int count){
	int i;
	for (i = 0; i < count; i++)
		bytes[i] = sbox_inverse[bytes[i]];
}

void printMatrix(byte *matrix){
	int i, j;
	for (i = 0; i < 4; i++){
		for (j = 0; j < 4; j++){
			printf("%x ", matrix[i * 4 + j]);
		}
		printf("\n");
	}
}
void inverseShiftRows(byte* matrix){
	int i, j;
	byte temp[16];

	for (i = 0; i < 4; i++){
		for (j = 0; j < 4; j++){
			int newJ = (j - i) % 4;
			if(newJ < 0)
				newJ += 4;
			temp[4 * i + j] = matrix[4 * i + newJ];
		}
	}

	memcpy(matrix, temp, 16);
}

void shiftRows(byte* matrix){
	int i, j;
	byte temp[16];

	for (i = 0; i < 4; i++){
		for (j = 0; j < 4; j++){
			int newJ = (j + i) % 4;
			temp[4 * i + j] = matrix[4 * i + newJ];
		}
	}

	memcpy(matrix, temp, 16);
}

void addRoundKey(byte *state, int startingIndex, byte *subkey){
	int i;
	for (i = 0; i < 16; i++)
		state[startingIndex + i] = state[startingIndex + i] ^ subkey[i];
}

void makeRoundKey(int roundCount, byte **allKeys)
{

	byte *lastKey = allKeys[roundCount - 1];
	byte *currentKey = allKeys[roundCount];

	byte lastColumn[4];
	int i, j;
	for (j = 0; j < 4; j++){
		if (j == 0){
			byte oldColumn[] = { lastKey[3], lastKey[7], lastKey[11], lastKey[15] };
			byte rotatedColumn[] = { oldColumn[1], oldColumn[2], oldColumn[3], oldColumn[0] };
			subBytes(rotatedColumn, 4);

			for (int i = 0; i < 4; i++){
				byte rconValue = i == 0 ? rcon[roundCount] : 0;
				lastColumn[i] = currentKey[i * 4 + j] = lastKey[i * 4 + j] ^ rotatedColumn[i] ^ rconValue;
			}
		}
		else {
			for (int i = 0; i < 4; i++)
				lastColumn[i] = currentKey[i * 4 + j] = lastKey[i * 4 + j] ^ lastColumn[i];
		}
	}
}

void makeAllRoundKeys(int roundCount, byte **allKeys, byte *firstKey)
{
	memcpy(allKeys[0], firstKey, 16);

	for (int i = 0; i < roundCount; i++)
	{
		makeRoundKey(i + 1, allKeys);
	}
}

void xor(byte * a, byte * b, byte * result)
{
	for (int i = 0; i < 16; i++)
	{
		result[i] = a[i] ^ b[i];
	}
}

void encryptBlock(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type)
{
	if (type == CBC)
		xor(data, *toXor, result);
	else
		memcpy(result, data, 16 * sizeof(byte));

	//Primeiro Round
	addRoundKey(result, 0, allKeys[0]);

	//demais rounds
	for (int i = 1; i < rounds; i++)
	{
		subBytes(result, 16);
		shiftRows(result);
		mixColumns(result);

		addRoundKey(result, 0, allKeys[i]);
	}

	//�ltimo round
	subBytes(result, 16);
	shiftRows(result);
	addRoundKey(result, 0, allKeys[rounds]);

	if (type == CBC) // SE CBC
		*toXor = result;
}

void decryptBlock(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type)
{
	memcpy(result, data, 16 * sizeof(byte));

	//Primeiro round
	addRoundKey(result, 0, allKeys[rounds]);
	inverseShiftRows(result);
	inverseSubBytes(result, 16);

	//demais rounds
	for (int i = rounds - 1; i >= 1; i--){
		addRoundKey(result, 0, allKeys[i]);
		inverseMixColumns(result);
		inverseShiftRows(result);
		inverseSubBytes(result, 16);
	}

	//�ltimo round
	addRoundKey(result, 0, allKeys[0]);

	if (type == CBC){
		xor(result, *toXor, result);
		*toXor = data;
	}	
}

void matrixTransposer(byte* data)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < i; j++)
		{
			byte aux = data[i * 4 + j];
			data[i * 4 + j] = data[j * 4 + i];
			data[j * 4 + i] = aux;
		}
	}
}

void encrypt(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv)
{
	matrixTransposer(key);

	byte **allKeys = (byte**)calloc(rounds + 1, sizeof(byte*));
	for (int i = 0; i < rounds + 1; i++)
		allKeys[i] = (byte*)calloc(16, sizeof(byte));
	makeAllRoundKeys(rounds, allKeys, key);

	byte* toXor = NULL;
	if (type == CBC) //CBC
	{
		matrixTransposer(iv);
		toXor = iv;
	}

	for (int i = 0; i < dataSize / 16; i++)
	{
		matrixTransposer(data + i * 16);
		encryptBlock(data + i * 16, allKeys, rounds, &toXor, result + i * 16, type);
	}

	//transpor os dados de volta ===============================================================
	for (int i = 0; i < dataSize / 16; i++)
	{
		matrixTransposer(data + i * 16);
		matrixTransposer(result + i * 16);
	}

	matrixTransposer(key);
	if (type == CBC)
		matrixTransposer(iv);
	//fim transposi��o =========================================================================

	for (int i = 0; i < rounds + 1; i++)
		free(allKeys[i]);

	
	free(allKeys);
}

void decrypt(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv)
{
	matrixTransposer(key);

	byte **allKeys = (byte**)calloc(rounds + 1, sizeof(byte*));
	for (int i = 0; i < rounds + 1; i++)
		allKeys[i] = (byte*)calloc(16, sizeof(byte));
	makeAllRoundKeys(rounds, allKeys, key);

	byte* toXor = NULL;
	if (type == CBC) //CBC
	{
		matrixTransposer(iv);
		toXor = iv;
	}

	for (int i = 0; i < dataSize / 16; i++)
	{
		matrixTransposer(data + i * 16);
		decryptBlock(data + i * 16, allKeys, rounds, &toXor, result + i * 16, type);
	}

	for (int i = 0; i < dataSize / 16; i++)
	{
		matrixTransposer(data + i * 16);
		matrixTransposer(result + i * 16);
		//printf("\nRESULTADO FINAL EM NOSSO FORMATO\n");
		//printMatrix(result + i * 16);
	}

	for (int i = 0; i < rounds + 1; i++)
		free(allKeys[i]);

	
	free(allKeys);
}

long calculateOnBits(byte b)
{
	long count = 0;
	for (int i = 0; i<8; i++)
	{
		byte mask = 1 << i;
		if ((mask  & b) != 0)
			count++;
	}
	return count;
}

long calculateHammingDistance(byte * clearM, byte * criptoM, int arraySize)
{
	long dist = 0;

	for (int i = 0; i < arraySize; i++)
	{
		byte xor = clearM[i] ^ criptoM[i];
		dist += calculateOnBits(xor);
	}
	return dist;
}
byte* byteStuffer(byte * b, int size, int* newSize)
{
	int toStuff = size % 16;
	byte *newBytes = (byte*)calloc(size + toStuff, sizeof(byte*));
	memcpy(newBytes,b,size*sizeof(byte));
	if(toStuff != 0)
	{
		for(int i = 1; i <= toStuff;i++)
		{
			newBytes[size+i] = 0;
		}
	}
	*newSize =  size + toStuff;
	return newBytes;

}

void columnarTransposition(byte *bytes, char *key, int dataSize){
	int const size = strlen(key);
	int* order = (int*)calloc(size, sizeof(int));
	for (int i = 0; i <= size; i++){
		for (int j = 0; j < size; j++){
			if ((key[i] == key[j] && i < j) || (key[i] > key[j]))
				order[i]++;
		}
	}
	//for (int i = 0; i < size; i++){
	//	printf("%d", order[i]);
	//}
	//printf("\n");
	int residue = dataSize % size;
	int rowsCount = ceil((float)dataSize / (float)size);
	int resultSize = size * rowsCount;

	byte* result = (byte*)calloc(resultSize, sizeof(byte));

	int target = 0;
	int stop = 0;
	while (!stop){
		for (int i = 0; i < size; i++){
			if (order[i] == target){
				for (int j = 0; j < rowsCount; j++){
					if (j < rowsCount-1 || residue == 0)
						result[target + j * size] = bytes[i + j * size];
					else if (i < residue)
						result[target + j * size] = bytes[i + j * size];
					else
						result[target + j * size] = 0xFF;
				}
				target++;
				break;
			}
		}
		if (target >= size)
			stop = 1;
	}
	free(order);
	memcpy(bytes, result, resultSize);
	free(result);
}

void inverseColumnarTransposition(byte *bytes, char *key, int dataSize){
	int const size = strlen(key);
	int* order = (int*)calloc(size, sizeof(int));
	for (int i = 0; i <= size; i++){
		for (int j = 0; j < size; j++){
			if ((key[i] == key[j] && i < j) || (key[i] > key[j]))
				order[i]++;
		}
	}
	//for (int i = 0; i < size; i++){
	//	printf("%d", order[i]);
	//}
	//printf("\n");
	int residue = dataSize % size;
	int keyLen = strlen(key);
	int rowsCount = ceil((float)dataSize / (float)keyLen);
	int resultSize = keyLen * rowsCount;

	byte* result = (byte*)calloc(resultSize, sizeof(byte));
	int targetI = 0;
	int target = order[targetI];
	int stop = 0;
	while (!stop){
		for (int i = 0; i < size; i++){
			if (i == target){
				for (int j = 0; j < rowsCount; j++){
					if (i < residue && targetI)
						*(result + targetI + j * size) = *(bytes + i + j * size);
					else
						*(result + targetI + j * size) = *(bytes + i + j * size);
				}
				targetI++;
				target = order[targetI];
				break;
			}
		}
		if (targetI >= size)
			stop = 1;
	}
	free(order);
	memcpy(bytes, result, resultSize);
	free(result);
}
void vigenereCipherEncryption(byte *bytes, byte *key){
	for (int i = 0; i < 16; i++)
		bytes[i] = (bytes[i] + key[i]) % 256;
}

void encryptBlockAlternative(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type, byte *vigKey, char *transKey)
{
	if (type == CBC)
		xor(data, *toXor, result);
	else
		memcpy(result, data, 16 * sizeof(byte));

	//Primeiro Round
	addRoundKey(result, 0, allKeys[0]);

	//demais rounds
	for (int i = 1; i < rounds; i++)
	{
			vigenereCipherEncryption(result, vigKey);
			columnarTransposition(result, transKey, 16);
			//shiftRows(result);
			//mixColumns(result);

			addRoundKey(result, 0, allKeys[i]);
	}

	//�ltimo round
	vigenereCipherEncryption(result, vigKey);
	//shiftRows(result);
	addRoundKey(result, 0, allKeys[rounds]);

	if (type == CBC) // SE CBC
		*toXor = result;
}

void encryptAlternative(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv, byte *vigKey, char *transKey){
	matrixTransposer(key);
	matrixTransposer(vigKey);

	byte **allKeys = (byte**)calloc(rounds + 1, sizeof(byte*));
	for (int i = 0; i < rounds + 1; i++)
		allKeys[i] = (byte*)calloc(16, sizeof(byte));
	makeAllRoundKeys(rounds, allKeys, key);

	byte* toXor = NULL;
	if (type == CBC) //CBC
	{
		matrixTransposer(iv);
		toXor = iv;
	}

	for (int i = 0; i < dataSize / 16; i++)
	{
		matrixTransposer(data + i * 16);
		encryptBlockAlternative(data + i * 16, allKeys, rounds, &toXor, result + i * 16, type, vigKey, transKey);
	}

	//transpor os dados de volta ===============================================================
	for (int i = 0; i < dataSize / 16; i++)
	{
		matrixTransposer(data + i * 16);
		matrixTransposer(result + i * 16);
	}

	matrixTransposer(vigKey);
	matrixTransposer(key);
	if (type == CBC)
		matrixTransposer(iv);
	//fim transposi��o =========================================================================

	for (int i = 0; i < rounds + 1; i++)
		free(allKeys[i]);

	free(allKeys);
}

void vigenereCipherDecryption(byte *bytes, byte *key){
	for (int i = 0; i < 16; i++)
		bytes[i] = (bytes[i] - key[i]) % 256;
}

void decryptBlockAlternative(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type, byte* vigKey, char* transKey)
{
	memcpy(result, data, 16 * sizeof(byte));
	int dataSize = sizeof(result) / sizeof(*result);

	//Primeiro round
	addRoundKey(result, 0, allKeys[rounds]);
	vigenereCipherDecryption(result, vigKey);

	//demais rounds
	for (int i = rounds - 1; i >= 1; i--){
		addRoundKey(result, 0, allKeys[i]);
		inverseColumnarTransposition(result, transKey, 16);
		vigenereCipherDecryption(result, vigKey);
	}

	//�ltimo round
	addRoundKey(result, 0, allKeys[0]);

	if (type == CBC){
		xor(result, *toXor, result);
		*toXor = data;
	}
}

void decryptAlternative(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv, byte* vigKey, char* transKey)
{
	matrixTransposer(key);
	matrixTransposer(vigKey);

	byte **allKeys = (byte**)calloc(rounds + 1, sizeof(byte*));
	for (int i = 0; i < rounds + 1; i++)
		allKeys[i] = (byte*)calloc(16, sizeof(byte));
	makeAllRoundKeys(rounds, allKeys, key);

	byte* toXor = NULL;
	if (type == CBC) //CBC
	{
		matrixTransposer(iv);
		toXor = iv;
	}

	for (int i = 0; i < dataSize / 16; i++)
	{
		matrixTransposer(data + i * 16);
		decryptBlockAlternative(data + i * 16, allKeys, rounds, &toXor, result + i * 16, type, vigKey, transKey);
	}

	for (int i = 0; i < dataSize / 16; i++)
	{
		matrixTransposer(data + i * 16);
		matrixTransposer(result + i * 16);
		//printf("\nRESULTADO FINAL EM NOSSO FORMATO\n");
		//printMatrix(result + i * 16);
	}

	for (int i = 0; i < rounds + 1; i++)
		free(allKeys[i]);

	free(allKeys);
}

void encryptBlockAddRoundKey(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type)
{
	if (type == CBC)
		xor(data, *toXor, result);
	else
		memcpy(result, data, 16 * sizeof(byte));

	//Primeiro Round
	addRoundKey(result, 0, allKeys[0]);

	//demais rounds
	for (int i = 1; i < rounds; i++)
	{
//		subBytes(result, 16);
//		shiftRows(result);
//		mixColumns(result);

		addRoundKey(result, 0, allKeys[i]);
	}

	//�ltimo round
//	subBytes(result, 16);
//	shiftRows(result);
	addRoundKey(result, 0, allKeys[rounds]);

	if (type == CBC) // SE CBC
		*toXor = result;
}

void encryptAddRoundKey(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv)
{
	matrixTransposer(key);

	byte **allKeys = (byte**)calloc(rounds + 1, sizeof(byte*));
	for (int i = 0; i < rounds + 1; i++)
		allKeys[i] = (byte*)calloc(16, sizeof(byte));
	makeAllRoundKeys(rounds, allKeys, key);

	byte* toXor = NULL;
	if (type == CBC) //CBC
	{
		matrixTransposer(iv);
		toXor = iv;
	}

	for (int i = 0; i < dataSize / 16; i++)
	{
		matrixTransposer(data + i * 16);
	//	encryptBlock(data + i * 16, allKeys, rounds, &toXor, result + i * 16, type);
		encryptBlockAddRoundKey(data + i * 16, allKeys, rounds, &toXor, result + i * 16, type);
	}

	//transpor os dados de volta ===============================================================
	for (int i = 0; i < dataSize / 16; i++)
	{
		matrixTransposer(data + i * 16);
		matrixTransposer(result + i * 16);
	}

	matrixTransposer(key);
	if (type == CBC)
		matrixTransposer(iv);
	//fim transposi��o =========================================================================

	for (int i = 0; i < rounds + 1; i++)
		free(allKeys[i]);


	free(allKeys);
}
void decryptBlockAddRoundKey(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type)
{
	memcpy(result, data, 16 * sizeof(byte));

	//Primeiro round
	addRoundKey(result, 0, allKeys[rounds]);
//	inverseShiftRows(result);
//	inverseSubBytes(result, 16);

	//demais rounds
	for (int i = rounds - 1; i >= 1; i--){
		addRoundKey(result, 0, allKeys[i]);
//		inverseMixColumns(result);
//		inverseShiftRows(result);
//		inverseSubBytes(result, 16);
	}

	//�ltimo round
	addRoundKey(result, 0, allKeys[0]);

	if (type == CBC){
		xor(result, *toXor, result);
		*toXor = data;
	}
}
void decryptAddRoundKey(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv)
{
	matrixTransposer(key);

	byte **allKeys = (byte**)calloc(rounds + 1, sizeof(byte*));
	for (int i = 0; i < rounds + 1; i++)
		allKeys[i] = (byte*)calloc(16, sizeof(byte));
	makeAllRoundKeys(rounds, allKeys, key);

	byte* toXor = NULL;
	if (type == CBC) //CBC
	{
		matrixTransposer(iv);
		toXor = iv;
	}

	for (int i = 0; i < dataSize / 16; i++)
	{
		matrixTransposer(data + i * 16);
		decryptBlockAddRoundKey(data + i * 16, allKeys, rounds, &toXor, result + i * 16, type);
	}

	for (int i = 0; i < dataSize / 16; i++)
	{
		matrixTransposer(data + i * 16);
		matrixTransposer(result + i * 16);
		//printf("\nRESULTADO FINAL EM NOSSO FORMATO\n");
		//printMatrix(result + i * 16);
	}

	for (int i = 0; i < rounds + 1; i++)
		free(allKeys[i]);


	free(allKeys);
}