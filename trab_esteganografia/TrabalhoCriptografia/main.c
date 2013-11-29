/*
Universidade Federal Fluminense
Bacharelado em Ciência da Computação
Introdução à Criptografia - 2013.2
Professor: Luis Antônio Kowada

Alunos:	Carlos Filipe Marques Teixeira Jr.
		Guilherme Rolim e Souza
		Mateus Carvalho Azis

Descrição: Esse trabalho é uma implementação do algoritmo AES com chave de 128 bits para codificação de imagens. Possui uma versão alternativa que apenas realiza a operação AddRoundKey e outra versão que utiliza Cifra de Vigenére.
*/

//includes das bibliotecas padrão
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

//biblioteca de imagens
#include <IL/il.h>

//nosso headers
#include "Tests.h"
#include "Images.h"
#include "AES.h"
#include "AlternativeAES.h"
#include "AddRoundKeyAES.h"

typedef unsigned char byte;

char* concat(char *s1, char *s2)
{
	char *result = (char*)malloc(strlen(s1) + strlen(s2) + 1);//+1 for the zero-terminator
	//in real code you would check for errors in malloc here
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

byte* getKeyFromFile()
{
	byte* key = (byte*)malloc(16 * sizeof(byte));
	FILE * file;
	char c;
	char* dupla = (char*)malloc(3 * sizeof(char));
	int pos = 1;
	int i = 0;
	file = fopen("key.txt", "r");
	if (file)
	{
		while ((c = getc(file)) != EOF && i<16)
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
		//	*(key + 16) = '\0';
		fclose(file);
	}
	free(dupla);

	return key;
}

byte* getIVFromFile()
{
	byte* iv = (byte*)malloc(16 * sizeof(byte));
	FILE * file;
	char c;
	char* dupla = (char*)malloc(3 * sizeof(char));
	int pos = 1;
	int i = 0;
	file = fopen("iv.txt", "r");
	if (file)
	{
		while ((c = getc(file)) != EOF && i<16)
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
		//*(iv + 16) = '\0';
		fclose(file);
	}
	free(dupla);
	return iv;
}

void executeCipher(int op, int mode, int cript, int rounds, char* filepath)
{
	//getTransKeyFromFile();
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
	char* destFile = (char*)malloc((tam + addStringTam) * sizeof(char));
	char *codif = (char*)malloc(5 * sizeof(char));
	strcpy(codif, filepath + (tam - 4));
	//printf("%s\n", codif);
	strncpy(destFile, filepath, tam - 4);
	destFile[tam - 4] = '\0';
	//printf("%s\n",destFile);
	destFile = concat(destFile, addString);
	destFile = concat(destFile, codif);
	printf("\n");
	printf("%s", "Arquivo gerado: ");
	printf("%s\n\n", destFile);
	//CRIPTOGRAFAR
	if (op == 1)
	{

		//ECB
		if (mode == 1)
		{
			//AES APENAS COM ADDROUNDKEY
			if (cript == 1)

				process(filepath, destFile, key, rounds, NULL, ECB, encryptAddRoundKey);
			//AES CONVENCIONAL
			if (cript == 2)
				process(filepath, destFile, key, rounds, NULL, ECB, encrypt);
			//process(filepath, destFile, key, rounds, NULL, ECB,encrypt);
			//AES MODIFICADO
			if (cript == 3)
				process(filepath, destFile, key, rounds, NULL, ECB, encryptAlternative);

		}
		//CBC
		if (mode == 2)
		{
			//AES APENAS COM ADDROUNDKEY
			if (cript == 1)
				process(filepath, destFile, key, rounds, iv, CBC, encryptAddRoundKey);
			//AES CONVENCIONAL
			if (cript == 2)
				process(filepath, destFile, key, rounds, iv, CBC, encrypt);
			//AES MODIFICADO
			if (cript == 3)
				process(filepath, destFile, key, rounds, iv, CBC, encryptAlternative);
		}

		printf("Distância de hamming entre %s e %s: %f\n", filepath, destFile, getImageHammingDistance(filepath, destFile));
	}
	//DECRIPTOGRAFAR
	if (op == 2)
	{
		//ECB
		if (mode == 1)
		{

			if (cript == 1)
				process(filepath, destFile, key, rounds, NULL, ECB, decryptAddRoundKey);
			if (cript == 2)
				process(filepath, destFile, key, rounds, NULL, ECB, decrypt);
			if (cript == 3)
				process(filepath, destFile, key, rounds, NULL, ECB, decryptAlternative);

		}
		//CBC
		if (mode == 2)
		{
			if (cript == 1)
				process(filepath, destFile, key, rounds, iv, CBC, decryptAddRoundKey);
			if (cript == 2)
				process(filepath, destFile, key, rounds, iv, CBC, decrypt);
			if (cript == 3)
				process(filepath, destFile, key, rounds, iv, CBC, decryptAlternative);

		}
	}
	free(iv);
	free(key);
	//	free(addString);
	free(destFile);
	free(codif);

}

void interface()
{
	int op = 0;
	int mode = 0;
	int cripto = 0;
	int rounds = 0;
	char *filepath = (char*)malloc(1000 * sizeof(char));

	printf("Digite o número da opção desejada para selecioná-la:\n\n");
	printf("Escolha a operação:\n");
	printf("1: Criptografar\n");
	printf("2: Decriptografar\n");
	printf("3: Sair\n");
	scanf("%d", &op);
	if (op == 3)
		exit(0);


	printf("Escolha o modo:\n");
	printf("1: ECB\n");
	printf("2: CBC\n");
	printf("3: Sair\n");
	scanf("%d", &mode);
	if (mode == 3)
		exit(0);

	printf("Escolha o algoritmo de criptografia:\n");
	printf("1: AES com apenas AddRoundKey\n");
	printf("2: AES de 128 bits\n");
	printf("3: AES modificado\n");
	printf("4: Sair\n");
	scanf("%d", &cripto);
	if (cripto == 4)
		exit(0);

	printf("Digite o numero de rounds desejado\n");

	scanf("%d", &rounds);

	printf("Por favor especifique o caminho da imagem a ser codificada.\n");

	scanf("%s", filepath);

	printf("\nAtenção! A chave e o vetor de inicialização (caso haja) são lidos dos arquivos:\n");
	printf("\nkey.txt\n");
	printf("iv.txt\n");

	if (cripto == 3)
	{
		printf("\nAtenção! O algoritmo alternativo criado utiliza duas chaves, a de vigenére e a de transposição que são lidas dos arquivos:\n");
		printf("\nvigkey.txt\n");
		printf("transkey.txt\n\n");
		printf("A chave transkey deve conter 4 caracteres.\n");
	}

	executeCipher(op, mode, cripto, rounds, filepath);

}

int main(int argc, char* argv[])
{
	ilInit();                                //inicializa biblioteca de imagens
	ilEnable(IL_FILE_OVERWRITE);             //permite que a biblioteca sobrescreva arquivos em disco
	ilEnable(IL_ORIGIN_SET);                 //define a origem das imagens como o canto superior esquerdo
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	setlocale(LC_ALL, "Portuguese");

	
	/*aplicaDoge("penguins.bmp", "b1.png", "doge.bmp", 1);
	process("b1.png", "c1.png", 1);

	aplicaDoge("penguins.bmp", "b2.png", "doge.bmp", 2);
	process("b2.png", "c2.png", 2);

	aplicaDoge("penguins.bmp", "b3.png", "doge.bmp", 3);
	process("b3.png", "c3.png", 3);

	aplicaDoge("penguins.bmp", "b4.png", "doge.bmp", 4);
	process("b4.png", "c4.png", 4);

	aplicaDoge("penguins.bmp", "b5.png", "doge.bmp", 5);
	process("b5.png", "c5.png", 5);

	aplicaDoge("penguins.bmp", "b6.png", "doge.bmp", 6);
	process("b6.png", "c6.png", 6);

	aplicaDoge("penguins.bmp", "b7.png", "doge.bmp", 7);
	process("b7.png", "c7.png", 7);

	aplicaDoge("penguins.bmp", "b8.png", "doge.bmp", 8);
	process("b8.png", "c8.png", 7);*/

	process("a.png", "b.png", 2);

	return 0;
}
