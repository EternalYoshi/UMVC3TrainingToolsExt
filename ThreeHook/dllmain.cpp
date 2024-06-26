// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <chrono> 
#include <thread>
#include "gui/dx9hook.h"
#include "gui/log.h"
#include "gui/notifications.h"

#include "plugin/Menu.h"

#include "plugin/Settings.h"
#include "plugin/Hooks.h"


#include "utils/MemoryMgr.h"
#include "utils/Trampoline.h"
#include "utils/Patterns.h"
#include "umvc3/Internals.h"


using namespace Memory::VP;
// TODO: patterns


void OnInitializeHook()
{

//#ifdef DEBUG
	if (SettingsMgr->bEnableConsoleWindow)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}
	eLog::Message(__FUNCTION__, "INFO: UMVC3Hook Begin!");
//#endif

	Notifications->Init();

	Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));

}

bool CheckGame()
{
	char* gameName = (char*)(_addr(0x140B12D10));

	if (strcmp(gameName, "umvc3") == 0)
	{
		return true;
	}
	else
	{
		MessageBoxA(0, "Invalid game version!\nUMVC3Hook only supports latest (or it needs to be updated) Steam executable.", 0, MB_ICONINFORMATION);
		return false;
	}
}

extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		eLog::Initialize();

		if (CheckGame())
		{
			OnInitializeHook();
		}

	}
}


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		GUIImplementationDX9::Shutdown();
		break;
	}
	return TRUE;
}




