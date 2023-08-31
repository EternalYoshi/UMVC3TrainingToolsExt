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
	ChangeWrightEvidence();
	Objection();
	PopTheBird();
	ChangeMODOKUnderstanding(MODOKLOU);
	SetDormSpellLevels();
	EndlessInstallBoolUpdate();
	SetDeadpoolTeleport();
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

//Updates things every frame.
void TickUpdates()
{
	if (EndlessXFactorP1 == true || EndlessXFactorP2 == true)
	{
		EndlessXFactorUpdate();
	}
}

//Sets Individual Character HP on Training Mode restarts.
void SetIndividualCharacterHealth()
{

	//Gets the Max HP Value for the active characters. They are ints instead of floats.
	//P1C1MaxHP = *(uintptr_t*)_addr((P1Character1Data + 0x154C));
	//P1C2MaxHP = *(uintptr_t*)_addr((P1Character2Data + 0x154C));
	//P1C3MaxHP = *(uintptr_t*)_addr((P1Character3Data + 0x154C));
	//P2C1MaxHP = *(uintptr_t*)_addr((P2Character1Data + 0x154C));
	//P2C2MaxHP = *(uintptr_t*)_addr((P2Character2Data + 0x154C));
	//P2C3MaxHP = *(uintptr_t*)_addr((P2Character3Data + 0x154C));	


	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x154C), &P1C1MaxHP, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x154C), &P1C2MaxHP, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x154C), &P1C3MaxHP, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x154C), &P2C1MaxHP, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x154C), &P2C2MaxHP, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x154C), &P2C3MaxHP, sizeof(int), 0);





	if (SetIndividualHP == true)
	{

		//P1 Character 1.
		//*(float*)_addr(P1Character1Data + 0x1550) = P1C1MaxHP * P1Char1Health;

		//P1 Character 2.
		//*(float*)_addr(P1Character2Data + 0x1550) = P1C2MaxHP * P1Char3Health;

		//P1 Character 3.
		//*(float*)_addr(P1Character3Data + 0x1550) = P1C3MaxHP * P1Char3Health;

		//P2 Character 1.
		//*(float*)_addr(P2Character1Data + 0x1550) = P2C1MaxHP * P2Char1Health;

		//P2 Character 2.
		//*(float*)_addr(P2Character2Data + 0x1550) = P2C2MaxHP * P2Char2Health;

		//P2 Character 3.
		//*(float*)_addr(P2Character3Data + 0x1550) = P2C3MaxHP * P2Char3Health;

		P1Char1HealthNew = P1C1MaxHP * P1Char1Health;
		P1Char2HealthNew = P1C2MaxHP * P1Char2Health;
		P1Char3HealthNew = P1C3MaxHP * P1Char3Health;
		P2Char1HealthNew = P2C1MaxHP * P2Char1Health;
		P2Char2HealthNew = P2C2MaxHP * P2Char2Health;
		P2Char3HealthNew = P2C3MaxHP * P2Char3Health;


		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1550), &P1Char1HealthNew, sizeof(P1Char1HealthNew), NULL))
		{

		}

		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1550), &P1Char2HealthNew, sizeof(P1Char2HealthNew), NULL))
		{

		}

		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1550), &P1Char3HealthNew, sizeof(P1Char3HealthNew), NULL))
		{

		}

		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1550), &P2Char1HealthNew, sizeof(P2Char1HealthNew), NULL))
		{

		}

		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1550), &P2Char2HealthNew, sizeof(P2Char2HealthNew), NULL))
		{

		}

		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1550), &P2Char3HealthNew, sizeof(P2Char3HealthNew), NULL))
		{

		}

		//Applies the requested values to the characters' red health as well.
		if (AlsoSetRedHealth == true)
		{
			//P1 Character 1.
			//*(float*)_addr(P1Character1Data + 0x1558) = P1C1MaxHP * P1Char1Health;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1558), &P1Char1HealthNew, sizeof(P1Char1HealthNew), NULL))
			{

			}

			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1558), &P1Char2HealthNew, sizeof(P1Char2HealthNew), NULL))
			{

			}

			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1558), &P1Char3HealthNew, sizeof(P1Char3HealthNew), NULL))
			{

			}

			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1558), &P2Char1HealthNew, sizeof(P2Char1HealthNew), NULL))
			{

			}

			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1558), &P2Char2HealthNew, sizeof(P2Char2HealthNew), NULL))
			{

			}

			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1558), &P2Char3HealthNew, sizeof(P2Char3HealthNew), NULL))
			{

			}

			//P1 Character 2.
			//*(float*)_addr(P1Character2Data + 0x1558) = P1C2MaxHP * P1Char3Health;

			//P1 Character 3.
			//*(float*)_addr(P1Character3Data + 0x1558) = P1C3MaxHP * P1Char3Health;

			//P2 Character 1.
			//*(float*)_addr(P2Character1Data + 0x1558) = P2C1MaxHP * P2Char1Health;

			//P2 Character 2.
			//*(float*)_addr(P2Character2Data + 0x1558) = P2C2MaxHP * P2Char2Health;

			//P2 Character 3.
			//*(float*)_addr(P2Character3Data + 0x1558) = P2C3MaxHP * P2Char3Health;
		}

	}



}

