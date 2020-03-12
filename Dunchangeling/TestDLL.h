//#ifndef TESTDLL_NATIVE_LIB_H
//#define TESTDLL_NATIVE_LIB_H
//
//#define DLLExport __declspec(dllexport)
//
//extern "C"
//{
//	DLLExport int add(int num1, int num2);
//	DLLExport int multiply(int num1, int num2);
//	DLLExport int substract(int num1, int num2);
//	DLLExport int divide(int num1, int num2);
//}
//#endif

#define DLLExport __declspec(dllexport)

DLLExport int add(int num1, int num2);
DLLExport int multiply(int num1, int num2);
DLLExport int substract(int num1, int num2);
DLLExport int divide(int num1, int num2);