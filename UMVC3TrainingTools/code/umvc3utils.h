#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

inline bool ValidVer;

bool CheckGame();

DWORD SingleWrite(DWORD64 AddressToWrite, int length, HANDLE hProcess, BYTE ByteToWrite, DWORD errorMessageID, LPSTR messageBuffer);

DWORD MultiWrite(DWORD64 AddressToWrite, int length, HANDLE hProcess, BYTE ByteToWrite[], DWORD errorMessageID, LPSTR messageBuffer);

DWORD ProtectedSingleWrite(DWORD64 AddressToWrite, int length, HANDLE hProcess, BYTE ByteToWrite, DWORD errorMessageID, LPSTR messageBuffer, DWORD oldPageProtection);

DWORD ProtectedMultiWrite(DWORD64 AddressToWrite, int length, HANDLE hProcess, BYTE ByteToWrite[], DWORD errorMessageID, LPSTR messageBuffer, DWORD oldPageProtection);
