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

/*void interface()
{
	int op = 0;
	int mode = 0;
	int cripto = 0;
	int rounds = 0;
	char *filepath = (char*)malloc(1000 * sizeof(char));

	printf("Digite o n�mero da op��o desejada para selecion�-la:\n\n");
	printf("Escolha a opera��o:\n");
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

	printf("\nAten��o! A chave e o vetor de inicializa��o (caso haja) s�o lidos dos arquivos:\n");
	printf("\nkey.txt\n");
	printf("iv.txt\n");

	if (cripto == 3)
	{
		printf("\nAten��o! O algoritmo alternativo criado utiliza duas chaves, a de vigen�re e a de transposi��o que s�o lidas dos arquivos:\n");
		printf("\nvigkey.txt\n");
		printf("transkey.txt\n\n");
		printf("A chave transkey deve conter 4 caracteres.\n");
	}

	executeCipher(op, mode, cripto, rounds, filepath);

}*/

int main(int argc, char* argv[])
{
	ilInit();                                //inicializa biblioteca de imagens
	ilEnable(IL_FILE_OVERWRITE);             //permite que a biblioteca sobrescreva arquivos em disco
	ilEnable(IL_ORIGIN_SET);                 //define a origem das imagens como o canto superior esquerdo
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	setlocale(LC_ALL, "Portuguese");

	char str1[20] = "";
	char str2[20] = "";
	system("PAUSE");
	for (int usedBits = 1; usedBits <= 8; usedBits++){
		str1[0] = str2[0] = '\0';
		sprintf(str1, "b%d.png", usedBits);
		sprintf(str2, "c%d.png", usedBits);

		encrypt("doge.bmp", str1, "penguins.bmp", usedBits);
		decrypt(str1, str2, usedBits);
	}

	decrypt("a.png", "b.png", 2);

	system("PAUSE");
	return 0;
}
