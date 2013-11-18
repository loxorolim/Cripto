#Para escrever comentários ##
############################# Makefile ##########################
DEVIL_INCLUDE_PATH=../devil/include
DEVIL_LIB=../devil/lib/DevIL.lib

all: trabalho
trabalho:aes.o images.o main.o utils.o AlternativeAES.o AddRoundKeyAES.o
		# O compilador faz a ligação entre os dois objetos
	gcc -oTrabalhoCripto aes.o images.o main.o utils.o AlternativeAES.o AddRoundKeyAES.o $(DEVIL_LIB)
#-----> Distancia com o botão TAB ### e não com espaços
aes.o:AES.c
	gcc -o aes.o -c AES.c -std=c99
images.o:Images.c
	gcc -o images.o -c Images.c -std=c99 -I$(DEVIL_INCLUDE_PATH)
main.o:main.c
	gcc -o main.o -c main.c -std=c99 -I$(DEVIL_INCLUDE_PATH)
utils.o:Utils.c
	gcc -o utils.o -c Utils.c -std=c99
AlternativeAES.o:AlternativeAES.c
	gcc -o AlternativeAES.o -c AlternativeAES.c -std=c99
AddRoundKeyAES.o:AddRoundKeyAES.c
	gcc -o AddRoundKeyAES.o -c AddRoundKeyAES.c -std=c99


clean:
	rm -rf *.o
mrproper: clean
	rm -rf teste