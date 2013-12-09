#Para escrever comentários ##
############################# Makefile ##########################
DEVIL_INCLUDE_PATH=../devil/include
DEVIL_LIB=../devil/lib/DevIL.lib

all: trabalho
trabalho:imagedata.o images.o main.o
		# O compilador faz a ligação entre os dois objetos
	gcc -oTrabalhoCripto imagedata.o images.o main.o $(DEVIL_LIB)
#-----> Distancia com o botão TAB ### e não com espaços
imagedata.o:ImageData.c
	gcc -o imagedata.o -c ImageData.c -I$(DEVIL_INCLUDE_PATH)
images.o:Images.c
	gcc -o images.o -c Images.c imagedata.o
main.o:main.c
	gcc -o main.o -c main.c -I$(DEVIL_INCLUDE_PATH)

clean:
	rm -rf *.o
mrproper: clean
	rm -rf teste