//Adjusts Frank's level based on the Slider's value.
void ChangeFrankLevel(int FrankLevel)
{
	GetPlayerData();
	GetCharacterIDs();
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

//Sets Evidence based on selection.
void ChangeWrightEvidence()
{
	GetPlayerData();
	GetCharacterIDs();

	//Checks Character ID.
	if (P1Character1ID == 23)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x6918), &WrightEvidenceA, sizeof(WrightEvidenceA), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x691C), &WrightEvidenceB, sizeof(WrightEvidenceB), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x6920), &WrightEvidenceC, sizeof(WrightEvidenceC), NULL))
		{

		}
	}
	if (P1Character2ID == 23)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x6918), &WrightEvidenceA, sizeof(WrightEvidenceA), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x691C), &WrightEvidenceB, sizeof(WrightEvidenceB), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x6920), &WrightEvidenceC, sizeof(WrightEvidenceC), NULL))
		{

		}
	}
	if (P1Character3ID == 23)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x6918), &WrightEvidenceA, sizeof(WrightEvidenceA), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x691C), &WrightEvidenceB, sizeof(WrightEvidenceB), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x6920), &WrightEvidenceC, sizeof(WrightEvidenceC), NULL))
		{

		}
	}
	if (P2Character1ID == 23)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x6918), &WrightEvidenceA, sizeof(WrightEvidenceA), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x691C), &WrightEvidenceB, sizeof(WrightEvidenceB), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x6920), &WrightEvidenceC, sizeof(WrightEvidenceC), NULL))
		{

		}
	}
	if (P2Character2ID == 23)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x6918), &WrightEvidenceA, sizeof(WrightEvidenceA), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x691C), &WrightEvidenceB, sizeof(WrightEvidenceB), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x6920), &WrightEvidenceC, sizeof(WrightEvidenceC), NULL))
		{

		}
	}
	if (P2Character3ID == 23)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x6918), &WrightEvidenceA, sizeof(WrightEvidenceA), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x691C), &WrightEvidenceB, sizeof(WrightEvidenceB), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x6920), &WrightEvidenceC, sizeof(WrightEvidenceC), NULL))
		{

		}
	}

}

int EvidenceNameToID(const char* name)
{

	if (name == "Nothing")
	{
		return -1;
	}
	else if (name == "Photo")
	{
		return 0;
	}
	else if (name == "Watch")
	{
		return 1;
	}
	else if (name == "Vase")
	{
		return 2;
	}
	else if (name == "Folder")
	{
		return 3;
	}
	else if (name == "Phone")
	{
		return 4;
	}
	else if (name == "Knife")
	{
		return 5;
	}
	else if (name == "Sunglasses")
	{
		return 6;
	}
	else if (name == "Beer")
	{
		return 7;
	}
	else if (name == "Bonsai")
	{
		return 8;
	}
	else if (name == "Doll")
	{
		return 9;
	}
	else if (name == "Figurine")
	{
		return 10;
	}
	else if (name == "Plunger")
	{
		return 11;
	}
	else if (name == "Chicken")
	{
		return 12;
	}

	return -1;

}

