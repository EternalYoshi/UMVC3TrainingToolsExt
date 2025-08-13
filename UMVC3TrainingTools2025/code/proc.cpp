//#include "stdafx.h"
#include "proc.h"
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

//Uses the Window Name to get the the Thread ID to get the Process ID.
static DWORD GetProcId(const wchar_t* procName)
{
	DWORD PID = 0;
	DWORD TID = 0;

	HWND Test = FindWindow(0, "ULTIMATE MARVEL VS. CAPCOM 3");
	if (Test) 
	{
		TID = GetWindowThreadProcessId(Test, &TID);
	}

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, TID);
	if (hThread != 0)
	{
		PID = GetProcessIdOfThread(hThread);
	}
	return PID;

}

//Gets The Module Base Address, which for Marvel 3 SHOULD always equal 5368709120, AKA 0x140000000.
static uintptr_t GetModuleBaseAddress(DWORD procId)
{

	DWORD_PTR   baseAddress = 0;
	HANDLE      processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
	HMODULE* moduleArray;
	LPBYTE      moduleArrayBytes;
	DWORD       bytesRequired;
	if (processHandle)
	{
		if (EnumProcessModules(processHandle, NULL, 0, &bytesRequired))
		{
			if (bytesRequired)
			{
				moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);
				if (moduleArrayBytes)
				{
					unsigned int moduleCount;
					moduleCount = bytesRequired / sizeof(HMODULE);
					moduleArray = (HMODULE*)moduleArrayBytes;
					if (EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired))
					{
						baseAddress = (DWORD_PTR)moduleArray[0];
					}
					LocalFree(moduleArrayBytes);
				}
			}
		}
		CloseHandle(processHandle);
	}
	return baseAddress;

}

static DWORD GetPointerAddress(HANDLE hProc, DWORD ptr, std::vector<DWORD> offsets)
{
	DWORD addr = ptr;
	for (int i = 0; i < offsets.size(); ++i)
	{
		addr = *(DWORD*)addr;
		addr += offsets[i];
	}
	return addr;
}

