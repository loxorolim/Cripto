#ifndef TESTS_H
#define TESTS_H

void inverseSubBytesTest();

void shiftRowsTest();
void inverseShiftRowsTest();

void addRoundKeyTest();
void testGenerateRoundKeys();

void testMixColumns();
void testInverseMixColumns();

void testeBoladoECB(int mode);
void testeBoladoCBC(int mode);
void testeBoladoECBDecrypt();
void testeBoladoCBCDecrypt();

void imageECBTest();
void imageCBCTest();

void alternativeEncryptionTest();
void alternativeDecryptionTest();

void columnarTranspositionTest();
void inverseColumnarTranspositionTest();

void encryptAddRoundKeyTest();



#endif