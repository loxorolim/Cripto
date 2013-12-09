/*
Universidade Federal Fluminense
Bacharelado em Ci�ncia da Computa��o
Introdu��o � Criptografia - 2013.2
Professor: Luis Ant�nio Kowada

Alunos:	Carlos Filipe Marques Teixeira Jr.
		Guilherme Rolim e Souza
		Mateus Carvalho Azis

Descri��o: Esse trabalho � uma implementa��o do algoritmo de criptografia de imagens baseada em Esteganografia. � poss�vel esconder uma imagem em outra usando certa quantidade de bits e tamb�m revelar uma imagem escondida.
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

//defini��o dos modos de uso
static const int ENCODE = 1;
static const int DECODE = 2;
static const int EXIT = 3;

char* createOuputFilePath(char *dest, const char *originalFile, const char *codingName)
{
	dest[0] = '\0';
	int fileLen = strlen(originalFile);
	const char *extension = originalFile + fileLen - 3;
	char fileName[100] = "";
	strncpy(fileName, originalFile, fileLen - 4);
	sprintf(dest, "%s-%s.%s", fileName, codingName, extension);
	return dest;
}

void interface()
{
	while (1){
		int op = 0;
		int usedBits = 0;
		int cripto = 0;
		int rounds = 0;
		char coverImage[1000];
		char imageToBeHidden[1000];

		do{
			printf("Digite o n�mero da op��o desejada para selecion�-la:\n\n");
			printf("Escolha a opera��o:\n");
			printf("1: Esconder Imagem\n");
			printf("2: Revelar Imagem\n");
			printf("3: Sair\n");
			scanf("%d", &op);
		} while (!(op >= ENCODE && op <= EXIT));

		if (op == EXIT)
			exit(0);

		do{
			printf("\nDigite o n�mero de bits, entre 1 e 8, a serem substitu�dos na imagem a ser modificada:\n");
			scanf("%d", &usedBits);
		} while (!(usedBits > 0 && usedBits < 9));


		printf("\nPor favor especifique o caminho da imagem a ser modificada (imagem vis�vel)\n");
		scanf("%s", coverImage);
		int tam = strlen(coverImage);

		if (op == ENCODE){
			printf("\nPor favor especifique o caminho da imagem a ser escondida.\n");
			scanf("%s", imageToBeHidden);
		}
		
		char destFile[100];
		createOuputFilePath(destFile, coverImage, op == ENCODE ? "steganographed" : "revealed");

		if (op == ENCODE)
			encrypt(coverImage, imageToBeHidden, destFile, usedBits);
		else if (op == DECODE)
			decrypt(coverImage, destFile, usedBits);

		printf("\n");
		printf("%s", "Arquivo gerado: ");
		printf("%s\n\n", destFile);
	}
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
