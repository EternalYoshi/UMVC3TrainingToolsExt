//#include "stdafx.h"
#include "proc.h"
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>


DWORD GetProcId(const wchar_t* procName)
{
	//Uses the Window Name to get the the Thread ID to get the Process ID.
	DWORD PID = 0;
	DWORD TID = 0;

	HWND Test = FindWindow(0, "ULTIMATE MARVEL VS. CAPCOM 3");
	if (Test) 
	{
		TID = GetWindowThreadProcessId(Test, &TID);
	}

	HANDLE hSnapshot = OpenThread(THREAD_ALL_ACCESS, FALSE, TID);
	PID = GetProcessIdOfThread(hSnapshot);

	return PID;

}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{

	//DWORD_PTR   baseAddress = 0;
	//HANDLE      processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
	//HMODULE* moduleArray;
	//LPBYTE      moduleArrayBytes;
	//DWORD       bytesRequired;
	//if (processHandle)
	//{
	//	if (EnumProcessModules(processHandle, NULL, 0, &bytesRequired))
	//	{
	//		if (bytesRequired)
	//		{
	//			moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);
	//			if (moduleArrayBytes)
	//			{
	//				unsigned int moduleCount;
	//				moduleCount = bytesRequired / sizeof(HMODULE);
	//				moduleArray = (HMODULE*)moduleArrayBytes;
	//				if (EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired))
	//				{
	//					baseAddress = (DWORD_PTR)moduleArray[0];
	//				}
	//				LocalFree(moduleArrayBytes);
	//			}
	//		}
	//	}
	//	CloseHandle(processHandle);
	//}
	//return baseAddress;

	
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(L"modEntry.szModule", modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
	
}

DWORD GetPointerAddress(HANDLE hProc, DWORD ptr, std::vector<DWORD> offsets)
{
	DWORD addr = ptr;
	for (int i = 0; i < offsets.size(); ++i)
	{
		addr = *(DWORD*)addr;
		addr += offsets[i];
	}
	return addr;
}

