#pragma once

#include <windows.h>
#include <string.h>

WINBASEAPI void     __cdecl     MSVCRT$free(void* _Block);
WINBASEAPI void*    WINAPI      MSVCRT$malloc(SIZE_T);
WINBASEAPI int      __cdecl     MSVCRT$_snprintf(char* s, size_t n, const char* fmt, ...);
WINBASEAPI char*    __cdecl     MSVCRT$strtok(char* _String, const char* _Delimiter);
WINBASEAPI BOOL     WINAPI      KERNEL32$CloseHandle(HANDLE hObject);
WINBASEAPI HANDLE   WINAPI      KERNEL32$CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, _In_opt_ HANDLE hTemplateFile);
WINBASEAPI BOOL     __stdcall   KERNEL32$FindClose(HANDLE hFile);
WINBASEAPI HANDLE   __stdcall   KERNEL32$FindFirstFileA(LPCSTR filename, LPWIN32_FIND_DATAA fileAttributes);
WINBASEAPI BOOL     __stdcall   KERNEL32$FindNextFileA(HANDLE hFoundFile, LPWIN32_FIND_DATAA fileAttributes);
WINBASEAPI DWORD    __stdcall   KERNEL32$GetEnvironmentVariableA(LPCSTR lpName, LPSTR lpBuffer, DWORD nSize);
WINBASEAPI DWORD    WINAPI      KERNEL32$GetLastError();