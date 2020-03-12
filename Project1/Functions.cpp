#include "../Dunchangeling/TestDLL.h"
#include "Functions.h"

int addWrapper(int num1, int num2)
{
	return add(num1, num2);
}

int multiplyWrapper(int num1, int num2)
{
	return multiply(num1, num2);
}

int substractWrapper(int num1, int num2)
{
	return substract(num1, num2);
}

int divideWrapper(int num1, int num2)
{
	return divide(num1, num2);
}