//Rough version of Turnabout Mode.
void Objection()
{
	int TBOn = 2;
	int TBOff = 0;
	GetPlayerData();
	GetCharacterIDs();

	if (P1Character1ID == 23)
	{
		if (Turnabout == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F0), &TurnaboutMode, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1548), &TBOn, sizeof(TBOn), NULL))
			{

			}		
		}
		else
		{			
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1548), &TBOff, sizeof(TBOff), NULL))
			{

			}
		}
	}
	if (P1Character2ID == 23)
	{
		if (Turnabout == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15F0), &TurnaboutMode, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1548), &TBOn, sizeof(TBOn), NULL))
			{

			}
		}
		else
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1548), &TBOff, sizeof(TBOff), NULL))
			{

			}
		}
	}
	if (P1Character3ID == 23)
	{
		if (Turnabout == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15F0), &TurnaboutMode, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1548), &TBOn, sizeof(TBOn), NULL))
			{

			}
		}
		else
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1548), &TBOff, sizeof(TBOff), NULL))
			{

			}
		}
	}
	if (P2Character1ID == 23)
	{
		if (Turnabout == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15F0), &TurnaboutMode, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1548), &TBOn, sizeof(TBOn), NULL))
			{

			}
		}
		else
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1548), &TBOff, sizeof(TBOff), NULL))
			{

			}
		}
	}
	if (P2Character2ID == 23)
	{
		if (Turnabout == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15F0), &TurnaboutMode, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1548), &TBOn, sizeof(TBOn), NULL))
			{

			}
		}
		else
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1548), &TBOff, sizeof(TBOff), NULL))
			{

			}
		}
	}
	if (P2Character3ID == 23)
	{
		if (Turnabout == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15F0), &TurnaboutMode, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1548), &TBOn, sizeof(TBOn), NULL))
			{

			}
		}
		else
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1548), &TBOff, sizeof(TBOff), NULL))
			{

			}
		}
	}

}

void PopTheBird()
{
	int DPOnA = 1;
	int DPOnB = 0;
	int DPOffA = 0;
	int DPOffB = 16;

	GetPlayerData();
	GetCharacterIDs();

	if (P1Character1ID == 36)
	{
		if (DarkPhoenix == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F0), &DarkPhoenixInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x6930), &DPOnA, sizeof(DPOnA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1548), &DPOnA, sizeof(DPOnA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1588), &DPOnB, sizeof(DPOnB), NULL))
			{

			}
		}
		else
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x6930), &DPOffA, sizeof(DPOffA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1548), &DPOffA, sizeof(DPOffA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1588), &DPOffB, sizeof(DPOffB), NULL))
			{

			}
		}
	}

	if (P1Character2ID == 36)
	{
		if (DarkPhoenix == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15F0), &DarkPhoenixInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x6930), &DPOnA, sizeof(DPOnA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1548), &DPOnA, sizeof(DPOnA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1588), &DPOnB, sizeof(DPOnB), NULL))
			{

			}
		}
		else
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x6930), &DPOffA, sizeof(DPOffA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1548), &DPOffA, sizeof(DPOffA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1588), &DPOffB, sizeof(DPOffB), NULL))
			{

			}
		}
	}

	if (P1Character3ID == 36)
	{
		if (DarkPhoenix == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15F0), &DarkPhoenixInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x6930), &DPOnA, sizeof(DPOnA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1548), &DPOnA, sizeof(DPOnA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1588), &DPOnB, sizeof(DPOnB), NULL))
			{

			}
		}
		else
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x6930), &DPOffA, sizeof(DPOffA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1548), &DPOffA, sizeof(DPOffA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1588), &DPOffB, sizeof(DPOffB), NULL))
			{

			}
		}
	}

	if (P2Character1ID == 36)
	{
		if (DarkPhoenix == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15F0), &DarkPhoenixInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x6930), &DPOnA, sizeof(DPOnA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1548), &DPOnA, sizeof(DPOnA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1588), &DPOnB, sizeof(DPOnB), NULL))
			{

			}
		}
		else
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x6930), &DPOffA, sizeof(DPOffA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1548), &DPOffA, sizeof(DPOffA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1588), &DPOffB, sizeof(DPOffB), NULL))
			{

			}
		}
	}

	if (P2Character2ID == 36)
	{
		if (DarkPhoenix == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15F0), &DarkPhoenixInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x6930), &DPOnA, sizeof(DPOnA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1548), &DPOnA, sizeof(DPOnA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1588), &DPOnB, sizeof(DPOnB), NULL))
			{

			}
		}
		else
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x6930), &DPOffA, sizeof(DPOffA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1548), &DPOffA, sizeof(DPOffA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1588), &DPOffB, sizeof(DPOffB), NULL))
			{

			}
		}
	}

	if (P2Character3ID == 36)
	{
		if (DarkPhoenix == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15F0), &DarkPhoenixInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x6930), &DPOnA, sizeof(DPOnA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1548), &DPOnA, sizeof(DPOnA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1588), &DPOnB, sizeof(DPOnB), NULL))
			{

			}
		}
		else
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x6930), &DPOffA, sizeof(DPOffA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1548), &DPOffA, sizeof(DPOffA), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1588), &DPOffB, sizeof(DPOffB), NULL))
			{

			}
		}
	}

}

