#ifndef TESTDLL_NATIVE_LIB_H
#define TESTDLL_NATIVE_LIB_H

#define DLLExport __declspec(dllexport)

extern "C"
{
	DLLExport int addWrapper(int num1, int num2);
	DLLExport int multiplyWrapper(int num1, int num2);
	DLLExport int substractWrapper(int num1, int num2);
	DLLExport int divideWrapper(int num1, int num2);
	DLLExport int generateLayout(int** data, int* size);
}
#endif