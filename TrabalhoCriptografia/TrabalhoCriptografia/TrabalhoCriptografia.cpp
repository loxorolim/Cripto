// TrabalhoCriptografia.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Tests.h"
#include <stdlib.h>

#define ENCRYPT 1
#define DECRYPT 0

int _tmain(int argc, _TCHAR* argv[])
{
	//shiftRowsTest();
	//addRoundKeyTest();
	//testMixColumns();
	//testGen/erateRoundKeys();
	//testeBoladoEBC(ENCRYPT);
	//inverseShiftRowsTest();
	//testeBoladoEBC(DECRYPT);
	testInverseMixColumns();
	//byte b1[] = {85};
	//byte b2[] = {17};
	//printf("%d", calculateHammingDistance(b1,b2,1));
	system("pause");

	return 0;
}