//Adjusts MODOK's level of Understanding.
void ChangeMODOKUnderstanding(int MODOKLOU)
{
	GetPlayerData();
	GetCharacterIDs();

	if (P1Character1ID == 38)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68F0), &MODOKLOU, sizeof(MODOKLOU), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68F4), &MODOKLOU, sizeof(MODOKLOU), NULL))
		{

		}
		//*((int*)(P1Character1Data + 0x68F0)) = MODOKLOU;
		//*((int*)(P1Character1Data + 0x68F4)) = MODOKLOU;
	}

	if (P1Character2ID == 38)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x68F0), &MODOKLOU, sizeof(MODOKLOU), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x68F4), &MODOKLOU, sizeof(MODOKLOU), NULL))
		{

		}
	}

	if (P1Character3ID == 38)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x68F0), &MODOKLOU, sizeof(MODOKLOU), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x68F4), &MODOKLOU, sizeof(MODOKLOU), NULL))
		{

		}
	}

	if (P2Character1ID == 38)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x68F0), &MODOKLOU, sizeof(MODOKLOU), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x68F4), &MODOKLOU, sizeof(MODOKLOU), NULL))
		{

		}
	}

	if (P2Character2ID == 38)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x68F0), &MODOKLOU, sizeof(MODOKLOU), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x68F4), &MODOKLOU, sizeof(MODOKLOU), NULL))
		{

		}
	}

	if (P2Character3ID == 38)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x68F0), &MODOKLOU, sizeof(MODOKLOU), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x68F4), &MODOKLOU, sizeof(MODOKLOU), NULL))
		{

		}
	}
}

//Sets Dormammu Spell Charges.
void SetDormSpellLevels()
{
	GetPlayerData();
	GetCharacterIDs();
	//Sets Both Spell Values.
	if (P1Character1ID == 39)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68F0), &DormRed, sizeof(DormRed), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68F4), &DormBlue, sizeof(DormBlue), NULL))
		{

		}
		//uintptr_t P1Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0);
		//*((int*)(P1Character1Data + 0x68F0)) = DormRed;
		//*((int*)(P1Character1Data + 0x68F4)) = DormBlue;
	}

	if (P1Character2ID == 39)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x68F0), &DormRed, sizeof(DormRed), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x68F4), &DormBlue, sizeof(DormBlue), NULL))
		{

		}
	}

	if (P1Character3ID == 39)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x68F0), &DormRed, sizeof(DormRed), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x68F4), &DormBlue, sizeof(DormBlue), NULL))
		{

		}
	}

	if (P2Character1ID == 39)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x68F0), &DormRed, sizeof(DormRed), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x68F4), &DormBlue, sizeof(DormBlue), NULL))
		{

		}
	}

	if (P2Character2ID == 39)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x68F0), &DormRed, sizeof(DormRed), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x68F4), &DormBlue, sizeof(DormBlue), NULL))
		{

		}
	}

	if (P2Character3ID == 39)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x68F0), &DormRed, sizeof(DormRed), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x68F4), &DormBlue, sizeof(DormBlue), NULL))
		{

		}
	}



}

