// TrabalhoCriptografia.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Tests.h"
#include <stdlib.h>
//#include <IL/il.h>

#define ENCRYPT 1
#define DECRYPT 0

int _tmain(int argc, _TCHAR* argv[])
{
	//ilInit();
	//ilEnable(IL_FILE_OVERWRITE);
	//ilEnable(IL_ORIGIN_SET);
	//ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
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
	printf("Press to start!\n");
	system("pause");
	//imageECBTest();
	//imageCBCTest();
	vigenereEncryptionTest();
	vigenereDecryptionTest();
	//inverseSubBytesTest();
	//testInverseMixColumns();
	//byte b1[] = {85};
	//byte b2[] = {17};
	//printf("%d", calculateHammingDistance(b1,b2,1));
	system("pause");

	return 0;
}


