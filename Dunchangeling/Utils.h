#pragma once

#include <Windows.h>
#include <atlstr.h>

VOID WINAPI FileWrittenCallback(DWORD dwErrorCode, DWORD dwBytesTransferred, LPOVERLAPPED lpOverlapped);

bool WriteAsync(const char* file, const char* content, int size);