//Sets Infnite Install Time for Several Characters.
void EndlessInstallBoolUpdate()
{
	float flo = 21600.0;
	char FlagA = 0;
	GetPlayerData();
	GetCharacterIDs();
	//Checks for these characters as these are the ones compatible with this method.
	if (P1Character1ID == 1 || P1Character1ID == 7 || P1Character1ID == 11 || P1Character1ID == 13 || P1Character1ID == 15 || P1Character1ID == 16
		|| P1Character1ID == 17 || P1Character1ID == 20 || P1Character1ID == 21 || P1Character1ID == 22 || P1Character1ID == 23 || P1Character1ID == 24 || P1Character1ID == 28
		|| P1Character1ID == 37 || P1Character1ID == 44)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F8), &FlagA, sizeof(FlagA), 0);
		if (FlagA == 17 || FlagA == 16)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1600), &flo, sizeof(flo), NULL))
			{

			}
		}
	}

	//Checks for these characters as these are the ones compatible with this method.
	if (P1Character2ID == 1 || P1Character2ID == 7 || P1Character2ID == 11 || P1Character2ID == 13 || P1Character2ID == 15 || P1Character2ID == 16
		|| P1Character2ID == 17 || P1Character2ID == 20 || P1Character2ID == 21 || P1Character2ID == 22 || P1Character2ID == 23 || P1Character2ID == 24 || P1Character2ID == 28
		|| P1Character2ID == 37 || P1Character2ID == 44)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15F8), &FlagA, sizeof(FlagA), 0);
		if (FlagA == 17 || FlagA == 16)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1600), &flo, sizeof(flo), NULL))
			{

			}
		}
	}

	//Checks for these characters as these are the ones compatible with this method.
	if (P1Character3ID == 1 || P1Character3ID == 7 || P1Character3ID == 11 || P1Character3ID == 13 || P1Character3ID == 15 || P1Character3ID == 16
		|| P1Character3ID == 17 || P1Character3ID == 20 || P1Character3ID == 21 || P1Character3ID == 22 || P1Character3ID == 23 || P1Character3ID == 24 || P1Character3ID == 28
		|| P1Character3ID == 37 || P1Character3ID == 44)
	{

		ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15F8), &FlagA, sizeof(FlagA), 0);
		if (FlagA == 17 || FlagA == 16)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1600), &flo, sizeof(flo), NULL))
			{

			}
		}
	}

	//Checks for these characters as these are the ones compatible with this method.
	if (P2Character1ID == 1 || P2Character1ID == 7 || P2Character1ID == 11 || P2Character1ID == 13 || P2Character1ID == 15 || P2Character1ID == 16
		|| P2Character1ID == 17 || P2Character1ID == 20 || P2Character1ID == 21 || P2Character1ID == 22 || P2Character1ID == 23 || P2Character1ID == 24 || P2Character1ID == 28
		|| P2Character1ID == 37 || P2Character1ID == 44)
	{

		ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15F8), &FlagA, sizeof(FlagA), 0);
		if (FlagA == 17 || FlagA == 16)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1600), &flo, sizeof(flo), NULL))
			{

			}
		}
	}

	//Checks for these characters as these are the ones compatible with this method.
	if (P2Character2ID == 1 || P2Character2ID == 7 || P2Character2ID == 11 || P2Character2ID == 13 || P2Character2ID == 15 || P2Character2ID == 16
		|| P2Character2ID == 17 || P2Character2ID == 20 || P2Character2ID == 21 || P2Character2ID == 22 || P2Character2ID == 23 || P2Character2ID == 24 || P2Character2ID == 28
		|| P2Character2ID == 37 || P2Character2ID == 44)
	{

		ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15F8), &FlagA, sizeof(FlagA), 0);
		if (FlagA == 17 || FlagA == 16)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1600), &flo, sizeof(flo), NULL))
			{

			}
		}
	}

	//Checks for these characters as these are the ones compatible with this method.
	if (P2Character3ID == 1 || P2Character3ID == 7 || P2Character3ID == 11 || P2Character3ID == 13 || P2Character3ID == 15 || P2Character3ID == 16
		|| P2Character3ID == 17 || P2Character3ID == 20 || P2Character3ID == 21 || P2Character3ID == 22 || P2Character3ID == 23 || P2Character3ID == 24 || P2Character3ID == 28
		|| P2Character3ID == 37 || P2Character3ID == 44)
	{

		ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15F8), &FlagA, sizeof(FlagA), 0);
		if (FlagA == 17 || FlagA == 16)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1600), &flo, sizeof(flo), NULL))
			{

			}
		}
	}

}

void SetDeadpoolTeleport()
{
	GetPlayerData();
	GetCharacterIDs();
	if (P1Character1ID == 40)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68F0), &DeadpoolTeleportCount, sizeof(DeadpoolTeleportCount), NULL))
		{

		}
	}

	if (P1Character2ID == 40)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x68F0), &DeadpoolTeleportCount, sizeof(DeadpoolTeleportCount), NULL))
		{

		}
	}

	if (P1Character3ID == 40)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x68F0), &DeadpoolTeleportCount, sizeof(DeadpoolTeleportCount), NULL))
		{

		}
	}

	if (P2Character1ID == 40)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x68F0), &DeadpoolTeleportCount, sizeof(DeadpoolTeleportCount), NULL))
		{

		}
	}

	if (P2Character2ID == 40)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x68F0), &DeadpoolTeleportCount, sizeof(DeadpoolTeleportCount), NULL))
		{

		}
	}

	if (P2Character3ID == 40)
	{
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x68F0), &DeadpoolTeleportCount, sizeof(DeadpoolTeleportCount), NULL))
		{

		}
	}


}
