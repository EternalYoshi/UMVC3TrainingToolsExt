#include "umvc3utils.h"

#include "proc.h"
#include "gui.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include "..\utils\addr.h"

/*
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
*/

bool CheckGame()
{
	char gameName[5] = { 0 };

	VirtualProtect((LPVOID*)0x140B12D10, 5, PAGE_EXECUTE_READWRITE, &oldPageProtection);

	ReadProcessMemory(hProcess, (LPVOID*)0x140B12D10, &gameName, sizeof(gameName), 0);

	VirtualProtect((LPVOID*)0x140B12D10, 5, oldPageProtection, &oldPageProtection);

	if (strncmp(gameName, "umvc3", 5) == 0)
	{
		return true;
	}
	else
	{
		MessageBoxA(0, "Invalid game version!\nUMVC3Hook only supports latest (or it needs to be updated) Steam executable.", 0, MB_ICONINFORMATION);
		return false;
	}

	return true;

}

DWORD SingleWrite(DWORD64 AddressToWrite, int length, HANDLE hProcess, BYTE ByteToWrite, DWORD errorMessageID, LPSTR messageBuffer)
{
	if (!WriteProcessMemory(hProcess, (BYTE*)(AddressToWrite), &ByteToWrite, length, NULL))
	{

		errorMessageID = ::GetLastError();

		messageBuffer = nullptr;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);


		return -1;
	}
	else
	{

	}

	return 0;
}

DWORD MultiWrite(DWORD64 AddressToWrite, int length, HANDLE hProcess, BYTE ByteToWrite[], DWORD errorMessageID, LPSTR messageBuffer)
{
	if (!WriteProcessMemory(hProcess, (BYTE*)(AddressToWrite), ByteToWrite, length, NULL))
	{

		errorMessageID = ::GetLastError();

		messageBuffer = nullptr;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);



		return -1;

	}
	else
	{
	}
	return 0;
}

DWORD ProtectedSingleWrite(DWORD64 AddressToWrite, int length, HANDLE hProcess, BYTE ByteToWrite, DWORD errorMessageID, LPSTR messageBuffer, DWORD oldPageProtection)
{
	oldPageProtection = 0;

	VirtualProtect((LPVOID)AddressToWrite, 4, PAGE_EXECUTE_READWRITE, &oldPageProtection);


	if (!WriteProcessMemory(hProcess, (LPVOID*)(AddressToWrite), &ByteToWrite, 1, NULL))
	{
		errorMessageID = ::GetLastError();

		messageBuffer = nullptr;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);



		return -1;

	}
	else
	{
	}

	VirtualProtect((LPVOID)AddressToWrite, 4, oldPageProtection, &oldPageProtection);

	return 0;
}

DWORD ProtectedMultiWrite(DWORD64 AddressToWrite, int length, HANDLE hProcess, BYTE ByteToWrite[], DWORD errorMessageID, LPSTR messageBuffer, DWORD oldPageProtection)
{
	oldPageProtection = 0;

	VirtualProtect((LPVOID)AddressToWrite, length, PAGE_EXECUTE_READWRITE, &oldPageProtection);

	//std::cout << "Attempting to write to Address " << std::hex << AddressToWrite << std::endl;

	if (!WriteProcessMemory(hProcess, (LPVOID*)(AddressToWrite), ByteToWrite, length, NULL))
	{
		errorMessageID = ::GetLastError();

		messageBuffer = nullptr;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);



		return -1;

	}
	else
	{
	}

	VirtualProtect((LPVOID)AddressToWrite, length, oldPageProtection, &oldPageProtection);

	return 0;
}

bool CheckTheMode()
{
	//Reads The Needed Pointer.
	ReadProcessMemory(hProcess, (LPVOID*)0x140d50e58, &StrangeTable, sizeof(uintptr_t), 0);

	//Reads the match/menu flag.
	ReadProcessMemory(hProcess, (LPVOID*)(StrangeTable + 0x34C), &GameMode, sizeof(int), 0);

	if (GameMode == 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Gets the Pointers to the Tables needed for everything else.
void GetMainPointers()
{
	if (GameMode == 5)
	{
		//Reads The Needed Pointers.
		ReadProcessMemory(hProcess, (LPVOID*)0x140d533e0, &mysterytable, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)0x140d47e68, &block2, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)0x140d50e58, &ptable, sizeof(uintptr_t), 0);		
		team1ptr = block2 + 0x350;
		team2ptr = block2 + 0x610;
		//ReadProcessMemory(hProcess, (LPVOID*)(TempA), &team1ptr, sizeof(uintptr_t), 0);
		//ReadProcessMemory(hProcess, (LPVOID*)(TempB), &team2ptr, sizeof(uintptr_t), 0);
		//ReadProcessMemory(hProcess, (LPVOID*)(block2 + 0x350), &team1ptr, sizeof(uintptr_t), 0);
		//ReadProcessMemory(hProcess, (LPVOID*)(block2 + 0x610), &team2ptr, sizeof(uintptr_t), 0);
	}
}

