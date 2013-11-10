// TrabalhoCriptografia.cpp : Defines the entry point for the console application.
//
#include "Tests.h"
#include <stdlib.h>
#include "Images.h"
#include <stdio.h>
#include <string.h>
#include <IL/il.h>

#define ENCRYPT 1
#define DECRYPT 0

#define ECB 1
#define CBC 0

typedef unsigned char byte;

char* concat(char *s1, char *s2)
{
	char *result = (char*) malloc(strlen(s1) + strlen(s2) + 1);//+1 for the zero-terminator
	//in real code you would check for errors in malloc here
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

byte valueOfChar(char c)
{
	if (c == '0')
		return 0;
	if (c == '1')
		return 1;
	if (c == '2')
		return 2;
	if (c == '3')
		return 3;
	if (c == '4')
		return 4;
	if (c == '5')
		return 5;
	if (c == '6')
		return 6;
	if (c == '7')
		return 7;
	if (c == '8')
		return 8;
	if (c == '9')
		return 9;
	if (c == 'A' || c == 'a')
		return 10;
	if (c == 'B' || c == 'b')
		return 11;
	if (c == 'C' || c == 'c')
		return 12;
	if (c == 'D' || c == 'd')
		return 13;
	if (c == 'E' || c == 'e')
		return 14;
	if (c == 'F' || c == 'f')
		return 15;
}
byte getValueFromChars(char* dupla)
{
//	printf("%s\n",dupla);
	byte ret = valueOfChar(dupla[0]) * 16 + valueOfChar(dupla[1]);
//	printf("%d\n", ret);
	return ret;

}
byte* getKeyFromFile()
{
	byte* key = (byte*)malloc(16 * sizeof(byte));
	FILE * file;
	char c;
	char* dupla = (char*)malloc(2 * sizeof(char));
	int pos = 1;
	int i = 0;
	file = fopen("key.txt", "r");
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
					key[i] = getValueFromChars(dupla);
					i++;
				}
				//putchar(c);
			
			} 
		}
		*(key + 16) = '\0';
		fclose(file);
	}
	return key;
}
byte* getIVFromFile()
{
	byte* iv = (byte*)malloc(16 * sizeof(byte));
	FILE * file;
	char c;
	char* dupla = (char*)malloc(2 * sizeof(char));
	int pos = 1;
	int i = 0;
	file = fopen("iv.txt", "r");
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
					iv[i] = getValueFromChars(dupla);
					i++;
				}
				//putchar(c);

			}
		}
		*(iv + 16) = '\0';
		fclose(file);
	}
	return iv;
}
void executeCipher(int op, int mode, int cript, int rounds, char* filepath)
{
	byte *iv = getIVFromFile();
	//printf("%d\n", iv);
	byte *key = getKeyFromFile();
	//printf("%d\n", key);
	
	int tam = strlen(filepath);
	char* addString;
	if (op == 1)	
		addString = "-cripto";	
	if (op == 2)
		addString = "-decripto";
	 
	int addStringTam = strlen(addString);
	char* destFile = (char*)malloc((tam+addStringTam) * sizeof(char));
	char *codif = (char*)malloc(3 * sizeof(char));
	strcpy(codif, filepath+(tam-4));
	printf("%s\n", codif);
	strncpy(destFile, filepath, tam - 4);
	destFile[tam - 4] = '\0';
	printf("%s\n",destFile);
	destFile = concat(destFile, addString);
	destFile = concat(destFile, codif);
	printf("%s", destFile);
	//CRIPTOGRAFAR
	if (op == 1)
	{
		
		//ECB
		if (mode == 1)
		{
			//AES APENAS COM ADDROUNDKEY
			//	if (cript == 1)
			//AES CONVENCIONAL
			if (cript == 2)
				encryptImage(filepath, destFile, key, rounds, NULL, ECB);
			//AES MODIFICADO
			//if (cript == 3)

		}
		//CBC
		if (mode == 2)
		{
			//AES APENAS COM ADDROUNDKEY
			//	if (cript == 1)
			//AES CONVENCIONAL
			if (cript == 2)
				encryptImage(filepath, destFile, key, rounds, iv, CBC);
			//AES MODIFICADO
			//if (cript == 3)
		}
	}
	//DECRIPTOGRAFAR
	if (op == 2)
	{
		//ECB
		if (mode == 1)
		{

			//if (cript == 1)

			if (cript == 2)
				decryptImage(filepath, destFile, key, rounds, NULL, ECB);
			//	if (cript == 3)

		}
		//CBC
		if (mode == 2)
		{
			//	if (cript == 1)

			if (cript == 2)
				decryptImage(filepath, destFile, key, rounds, iv, CBC);
			//	if (cript == 3)
		}
	}

}

void interface()
{

	int op = 0;
	int mode = 0;
	int cripto = 0;
	int rounds = 0;
	char *filepath = (char*)malloc(1000*sizeof(char));

	printf("Escolha a operação:\n");
	printf("1: Criptografar\n");
	printf("2: Decriptografar\n");
	scanf("%d", &op);

	printf("Escolha o modo:\n");
	printf("1: ECB\n");
	printf("2: CBC\n");
	scanf("%d", &mode);

	printf("Escolha o algoritmo de criptografia:\n");
	printf("1: AES com apenas AddRoundKey\n");
	printf("2: AES de 128 bits\n");
	printf("3: AES modificado\n");
	scanf("%d", &cripto);

	printf("Digite o numero de rounds desejado\n");

	scanf("%d", &rounds);

	printf("Por favor especifique o caminho da imagem a ser codificada.\n");

	scanf("%s", filepath);


	printf("Atencao! A chave e o vetor de inicialização (caso haja) são lidos dos arquivos:\n");
	printf("key.txt\n");
	printf("iv.txt\n");

	executeCipher(op, mode, cripto, rounds, filepath);

}


int main(int argc, char* argv[])
{
	ilInit();
	ilEnable(IL_FILE_OVERWRITE);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	//shiftRowsTest();
	//addRoundKeyTest();
	//testMixColumns();
	//testGen/erateRoundKeys();
	//testeBoladoECB(ENCRYPT);
	//testeBoladoCBC(ENCRYPT);
	//inverseShiftRowsTest();
	//testeBoladoECB(DECRYPT);
	//testeBoladoECBDecrypt();
	//testeBoladoCBCDecrypt();
	//printf("Press to start!\n");
	//system("pause");
	//imageECBTest();
	//imageCBCTest();
	//vigenereEncryptionTest();
	//vigenereDecryptionTest();
	//inverseSubBytesTest();
	//testInverseMixColumns();
	//byte b1[] = {85};
	//byte b2[] = {17};
	//printf("%d", calculateHammingDistance(b1,b2,1));
	interface();
	//encryptAddRoundKeyTest();
	/*columnarTranspositionTest();
	inverseColumnarTranspositionTest();*/
	system("pause");

	return 0;
}



