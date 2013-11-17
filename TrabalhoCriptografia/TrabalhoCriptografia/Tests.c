#include "Tests.h"
#include "AES.h"
#include "AlternativeAES.h"
#include "Images.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"

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

void testeBoladoECB(int mode)
{
	byte originalKey[] = {
		0x2b, 0x28, 0xab, 0x09,
		0x7e, 0xae, 0xf7, 0xcf,
		0x15, 0xd2, 0x15, 0x4f,
		0x16, 0xa6, 0x88, 0x3c
	};

	byte inputData[32] = { 0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37, 0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34, 0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37, 0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34 };
	byte result[32];

	encrypt(inputData, 32, originalKey, result, 10, ECB, NULL);

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
	byte iv[16] = { 0x2e, 0xc5, 0x31, 0x5F, 0xAA, 0xF5, 0xA7, 0x4E, 0x7D, 0x6E, 0xD2, 0x0A, 0x1E, 0x03, 0x25, 0x0F };
	byte inputData[32] = { 0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37, 0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34,
		0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37, 0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34 };
	byte result[32];

	encrypt(inputData, 32, originalKey, result, 10, CBC, iv);

	//printMatrix(inputData,0);

	//printMatrix(result,0);
}
void testeBoladoECBDecrypt()
{
	byte originalKey[] = {
		0x2b, 0x28, 0xab, 0x09,
		0x7e, 0xae, 0xf7, 0xcf,
		0x15, 0xd2, 0x15, 0x4f,
		0x16, 0xa6, 0x88, 0x3c
	};

	byte inputData[16] = { 0x57, 0x16, 0xAA, 0xFA, 0x2C, 0xC6, 0x8B, 0x9B, 0x8B, 0x9B, 0xE5, 0x0D, 0x30, 0xE3, 0xF2, 0x06 };
	byte result[16];

	decrypt(inputData, 16, originalKey, result, 10, ECB, NULL);
}

void testeBoladoCBCDecrypt()
{
	byte originalKey[] = {
		0x2b, 0x28, 0xab, 0x09,
		0x7e, 0xae, 0xf7, 0xcf,
		0x15, 0xd2, 0x15, 0x4f,
		0x16, 0xa6, 0x88, 0x3c
	};

	byte iv[16] = { 0xB4, 0xCA, 0xD2, 0x45, 0x0E, 0x6F, 0x11, 0x20, 0x33, 0x34, 0xDD, 0x81, 0x47, 0x81, 0x13, 0x35 };
	byte inputData[32] = { 0xE8, 0xDF, 0xB5, 0x6C, 0xE3, 0x34, 0x6B, 0x58, 0x5D, 0xFE, 0xEB, 0x12, 0xCA, 0x70, 0x40, 0xF7,
		0xD5, 0x78, 0x05, 0xD9, 0xC6, 0x3C, 0xCC, 0x36, 0x86, 0xA3, 0x71, 0xB7, 0x96, 0x39, 0x89, 0x21 };
	byte result[32];

	decrypt(inputData, 32, originalKey, result, 10, CBC, iv);
}

void inverseSubBytesTest(){
	printf("Inverse subbytes test\n");

	byte teste[] = { 0xd4, 0xe0, 0xb8, 0x1e, 0xbf, 0xb4, 0x41, 0x27, 0x5d, 0x52, 0x11, 0x98, 0x30, 0xae, 0xf1, 0xe5 };
	printf("Original:\n");
	printMatrix(teste);

	subBytes(teste, 16);
	printf("\nSubbed:\n");
	printMatrix(teste);

	inverseSubBytes(teste, 16);
	printf("\nDeSubbed:\n");
	printMatrix(teste);
}

void imageECBTest(){
	const char* dest = "kocada.bmp";
	
	byte key[] = {
		0x2b, 0x28, 0xab, 0x09,
		0x7e, 0xae, 0xf7, 0xcf,
		0x15, 0xd2, 0x15, 0x4f,
		0x16, 0xa6, 0x88, 0x3c
	};

	encryptImage("kocada.bmp", "kocada_crypt.bmp", key, 10, NULL, ECB);
	//decryptImage("kocada_crypt.bmp", "kocada_decrypt.bmp", key, 10, NULL, ECB);
}

void alternativeEncryptionTest(){
	int const dataSize = 32;
	byte inputData[32] = { 0x4f, 0x6c, 0x61, 0x20, 0x74, 0x75, 0x64, 0x6f, 0x20, 0x62, 0x65, 0x6d, 0x20, 0x63, 0x6f, 0x6d, 0x4f, 0x6c, 0x61, 0x20, 0x74, 0x75, 0x64, 0x6f, 0x20, 0x62, 0x65, 0x6d, 0x20, 0x63, 0x6f, 0x6d };
	byte vigKey[16] = { 0x74, 0x6f, 0x6d, 0x61, 0x74, 0x65, 0x74, 0x6f, 0x6d, 0x61, 0x74, 0x65, 0x74, 0x6f, 0x6d, 0x61 };
	byte originalKey[16] = {
		0x2b, 0x28, 0xab, 0x09,
		0x7e, 0xae, 0xf7, 0xcf,
		0x15, 0xd2, 0x15, 0x4f,
		0x16, 0xa6, 0x88, 0x3c
	};
	byte iv[16] = { 0xB4, 0xCA, 0xD2, 0x45, 0x0E, 0x6F, 0x11, 0x20, 0x33, 0x34, 0xDD, 0x81, 0x47, 0x81, 0x13, 0x35 };
	char* transKey = "BOLA";
	byte result[32];
	

	printf("\nOriginal Data:\n");
	for (int i = 0; i < dataSize / 16; i++)
	{
		printf("\nBloco %d:\n", i + 1);
		printMatrix(inputData + i * 16);
	}
	encryptAlternative(inputData, dataSize, originalKey, result, 10, CBC, iv, 16);
	printf("\nResultado Encriptacao:\n");
	for (int i = 0; i < dataSize / 16; i++)
	{
		printf("\nBloco %d:\n", i+1);
		printMatrix(result + i * 16);
	}
}

