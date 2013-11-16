#include <stdio.h>
#include <string.h>
#include <math.h>
#include "AlternativeAES.h"
#include "utils.h"

char* getTransKeyFromFile()
{
	char* key = (char*)malloc(4 * sizeof(char));
	FILE * file;
	char c;

	file = fopen("transkey.txt", "r");
	int i = 0;
	if (file)
	{
		while ((c = getc(file)) != EOF)
		{
			*(key + i) = c;
			i++;
		}
		*(key + 4) = '\0';
		fclose(file);
	}
	return key;
}
byte* getVigKeyFromFile(int* size)
{
	byte* tempKey = (byte*)malloc(16 * sizeof(byte));
	byte* key;
	FILE * file;
	char c;
	char* dupla = (char*)malloc(2 * sizeof(char));
	int pos = 1;
	int i = 0;
	file = fopen("vigkey.txt", "r");
	if (file)
	{
		while ((c = getc(file)) != EOF)
		{
			if (c != ' ')
			{

				if (pos)
				{
					*dupla = c;
					pos = !pos;
				}
				else
				{
					*(dupla + 1) = c;
					*(dupla + 2) = '\0';
					pos = !pos;
					tempKey[i] = getValueFromChars(dupla);
					i++;
				}
				//putchar(c);
			}
		}
		*size = i;
		key = (byte*)malloc(*size * sizeof(byte));
		memcpy(key, tempKey, *size);
		free(tempKey);
		*(key + *size) = '\0';
		fclose(file);
	}
	return key;
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
	int rowsCount = (int)ceil((float)dataSize / (float)size);
	int resultSize = size * rowsCount;

	byte* result = (byte*)calloc(resultSize, sizeof(byte));

	int target = 0;
	int stop = 0;
	while (!stop){
		for (int i = 0; i < size; i++){
			if (order[i] == target){
				for (int j = 0; j < rowsCount; j++){
					if (j < rowsCount - 1 || residue == 0)
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

void vigenereCipherEncryption(byte *bytes, byte *key, int size){
	for (int i = 0; i < 16; i++)
		bytes[i] = (bytes[i] + key[i%(size)]) % 256;
}

void encryptBlockAlternative(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type, byte *vigKey, char *transKey, int vigKeySize)
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
		vigenereCipherEncryption(result, vigKey, vigKeySize);
		columnarTransposition(result, transKey, 16);
		//shiftRows(result);
		//mixColumns(result);

		addRoundKey(result, 0, allKeys[i]);
	}

	//Último round
	vigenereCipherEncryption(result, vigKey, vigKeySize);
	//shiftRows(result);
	addRoundKey(result, 0, allKeys[rounds]);

	if (type == CBC) // SE CBC
		*toXor = result;
}

void encryptAlternative(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv){
	int* vigKeySize = (int*)calloc(1,sizeof(int));
	byte *vigKey = getVigKeyFromFile(vigKeySize);
	char *transKey = getTransKeyFromFile();
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
		encryptBlockAlternative(data + i * 16, allKeys, rounds, &toXor, result + i * 16, type, vigKey, transKey, *vigKeySize);
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
	//fim transposição =========================================================================

	for (int i = 0; i < rounds + 1; i++)
		free(allKeys[i]);

	free(allKeys);
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
	int rowsCount = (int)ceil((float)dataSize / (float)keyLen);
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

void vigenereCipherDecryption(byte *bytes, byte *key, int size){
	for (int i = 0; i < 16; i++)
		bytes[i] = (bytes[i] - key[i%size]) % 256;
}

void decryptBlockAlternative(byte* data, byte** allKeys, int rounds, byte** toXor, byte* result, int type, byte* vigKey, char* transKey, int vigKeySize)
{
	memcpy(result, data, 16 * sizeof(byte));
	int dataSize = sizeof(result) / sizeof(*result);

	//Primeiro round
	addRoundKey(result, 0, allKeys[rounds]);
	vigenereCipherDecryption(result, vigKey, vigKeySize);

	//demais rounds
	for (int i = rounds - 1; i >= 1; i--){
		addRoundKey(result, 0, allKeys[i]);
		inverseColumnarTransposition(result, transKey, 16);
		vigenereCipherDecryption(result, vigKey, vigKeySize);
	}

	//Último round
	addRoundKey(result, 0, allKeys[0]);

	if (type == CBC){
		xor(result, *toXor, result);
		*toXor = data;
	}
}

void decryptAlternative(byte * data, int dataSize, byte * key, byte * result, int rounds, int type, byte * iv)
{
	int* vigKeySize = (int*)calloc(1, sizeof(int));
	byte* vigKey = getVigKeyFromFile(vigKeySize);
	char* transKey = getTransKeyFromFile();
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
		decryptBlockAlternative(data + i * 16, allKeys, rounds, &toXor, result + i * 16, type, vigKey, transKey, *vigKeySize);
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
