#include <Windows.h>
#include <atlstr.h>
#include <string>
#include <iostream>

VOID WINAPI FileWrittenCallback(DWORD dwErrorCode, DWORD dwBytesTransferred, LPOVERLAPPED lpOverlapped)
{
	if (dwErrorCode != 0)
	{
		fprintf(stdout, "CompletionRoutine: Unable to write to file! Error: %u, AddrOverlapped: %p\n", dwErrorCode, lpOverlapped);
	}
	else
	{
		fprintf(stdout, "CompletionRoutine: Transferred: %u Bytes, AddrOverlapped: %p\n", dwBytesTransferred, lpOverlapped);
	}
}

bool WriteAsync(const char* file, const char* content, int size)
{
	char drive[4];
	char dir[128];
	char pathBuffer[256];
	_splitpath_s(file, drive, 4, dir, 128, NULL, 0, NULL, 0);
	_makepath_s(pathBuffer, 256, drive, dir, NULL, NULL);

	//TODO check if dir exists

	//std::cout << "The path: " << pathBuffer << std::endl;
	if (!CreateDirectory(pathBuffer, NULL))
	{
		//fprintf(stdout, "Could not create directory! Error %u\n", GetLastError());
	}

	HANDLE hFile = CreateFile(file, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		fprintf(stdout, "Could not create file! Error %u\n", GetLastError());
		return false;
	}

	OVERLAPPED oFile;
	oFile.Offset = 0xFFFFFFFF;
	oFile.OffsetHigh = 0xFFFFFFFF;

	if (!WriteFileEx(hFile, content, size, &oFile, (LPOVERLAPPED_COMPLETION_ROUTINE)FileWrittenCallback))
	{
		fprintf(stdout, "Unable to write to file! Error %u\n", GetLastError());
	}
	CloseHandle(hFile);
	return true;
}