#include "AddRoundKeyAES.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes_tables.h"

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

	//Último round
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

	int remainingBytes = dataSize % 16;               //calcula quantos bytes sobraram no último blocoo
	if (remainingBytes != 0){                        //se sobraram bytes, junta-os com os anteriores e re-criptografa
		int reusedBytes = 16 - remainingBytes;       //quantidade de bytes que serão re-criptografados
		byte lastBlock[16];                          //aloca espaço para o bloco fictício
		memcpy(lastBlock, result + dataSize - 16, reusedBytes);   //preenche esse bloco com os bytes reusados
		memcpy(lastBlock + reusedBytes, data + dataSize - remainingBytes, remainingBytes);  //preenche o bloco com os bytes que sobraram

		encryptBlockAddRoundKey(lastBlock, allKeys, rounds, NULL, result + dataSize - 16, ECB); //encripta o bloco virtual
	}

	matrixTransposer(key);
	if (type == CBC)
		matrixTransposer(iv);
	//fim transposição =========================================================================

	for (int i = 0; i < rounds + 1; i++)
		free(allKeys[i]);


	free(allKeys);
}
void decryptBlockAddRoundKey(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type)
{
	memcpy(result, data, 16 * sizeof(byte));

	//Primeiro round
	addRoundKey(result, 0, allKeys[rounds]);

	//demais rounds
	for (int i = rounds - 1; i >= 1; i--){
		addRoundKey(result, 0, allKeys[i]);
	}

	//Último round
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

	int remainingBytes = dataSize % 16;
	if (remainingBytes != 0){
		byte lastBlock[16];
		memcpy(lastBlock, data + dataSize - 16, 16);

		decryptBlockAddRoundKey(lastBlock, allKeys, rounds, NULL, data + dataSize - 16, ECB);

		//tem que copiar os bytes que sobraram pro result, pois eles não serão tratados pelo algoritmo principal
		memcpy(result + dataSize - remainingBytes, data + dataSize - remainingBytes, remainingBytes);
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
	}

	for (int i = 0; i < rounds + 1; i++)
		free(allKeys[i]);


	free(allKeys);
}