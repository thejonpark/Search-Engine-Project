#include "../head/char_functions.h"

bool isLetter(char c)
{
	// it is a letter if it is either lower case or upper case
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

bool isNumber(char c)
{
	return c >= '0' && c <= '9';
}

bool isSpecial(char c)
{
	return !isLetter(c) && !isNumber(c);
}

char lower(char c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return c + 'a' - 'A';
	}
	return c;
}

void lower(char* str)
{
	for (int i = 0; str[i] != '\0'; ++ i)
	{
		str[i] = lower(str[i]);
	}
}