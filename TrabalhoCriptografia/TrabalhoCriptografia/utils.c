#include "utils.h"

byte getValueFromChars(char* dupla)
{
	byte ret = valueOfChar(dupla[0]) * 16 + valueOfChar(dupla[1]);
	return ret;
}