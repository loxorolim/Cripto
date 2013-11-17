#include "AES.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes_tables.h"

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

			for (i = 0; i < 4; i++){
				byte rconValue = i == 0 ? rcon[roundCount] : 0;
				lastColumn[i] = currentKey[i * 4 + j] = lastKey[i * 4 + j] ^ rotatedColumn[i] ^ rconValue;
			}
		}
		else {
			for (i = 0; i < 4; i++)
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

	//Último round
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

	//Último round
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

	
	int remainingBytes = dataSize % 16;               //calcula quantos bytes sobraram no último blocoo
	if (remainingBytes != 0){                        //se sobraram bytes, junta-os com os anteriores e re-criptografa
		int reusedBytes = 16 - remainingBytes;       //quantidade de bytes que serão re-criptografados
		byte lastBlock[16];                          //aloca espaço para o bloco fictício
		memcpy(lastBlock, result + dataSize - 16, reusedBytes);   //preenche esse bloco com os bytes reusados
		memcpy(lastBlock + reusedBytes, data + dataSize - remainingBytes, remainingBytes);  //preenche o bloco com os bytes que sobraram

		encryptBlock(lastBlock, allKeys, rounds, NULL, result + dataSize - 16, ECB); //encripta o bloco virtual
	}

	matrixTransposer(key);
	if (type == CBC)
		matrixTransposer(iv);
	//fim transposição =========================================================================

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

	int remainingBytes = dataSize % 16;
	if (remainingBytes != 0){
		byte lastBlock[16];
		memcpy(lastBlock, data + dataSize - 16, 16);

		decryptBlock(lastBlock, allKeys, rounds, NULL, data + dataSize - 16, ECB);

		//tem que copiar os bytes que sobraram pro result, pois eles não serão tratados pelo algoritmo principal
		memcpy(result + dataSize - remainingBytes, data + dataSize - remainingBytes, remainingBytes);
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

static long calculateOnBits(byte b)
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

float calculateHammingDistance(byte * clearM, byte * criptoM, int arraySize)
{
	long dist = 0;

	for (int i = 0; i < arraySize; i++)
	{
		byte xor = clearM[i] ^ criptoM[i];
		dist += calculateOnBits(xor);
	}
	return dist / (float)(arraySize * 8.0f); //quantidade total de bits = quantidade de bytes * 8
}

