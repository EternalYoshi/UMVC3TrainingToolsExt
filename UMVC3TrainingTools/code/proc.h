#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

inline HANDLE hProcess = 0;
inline DWORD ProcID = 0;
inline DWORD dwPriorityClass;
inline uintptr_t ModuleBase = 0;
inline DWORD oldPageProtection = 0;
//inline uintptr_t ModuleBase = 0x140000000;
inline BYTE* addr;

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD procId);

DWORD GetPointerAddress(DWORD ptr, std::vector<DWORD> offsets);
