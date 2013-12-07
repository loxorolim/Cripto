#Para escrever comentários ##
############################# Makefile ##########################
DEVIL_INCLUDE_PATH=../devil/include
DEVIL_LIB=../devil/lib/DevIL.lib

all: trabalho
trabalho:images.o main.o
		# O compilador faz a ligação entre os dois objetos
	gcc -oTrabalhoCripto images.o main.o $(DEVIL_LIB)
#-----> Distancia com o botão TAB ### e não com espaços
images.o:Images.c
	gcc -o images.o -c Images.c -I$(DEVIL_INCLUDE_PATH)
main.o:main.c
	gcc -o main.o -c main.c -I$(DEVIL_INCLUDE_PATH)

clean:
	rm -rf *.o
mrproper: clean
	rm -rf teste
