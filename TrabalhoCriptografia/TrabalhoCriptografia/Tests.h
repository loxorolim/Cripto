#ifndef TESTS_H
#define TESTS_H

void inverseSubBytesTest();

void shiftRowsTest();
void inverseShiftRowsTest();

void addRoundKeyTest();
void testGenerateRoundKeys();

void testMixColumns();
void testInverseMixColumns();

void testeBoladoEBC(int mode);
void testeBoladoCBC(int mode);
void testeBoladoEBCDecrypt();

#endif