#include "Tests.h"
#include "AES.h"

#include <stdio.h>
#include <stdlib.h>

void shiftRowsTest(){
	byte mat[] = {
		0xd4, 0xe0, 0xb8, 0x1e,
		0x27, 0xbf, 0xb4, 0x41,
		0x11, 0x98, 0x5d, 0x52,
		0xae, 0xf1, 0xe5, 0x30
	};
	shiftRows(mat);
	printMatrix(mat);
}

void inverseShiftRowsTest(){
	printf("Inverse shift rows test\n");
	byte mat[] = {
		0xd4, 0xe0, 0xb8, 0x1e,
		0x27, 0xbf, 0xb4, 0x41,
		0x11, 0x98, 0x5d, 0x52,
		0xae, 0xf1, 0xe5, 0x30
	};
	shiftRows(mat);
	printMatrix(mat);
	printf("\n");
	inverseShiftRows(mat);
	printMatrix(mat);
}

void addRoundKeyTest(){
	byte mat[] = {
		0x04, 0xe0, 0x48, 0x28,
		0x66, 0xcb, 0xf8, 0x06,
		0x81, 0x19, 0xd3, 0x26,
		0xe5, 0x9a, 0x7a, 0x4c
	};

	byte subkey[] = {
		0xa0, 0x88, 0x23, 0x2a,
		0xfa, 0x54, 0xa3, 0x6c,
		0xfe, 0x2c, 0x39, 0x76,
		0x17, 0xb1, 0x39, 0x05
	};
	addRoundKey(mat, 0, subkey);
	printMatrix(mat);
}

void testGenerateRoundKeys(){
	byte originalKey[] = {
		0x2b, 0x28, 0xab, 0x09,
		0x7e, 0xae, 0xf7, 0xcf,
		0x15, 0xd2, 0x15, 0x4f,
		0x16, 0xa6, 0x88, 0x3c
	};
	byte *allKeys[4];
	allKeys[0] = originalKey;
	int i;
	for (i = 1; i < 4; i++)
		allKeys[i] = (byte*)calloc(16, sizeof(byte));

	for (i = 1; i < 4; i++){
		//	makeAllRoundKeys(i, allKeys);
		printf("\nRound %d:\n", i);
		printMatrix(allKeys[i]);

	}



	for (i = 1; i < 4; i++)
		free(allKeys[i]);
}

void testMixColumns(){
	unsigned char teste[] = { 0xd4, 0xe0, 0xb8, 0x1e, 0xbf, 0xb4, 0x41, 0x27, 0x5d, 0x52, 0x11, 0x98, 0x30, 0xae, 0xf1, 0xe5 };
	
	printMatrix(teste);
	mixColumns(teste);
	printMatrix(teste);
}

void testInverseMixColumns(){
	printf("Inverse mix columns test\n");

	byte teste[] = { 0xd4, 0xe0, 0xb8, 0x1e, 0xbf, 0xb4, 0x41, 0x27, 0x5d, 0x52, 0x11, 0x98, 0x30, 0xae, 0xf1, 0xe5 };
	printf("Original:\n");
	printMatrix(teste);

	mixColumns(teste);
	printf("\nMixed:\n");
	printMatrix(teste);

	inverseMixColumns(teste);
	printf("\nDemixed:\n");
	printMatrix(teste);
}

void testeBoladoEBC(int mode)
{
	byte originalKey[] = {
		0x2b, 0x28, 0xab, 0x09,
		0x7e, 0xae, 0xf7, 0xcf,
		0x15, 0xd2, 0x15, 0x4f,
		0x16, 0xa6, 0x88, 0x3c
	};

	byte inputData[32] = { 0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37, 0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34, 0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37, 0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34 };
	byte result[32];

	startAES(inputData, 32, originalKey, result, 10, EBC, NULL,mode);

	//printMatrix(inputData,0);

	//printMatrix(result,0);
}
void testeBoladoCBC(int mode)
{
	byte originalKey[] = {
		0x2b, 0x28, 0xab, 0x09,
		0x7e, 0xae, 0xf7, 0xcf,
		0x15, 0xd2, 0x15, 0x4f,
		0x16, 0xa6, 0x88, 0x3c
	};
	byte iv[16] = { 0x59, 0xA1, 0xD3, 0x3E, 0xCB, 0x37, 0x56, 0x0B, 0x7E, 0x3D, 0xD3, 0xA2, 0x41, 0x8A, 0x1A, 0x2C };
	byte inputData[32] = { 0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37, 0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34,
		0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37, 0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34 };
	byte result[32];

	startAES(inputData, 32, originalKey, result, 10, CBC, iv,mode);

	//printMatrix(inputData,0);

	//printMatrix(result,0);
}
void testeBoladoEBCDecrypt()
{
	byte originalKey[] = {
		0x2b, 0x28, 0xab, 0x09,
		0x7e, 0xae, 0xf7, 0xcf,
		0x15, 0xd2, 0x15, 0x4f,
		0x16, 0xa6, 0x88, 0x3c
	};

	byte inputData[16] = { 0x57, 0x16, 0xAA, 0xFA, 0x2C, 0xC6, 0x8B, 0x9B, 0x8B, 0x9B, 0xE5, 0x0D, 0x30, 0xE3, 0xF2, 0x06 };
	byte result[16];

	startAES(inputData, 16, originalKey, result, 10, EBC, NULL,DECRYPT);
}