//Gets the active characters based on the Table Pointers.
void GetCharacterIDs()
{
	if (GameMode == 5)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(ptable + 0x44), &P1Character1ID, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(ptable + 0x44 + 0x58), &P1Character2ID, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(ptable + 0x44 + 0x58 + 0x58), &P1Character3ID, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(ptable + 0x44 + 0x58 + 0x58 + 0x58), &P2Character1ID, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58), &P2Character2ID, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58 + 0x58), &P2Character3ID, sizeof(int), 0);
	}
}

//Gets the Active Player pointers.
void GetPlayerData()
{
	if (GameMode == 5)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(mysterytable + 0xAA0), &P1Character1Data, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(mysterytable + 0xAA0 + (0x438)), &P1Character2Data, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(mysterytable + 0xAA0 + (0x438 * 2)), &P1Character3Data, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(mysterytable + 0xAA0 + (0x438 * 3)), &P2Character1Data, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(mysterytable + 0xAA0 + (0x438 * 4)), &P2Character2Data, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(mysterytable + 0xAA0 + (0x438 * 5)), &P2Character3Data, sizeof(uintptr_t), 0);
	}
}

//Carries over changes To restart.
void RestartWithChanges()
{
	ChangeFrankLevel(FrankLevel);
	//ChangeWrightEvidence();
	//ChangeMODOKUnderstanding(MODOKLOU);
	//SetDormSpellLevels();
	//EndlessInstallBoolUpdate();
	//SetDeadpoolTeleport();
}

//Forces the X Factor Timer to stay at a stupid high time to be unlimited.
void EndlessXFactorUpdate()
{

	float fli = 7000.0;

	//Team1.
	//*(float*)_addr(team1ptr + 0xC0) = 7000.0;
	if (EndlessXFactorP1) {
		if (!WriteProcessMemory(hProcess, (LPVOID*)(team1ptr + 0xC0), &fli, sizeof(float), NULL))
		{

		}
	}
	//Team2.
	//*(float*)_addr(team2ptr + 0xC0) = 7000.0;
	if (EndlessXFactorP2)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(team2ptr + 0xC0), &fli, sizeof(float), NULL))
		{

		}
	}



}

void TickUpdates()
{
	if (EndlessXFactorP1 == true || EndlessXFactorP2 == true)
	{
		EndlessXFactorUpdate();
	}
}


//Adjusts Frank's level based on the Slider's value.
void ChangeFrankLevel(int FrankLevel)
{
	GetCharacterIDs();
	GetPlayerData();
	//Sets Prestige Points based on level chosen.
	switch (FrankLevel)
	{
	case 1:
		PrestigePoints = 0.0;
		break;
	case 2:
		PrestigePoints = 5.0;
		break;
	case 3:
		PrestigePoints = 20.0;
		break;
	case 4:
		PrestigePoints = 50.0;
		break;
	case 5:
		PrestigePoints = 100.0;
		break;
	default:
		PrestigePoints = 0.0;
		break;

	}

	if (P1Character1ID == 9)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x69C4), &FrankLevel, sizeof(FrankLevel), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x69DC), &PrestigePoints, sizeof(PrestigePoints), NULL))
		{

		}
	}

	if (P1Character2ID == 9)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x69C4), &FrankLevel, sizeof(FrankLevel), NULL))
		{
			MessageBox(NULL, "An error has occurred!\n", "Title!", MB_ICONERROR | MB_OK);
		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x69DC), &PrestigePoints, sizeof(PrestigePoints), NULL))
		{
			MessageBox(NULL, "An error has occurred!\n", "Title!", MB_ICONERROR | MB_OK);
		}
	}

	if (P1Character3ID == 9)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x69C4), &FrankLevel, sizeof(FrankLevel), NULL))
		{
			MessageBox(NULL, "An error has occurred!\n", "Title!", MB_ICONERROR | MB_OK);
		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x69DC), &PrestigePoints, sizeof(PrestigePoints), NULL))
		{
			MessageBox(NULL, "An error has occurred!\n", "Title!", MB_ICONERROR | MB_OK);
		}
	}

	if (P2Character1ID == 9)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x69C4), &FrankLevel, sizeof(FrankLevel), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x69DC), &PrestigePoints, sizeof(PrestigePoints), NULL))
		{

		}
	}

	if (P2Character2ID == 9)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x69C4), &FrankLevel, sizeof(FrankLevel), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x69DC), &PrestigePoints, sizeof(PrestigePoints), NULL))
		{

		}
	}

	if (P2Character3ID == 9)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x69C4), &FrankLevel, sizeof(FrankLevel), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x69DC), &PrestigePoints, sizeof(PrestigePoints), NULL))
		{

		}
	}
}