void alternativeDecryptionTest(){
	int const dataSize = 32;
	/*ECB*/
	//byte inputData[32] = { 0x1e, 0xae, 0x8a, 0xdb, 0xa1, 0x5b, 0x11, 0xd3, 0x71, 0x4d, 0x77, 0x82, 0x39, 0x46, 0xc3, 0x85, 0x1e, 0xae, 0x8a, 0xdb, 0xa1, 0x5b, 0x11, 0xd3, 0x71, 0x4d, 0x77, 0x82, 0x39, 0x46, 0xc3, 0x85 };
	/*CBC*/
	byte inputData[32] = { 0x6a, 0x80, 0x78, 0x06, 0xaf, 0x08, 0x64, 0x73, 0xfa, 0x29, 0x5a, 0x4b, 0x0e, 0x33, 0x6e, 0xec, 0x54, 0x2e, 0x12, 0x35, 0x0e, 0xb3, 0x2d, 0x48, 0xd3, 0xcc, 0x1d, 0x51, 0xe7, 0xc1, 0xb5, 0x09 };
		
	byte vigKey[16] = { 0x74, 0x6f, 0x6d, 0x61, 0x74, 0x65, 0x74, 0x6f, 0x6d, 0x61, 0x74, 0x65, 0x74, 0x6f, 0x6d, 0x61 };
	byte originalKey[16] = {
		0x2b, 0x28, 0xab, 0x09,
		0x7e, 0xae, 0xf7, 0xcf,
		0x15, 0xd2, 0x15, 0x4f,
		0x16, 0xa6, 0x88, 0x3c
	};
	byte iv[16] = { 0xB4, 0xCA, 0xD2, 0x45, 0x0E, 0x6F, 0x11, 0x20, 0x33, 0x34, 0xDD, 0x81, 0x47, 0x81, 0x13, 0x35 };
	char* transKey = "BOLA";
	byte result[32];

		
	printf("\nOriginal Data:\n");
	for (int i = 0; i < dataSize / 16; i++)
	{
		printf("\nBloco %d:\n", i + 1);
		printMatrix(inputData + i * 16);
	}
	decryptAlternative(inputData, dataSize, originalKey, result, 10, CBC, iv, vigKey, transKey);
	printf("\nResultado Decriptacao:\n");
	for (int i = 0; i < dataSize / 16; i++)
	{
		printf("\nBloco %d:\n", i + 1);
		printMatrix(result + i * 16);
	}
}

void columnarTranspositionTest(){
	byte inputData[16] = { 0x4f, 0x6c, 0x61, 0x20, 0x74, 0x75, 0x64, 0x6f, 0x20, 0x62, 0x65, 0x6d, 0x20, 0x63, 0x6f, 0x6d };
	printMatrix(inputData);
	for (int i = 0; i < sizeof(inputData); i++)
		printf("%c", *(inputData + i));
	printf("\n");
	char* key = "BOLA";
	int keyLen = strlen(key);
	int dataSize = sizeof(inputData) / sizeof(*inputData);
	int rowsCount = (int)ceil((float)dataSize / (float)keyLen);
	int resultSize = keyLen * rowsCount;

	columnarTransposition(inputData, key, dataSize);

	for (int i = 0; i < resultSize; i++)
		printf("%c", *(inputData + i));
	printf("\n");
}

void inverseColumnarTranspositionTest(){
	byte inputData[16] = { 0x20, 0x4f, 0x61, 0x6c, 0x6f, 0x74, 0x64, 0x75, 0x6d, 0x20, 0x65, 0x62, 0x6d, 0x20, 0x6f, 0x63 };
	printMatrix(inputData);
	for (int i = 0; i < sizeof(inputData); i++)
		printf("%c", *(inputData + i));
	printf("\n");
	char* key = "BOLA";
	int keyLen = strlen(key);
	int dataSize = sizeof(inputData) / sizeof(*inputData);
	int rowsCount = (int)ceil((float)dataSize / (float)keyLen);
	int resultSize = keyLen * rowsCount;
	
	inverseColumnarTransposition(inputData, key, resultSize);
	
	for (int i = 0; i < resultSize; i++)
		if (inputData[i] != 0xff)
			printf("%c", *(inputData + i));
	printf("\n");
}

void encryptAddRoundKeyTest()
{
	byte originalKey[] = {
		0x2b, 0x28, 0xab, 0x09,
		0x7e, 0xae, 0xf7, 0xcf,
		0x15, 0xd2, 0x15, 0x4f,
		0x16, 0xa6, 0x88, 0x3c
	};
	byte iv[16] = { 0x2e, 0xc5, 0x31, 0x5F, 0xAA, 0xF5, 0xA7, 0x4E, 0x7D, 0x6E, 0xD2, 0x0A, 0x1E, 0x03, 0x25, 0x0F };
	byte inputData[32] = { 0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37, 0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34,
		0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37, 0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34 };
	byte result[32];

	printMatrix(inputData);
	encryptAddRoundKey(inputData, 32, originalKey, result, 10, CBC, iv);
	printMatrix(result);
	decryptAddRoundKey(result, 32, originalKey, inputData, 10, CBC, iv);
	printMatrix(inputData);
	
}

void alternativeEncrypt(){

}