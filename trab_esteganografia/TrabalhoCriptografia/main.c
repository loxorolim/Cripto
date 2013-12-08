/*
Universidade Federal Fluminense
Bacharelado em Ci�ncia da Computa��o
Introdu��o � Criptografia - 2013.2
Professor: Luis Ant�nio Kowada

Alunos:	Carlos Filipe Marques Teixeira Jr.
		Guilherme Rolim e Souza
		Mateus Carvalho Azis

Descri��o: Esse trabalho � uma implementa��o do algoritmo AES com chave de 128 bits para codifica��o de imagens. Possui uma vers�o alternativa que apenas realiza a opera��o AddRoundKey e outra vers�o que utiliza Cifra de Vigen�re.
*/

//includes das bibliotecas padr�o
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

//biblioteca de imagens
#include <IL/il.h>

//nosso headers
#include "utils.h"
#include "Images.h"

char* concat(char *s1, char *s2)
{
	char *result = (char*)malloc(strlen(s1) + strlen(s2) + 1);//+1 for the zero-terminator
	//in real code you would check for errors in malloc here
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

char* createOuputFilePath(char *s1, int tam, char *s2, int addStringTam, int op)
{
	char* destFile = (char*)malloc((tam + addStringTam) * sizeof(char));
	char *codif = (char*)malloc(5 * sizeof(char));

	strcpy(codif, s1 + (tam - 4));
	strncpy(destFile, s1, tam - 4);
	destFile[tam - 4] = '\0';
	destFile = concat(destFile, s2);
	destFile = concat(destFile, codif);
	free(codif);
	return destFile;
}

void interface()
{
	int op = 0;
	int usedBits = 0;
	int cripto = 0;
	int rounds = 0;
	char *coverImage = (char*)malloc(1000 * sizeof(char));
	char *imageToBeHidden = (char*)malloc(1000 * sizeof(char));
	char *result = (char*)malloc(1000 * sizeof(char));

	do{
		printf("Digite o n�mero da op��o desejada para selecion�-la:\n\n");
		printf("Escolha a opera��o:\n");
		printf("1: Criptografar\n");
		printf("2: Decriptografar\n");
		printf("3: Sair\n");
		scanf("%d", &op);
	} while (!(op > 0 && op < 4));
	if (op == 3)
		exit(0);

	do{
		printf("Digite o n�mero de bits, entre 1 e 8, a ser substituido na imagem a ser modificada:\n");
		scanf("%d", &usedBits);
	} while (!(usedBits > 0 && usedBits < 9));


	printf("Por favor especifique o caminho da imagem a ser modificada.\n");
	scanf("%s", coverImage);

	if (op == 1){
		printf("Por favor especifique o caminho da imagem a ser escondida.\n");
		scanf("%s", imageToBeHidden);
	}
	int tam = strlen(coverImage);
	
	char* addString;
	if (op == 1)
		addString = "-steganographed";
	if (op == 2)
		addString = "-revealed";
	int addStringTam = strlen(addString);

	char* destFile = (char*)malloc((tam + addStringTam) * sizeof(char));

	destFile = createOuputFilePath(coverImage, tam, addString, addStringTam, op);

	if (op == 1)
		encrypt(coverImage, imageToBeHidden, destFile, usedBits);
	else if (op == 2)
		decrypt(coverImage, destFile, usedBits);

	printf("\n");
	printf("%s", "Arquivo gerado: ");
	printf("%s\n\n", destFile);
}

int main(int argc, char* argv[])
{
	ilInit();                                //inicializa biblioteca de imagens
	ilEnable(IL_FILE_OVERWRITE);             //permite que a biblioteca sobrescreva arquivos em disco
	ilEnable(IL_ORIGIN_SET);                 //define a origem das imagens como o canto superior esquerdo
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	setlocale(LC_ALL, "Portuguese");

	interface();

	ilShutDown();
	system("PAUSE");
	return 0;
}
