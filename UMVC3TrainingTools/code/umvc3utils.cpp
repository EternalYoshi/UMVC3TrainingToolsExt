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
		MessageBoxA(0, "Bad News.\nThis tool only supports latest (or at least the version from 2017) Steam executable\nof Ultimate Marvel vs Capcom 3.", 0, MB_ICONINFORMATION);
		exit(1);
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

		//Gets better Pointers.
		ReadProcessMemory(hProcess, (LPVOID*)0x140D44A70, &TysonTable, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(TysonTable + 0x58), &Player1TeamTable, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(TysonTable + 0x328), &Player2TeamTable, sizeof(uintptr_t), 0);



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

		uintptr_t TempA = 0;
		uintptr_t TempB = 0;
		ReadProcessMemory(hProcess, (LPVOID*)(Player1TeamTable + 0x10), &TempA, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(Player2TeamTable + 0x10), &TempB, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(Player1TeamTable + 0x8), &P1Character1Data, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(TempA + 0x8), &P1Character2Data, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x18), &P1Character3Data, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(Player2TeamTable + 0x8), &P2Character1Data, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(TempB + 0x8), &P2Character2Data, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x18), &P2Character3Data, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data), scriptableFighters[0].fighter, sizeof(Fighter), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data), scriptableFighters[1].fighter, sizeof(Fighter), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data), scriptableFighters[2].fighter, sizeof(Fighter), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data), scriptableFighters[3].fighter, sizeof(Fighter), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data), scriptableFighters[4].fighter, sizeof(Fighter), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data), scriptableFighters[5].fighter, sizeof(Fighter), 0);

		const char* name = nullptr;
		/*
		if (scriptableFighters[0].name == nullptr) {
			scriptableFighters[0].name = new char[strlen(name) + 1] {0};
			memcpy(scriptableFighters[0].name, name, strlen(name));
		}
		if (scriptableFighters[1].name == nullptr) {
			scriptableFighters[1].name = new char[strlen(name) + 1] {0};
			memcpy(scriptableFighters[1].name, name, strlen(name));
		}
		if (scriptableFighters[2].name == nullptr) {
			scriptableFighters[2].name = new char[strlen(name) + 1] {0};
			memcpy(scriptableFighters[2].name, name, strlen(name));
		}
		if (scriptableFighters[3].name == nullptr) {
			scriptableFighters[3].name = new char[strlen(name) + 1] {0};
			memcpy(scriptableFighters[3].name, name, strlen(name));
		}
		if (scriptableFighters[4].name == nullptr) {
			scriptableFighters[4].name = new char[strlen(name) + 1] {0};
			memcpy(scriptableFighters[4].name, name, strlen(name));
		}
		if (scriptableFighters[5].name == nullptr) {
			scriptableFighters[5].name = new char[strlen(name) + 1] {0};
			memcpy(scriptableFighters[5].name, name, strlen(name));
		}
		*/


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

	if (DormSpellSet == true)
	{
		if (CheckTheMode() == true)
		{
			SetDormSpellLevels();
		}
	}

	if (EndlessInstalls == true)
	{
		if (CheckTheMode() == true)
		{
			EndlessInstallBoolUpdate();
		}
	}

	if (FreezeDeadpoolTPCounter == true)
	{
		if (CheckTheMode() == true)
		{
			SetDeadpoolTeleport();
		}
	}

	if (DarkPhoenix == true)
	{
		PopTheBird();
	}

	if (Turnabout == true)
	{
		Objection();
	}

	if (LockMODOKLOU == true)
	{
		ChangeMODOKUnderstanding(MODOKLOU);
	}

	if(P1C1Jammed == true || P1C2Jammed == true || P1C3Jammed == true || P2C1Jammed == true || P2C2Jammed == true || P2C3Jammed == true)
	{
		JammingToggle();
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

	if (P1Character1ID != 0)
	{
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

void ErrorOccured(DWORD errorMessageID)
{
	//MessageBox(hWnd, errorMessageID, "An Error Occured", MB_OK | MB_ICONERROR);


}

void FUN_1402b41b0(longlong param_1)
{
	switch (RecordingSlot)
	{

	case 2:

		printf("replay %x\n", param_1);
		if (!inputRefSet) {
			inputRefSet = true;
			inputRef = (uintptr_t)param_1;
			printf("replayu %x\n", param_1);
		}

		((void (*)(longlong))_addr(0x1402b41b0))(param_1);
		if (inputRefSet) {
			if (recording) {
				printf("recording %d\n", recordReplayIndex2);
				memcpy(&replayBuffer3[recordReplayIndex2], (uint8_t*)inputRef, ReplayBufferSize);
				memcpy(&replayBuffer4[recordReplayIndex2], (uint8_t*)inputRef + 0x2c0, ReplayBufferSize);
				recordReplayIndex2++;
				recordReplayIndex2P1++;
				recordReplayIndex2P2++;
				replayAvailable2 = true;
				if (recordReplayIndex >= ReplayLength - 1) {
					recording = false;
					recordedLength2 = recordReplayIndex2 - 1;
				}
			}
			if (recordingP1) {
				printf("recording %d\n", recordReplayIndex2P1);
				memcpy(&replayBuffer3[recordReplayIndex2P1], (uint8_t*)inputRef, ReplayBufferSize);
				recordReplayIndex2P1++;
				replayAvailable2P1 = true;
				if (recordReplayIndex2P1 >= ReplayLength - 1) {
					recordingP1 = false;
					recordedLength2P1 = recordReplayIndex2P1 - 1;
				}
			}
			if (recordingP2) {
				printf("recording %d\n", recordReplayIndexP2);
				memcpy(&replayBuffer4[recordReplayIndex2P2], (uint8_t*)inputRef + 0x2c0, ReplayBufferSize);
				recordReplayIndex2P2++;
				replayAvailable2P2 = true;
				if (recordReplayIndex2P2 >= ReplayLength - 1) {
					recordingP2 = false;
					recordedLength2P2 = recordReplayIndex2P2 - 1;
				}
			}
			if (replaying) {
				printf("replaying %d\n", recordReplayIndex2);
				memcpy((uint8_t*)inputRef, &replayBuffer3[recordReplayIndex2], ReplayBufferSize);
				memcpy((uint8_t*)inputRef + 0x2c0, &replayBuffer4[recordReplayIndex2], ReplayBufferSize);
				recordReplayIndex2P1++;
				recordReplayIndex2P2++;
				if (recordReplayIndex2 >= recordedLength2) {
					replaying = false;
				}
				if (recordReplayIndex2P1 >= recordedLength2) {
					replayingP1 = false;
				}
				if (recordReplayIndex2P2 >= recordedLength2) {
					replayingP2 = false;
				}
			}
			if (replayingP1) {
				printf("replaying %d\n", recordReplayIndex2P1);
				memcpy((uint8_t*)inputRef, &replayBuffer3[recordReplayIndex2P1], ReplayBufferSize);
				recordReplayIndex2P1++;
				if (recordReplayIndex2P1 >= recordedLength2P1) {
					replayingP1 = false;
				}
			}
			if (replayingP2) {
				printf("replaying %d\n", recordReplayIndex2P2);
				memcpy((uint8_t*)inputRef + 0x2c0, &replayBuffer4[recordReplayIndex2P2], ReplayBufferSize);
				recordReplayIndex2P2++;
				if (recordReplayIndex2P2 >= recordedLength2P2) {
					replayingP2 = false;
				}
			}

		}

		break;

	case 3:

		printf("replay %x\n", param_1);
		if (!inputRefSet) {
			inputRefSet = true;
			inputRef = (uintptr_t)param_1;
			printf("replayu %x\n", param_1);
		}

		((void (*)(longlong))_addr(0x1402b41b0))(param_1);
		if (inputRefSet) {
			if (recording) {
				printf("recording %d\n", recordReplayIndex3);
				memcpy(&replayBuffer5[recordReplayIndex3], (uint8_t*)inputRef, ReplayBufferSize);
				memcpy(&replayBuffer6[recordReplayIndex3], (uint8_t*)inputRef + 0x2c0, ReplayBufferSize);
				recordReplayIndex3++;
				recordReplayIndex3P1++;
				recordReplayIndex3P2++;
				replayAvailable3 = true;
				if (recordReplayIndex >= ReplayLength - 1) {
					recording = false;
					recordedLength3 = recordReplayIndex3 - 1;
				}
			}
			if (recordingP1) {
				printf("recording %d\n", recordReplayIndex3P1);
				memcpy(&replayBuffer5[recordReplayIndex3P1], (uint8_t*)inputRef, ReplayBufferSize);
				recordReplayIndex3P1++;
				replayAvailable3P1 = true;
				if (recordReplayIndex3P1 >= ReplayLength - 1) {
					recordingP1 = false;
					recordedLength3P1 = recordReplayIndex3P1 - 1;
				}
			}
			if (recordingP2) {
				printf("recording %d\n", recordReplayIndexP2);
				memcpy(&replayBuffer6[recordReplayIndex3P2], (uint8_t*)inputRef + 0x2c0, ReplayBufferSize);
				recordReplayIndex3P2++;
				replayAvailable3P2 = true;
				if (recordReplayIndex3P2 >= ReplayLength - 1) {
					recordingP2 = false;
					recordedLength3P2 = recordReplayIndex3P2 - 1;
				}
			}
			if (replaying) {
				printf("replaying %d\n", recordReplayIndex3);
				memcpy((uint8_t*)inputRef, &replayBuffer5[recordReplayIndex3], ReplayBufferSize);
				memcpy((uint8_t*)inputRef + 0x2c0, &replayBuffer6[recordReplayIndex3], ReplayBufferSize);
				recordReplayIndex3P1++;
				recordReplayIndex3P2++;
				if (recordReplayIndex3 >= recordedLength3) {
					replaying = false;
				}
				if (recordReplayIndex3P1 >= recordedLength3) {
					replayingP1 = false;
				}
				if (recordReplayIndex3P2 >= recordedLength3) {
					replayingP2 = false;
				}
			}
			if (replayingP1) {
				printf("replaying %d\n", recordReplayIndex3P1);
				memcpy((uint8_t*)inputRef, &replayBuffer5[recordReplayIndex3P1], ReplayBufferSize);
				recordReplayIndex3P1++;
				if (recordReplayIndex3P1 >= recordedLength3P1) {
					replayingP1 = false;
				}
			}
			if (replayingP2) {
				printf("replaying %d\n", recordReplayIndex3P2);
				memcpy((uint8_t*)inputRef + 0x2c0, &replayBuffer6[recordReplayIndex3P2], ReplayBufferSize);
				recordReplayIndex3P2++;
				if (recordReplayIndex3P2 >= recordedLength3P2) {
					replayingP2 = false;
				}
			}

		}

		break;

	case 4:

		printf("replay %x\n", param_1);
		if (!inputRefSet) {
			inputRefSet = true;
			inputRef = (uintptr_t)param_1;
			printf("replayu %x\n", param_1);
		}

		((void (*)(longlong))_addr(0x1402b41b0))(param_1);
		if (inputRefSet) {
			if (recording) {
				printf("recording %d\n", recordReplayIndex4);
				memcpy(&replayBuffer7[recordReplayIndex4], (uint8_t*)inputRef, ReplayBufferSize);
				memcpy(&replayBuffer8[recordReplayIndex4], (uint8_t*)inputRef + 0x2c0, ReplayBufferSize);
				recordReplayIndex4++;
				recordReplayIndex4P1++;
				recordReplayIndex4P2++;
				replayAvailable4 = true;
				if (recordReplayIndex >= ReplayLength - 1) {
					recording = false;
					recordedLength4 = recordReplayIndex4 - 1;
				}
			}
			if (recordingP1) {
				printf("recording %d\n", recordReplayIndex4P1);
				memcpy(&replayBuffer7[recordReplayIndex4P1], (uint8_t*)inputRef, ReplayBufferSize);
				recordReplayIndex4P1++;
				replayAvailable4P1 = true;
				if (recordReplayIndex4P1 >= ReplayLength - 1) {
					recordingP1 = false;
					recordedLength4P1 = recordReplayIndex4P1 - 1;
				}
			}
			if (recordingP2) {
				printf("recording %d\n", recordReplayIndexP2);
				memcpy(&replayBuffer8[recordReplayIndex4P2], (uint8_t*)inputRef + 0x2c0, ReplayBufferSize);
				recordReplayIndex4P2++;
				replayAvailable4P2 = true;
				if (recordReplayIndex4P2 >= ReplayLength - 1) {
					recordingP2 = false;
					recordedLength4P2 = recordReplayIndex4P2 - 1;
				}
			}
			if (replaying) {
				printf("replaying %d\n", recordReplayIndex4);
				memcpy((uint8_t*)inputRef, &replayBuffer7[recordReplayIndex4], ReplayBufferSize);
				memcpy((uint8_t*)inputRef + 0x2c0, &replayBuffer8[recordReplayIndex4], ReplayBufferSize);
				recordReplayIndex4P1++;
				recordReplayIndex4P2++;
				if (recordReplayIndex4 >= recordedLength4) {
					replaying = false;
				}
				if (recordReplayIndex4P1 >= recordedLength4) {
					replayingP1 = false;
				}
				if (recordReplayIndex4P2 >= recordedLength4) {
					replayingP2 = false;
				}
			}
			if (replayingP1) {
				printf("replaying %d\n", recordReplayIndex4P1);
				memcpy((uint8_t*)inputRef, &replayBuffer7[recordReplayIndex4P1], ReplayBufferSize);
				recordReplayIndex4P1++;
				if (recordReplayIndex4P1 >= recordedLength4P1) {
					replayingP1 = false;
				}
			}
			if (replayingP2) {
				printf("replaying %d\n", recordReplayIndex4P2);
				memcpy((uint8_t*)inputRef + 0x2c0, &replayBuffer8[recordReplayIndex4P2], ReplayBufferSize);
				recordReplayIndex4P2++;
				if (recordReplayIndex4P2 >= recordedLength4P2) {
					replayingP2 = false;
				}
			}

		}

		break;

	case 5:

		printf("replay %x\n", param_1);
		if (!inputRefSet) {
			inputRefSet = true;
			inputRef = (uintptr_t)param_1;
			printf("replayu %x\n", param_1);
		}

		((void (*)(longlong))_addr(0x1402b41b0))(param_1);
		if (inputRefSet) {
			if (recording) {
				printf("recording %d\n", recordReplayIndex5);
				memcpy(&replayBuffer9[recordReplayIndex5], (uint8_t*)inputRef, ReplayBufferSize);
				memcpy(&replayBuffer10[recordReplayIndex5], (uint8_t*)inputRef + 0x2c0, ReplayBufferSize);
				recordReplayIndex5++;
				recordReplayIndex5P1++;
				recordReplayIndex5P2++;
				replayAvailable5 = true;
				if (recordReplayIndex >= ReplayLength - 1) {
					recording = false;
					recordedLength5 = recordReplayIndex5 - 1;
				}
			}
			if (recordingP1) {
				printf("recording %d\n", recordReplayIndex5P1);
				memcpy(&replayBuffer9[recordReplayIndex5P1], (uint8_t*)inputRef, ReplayBufferSize);
				recordReplayIndex5P1++;
				replayAvailable5P1 = true;
				if (recordReplayIndex5P1 >= ReplayLength - 1) {
					recordingP1 = false;
					recordedLength5P1 = recordReplayIndex5P1 - 1;
				}
			}
			if (recordingP2) {
				printf("recording %d\n", recordReplayIndexP2);
				memcpy(&replayBuffer10[recordReplayIndex5P2], (uint8_t*)inputRef + 0x2c0, ReplayBufferSize);
				recordReplayIndex5P2++;
				replayAvailable5P2 = true;
				if (recordReplayIndex5P2 >= ReplayLength - 1) {
					recordingP2 = false;
					recordedLength5P2 = recordReplayIndex5P2 - 1;
				}
			}
			if (replaying) {
				printf("replaying %d\n", recordReplayIndex5);
				memcpy((uint8_t*)inputRef, &replayBuffer9[recordReplayIndex5], ReplayBufferSize);
				memcpy((uint8_t*)inputRef + 0x2c0, &replayBuffer10[recordReplayIndex5], ReplayBufferSize);
				//recordReplayIndex5++;
				recordReplayIndex5P1++;
				recordReplayIndex5P2++;
				if (recordReplayIndex5 >= recordedLength5) {
					replaying = false;
				}
				if (recordReplayIndex5P1 >= recordedLength5) {
					replayingP1 = false;
				}
				if (recordReplayIndex5P2 >= recordedLength5) {
					replayingP2 = false;
				}
			}
			if (replayingP1) {
				printf("replaying %d\n", recordReplayIndex5P1);
				memcpy((uint8_t*)inputRef, &replayBuffer9[recordReplayIndex5P1], ReplayBufferSize);
				//recordReplayIndex5++;
				recordReplayIndex5P1++;
				if (recordReplayIndex5P1 >= recordedLength5P1) {
					replayingP1 = false;
				}
			}
			if (replayingP2) {
				printf("replaying %d\n", recordReplayIndex5P2);
				memcpy((uint8_t*)inputRef + 0x2c0, &replayBuffer10[recordReplayIndex5P2], ReplayBufferSize);
				//recordReplayIndex5++;
				recordReplayIndex5P2++;
				if (recordReplayIndex5P2 >= recordedLength5P2) {
					replayingP2 = false;
				}
			}

		}

		break;

	case 1:
	default:

		printf("replay %x\n", param_1);
		if (!inputRefSet) {
			inputRefSet = true;
			inputRef = (uintptr_t)param_1;
			printf("replayu %x\n", param_1);
		}

		((void (*)(longlong))_addr(0x1402b41b0))(param_1);
		if (inputRefSet) {
			if (recording) {
				printf("recording %d\n", recordReplayIndex);
				memcpy(&replayBuffer[recordReplayIndex], (uint8_t*)inputRef, ReplayBufferSize);
				memcpy(&replayBuffer2[recordReplayIndex], (uint8_t*)inputRef + 0x2c0, ReplayBufferSize);
				recordReplayIndex++;
				recordReplayIndexP1++;
				recordReplayIndexP2++;
				replayAvailable = true;
				if (recordReplayIndex >= ReplayLength - 1) {
					recording = false;
					recordedLength = recordReplayIndex - 1;
				}
			}
			if (recordingP1) {
				printf("recording %d\n", recordReplayIndexP1);
				memcpy(&replayBuffer[recordReplayIndexP1], (uint8_t*)inputRef, ReplayBufferSize);
				recordReplayIndexP1++;
				replayAvailableP1 = true;
				if (recordReplayIndexP1 >= ReplayLength - 1) {
					recordingP1 = false;
					recordedLengthP1 = recordReplayIndexP1 - 1;
				}
			}
			if (recordingP2) {
				printf("recording %d\n", recordReplayIndexP2);
				memcpy(&replayBuffer2[recordReplayIndexP2], (uint8_t*)inputRef + 0x2c0, ReplayBufferSize);
				recordReplayIndexP2++;
				replayAvailableP2 = true;
				if (recordReplayIndexP2 >= ReplayLength - 1) {
					recordingP2 = false;
					recordedLengthP2 = recordReplayIndexP2 - 1;
				}
			}
			if (replaying) {
				printf("replaying %d\n", recordReplayIndex);
				memcpy((uint8_t*)inputRef, &replayBuffer[recordReplayIndex], ReplayBufferSize);
				memcpy((uint8_t*)inputRef + 0x2c0, &replayBuffer2[recordReplayIndex], ReplayBufferSize);
				//recordReplayIndex++;
				recordReplayIndexP1++;
				recordReplayIndexP2++;
				if (recordReplayIndex >= recordedLength) {
					replaying = false;
				}
				if (recordReplayIndexP1 >= recordedLength) {
					replayingP1 = false;
				}
				if (recordReplayIndexP2 >= recordedLength) {
					replayingP2 = false;
				}
			}
			if (replayingP1) {
				printf("replaying %d\n", recordReplayIndexP1);
				memcpy((uint8_t*)inputRef, &replayBuffer[recordReplayIndexP1], ReplayBufferSize);
				//recordReplayIndex++;
				recordReplayIndexP1++;
				if (recordReplayIndexP1 >= recordedLengthP1) {
					replayingP1 = false;
				}
			}
			if (replayingP2) {
				printf("replaying %d\n", recordReplayIndexP2);
				memcpy((uint8_t*)inputRef + 0x2c0, &replayBuffer2[recordReplayIndexP2], ReplayBufferSize);
				//recordReplayIndex++;
				recordReplayIndexP2++;
				if (recordReplayIndexP2 >= recordedLengthP2) {
					replayingP2 = false;
				}
			}

		}
		break;
	}

	//Original Code here.
	/*
	((void (*)(longlong))_addr(0x1402b41b0))(param_1);
	if (inputRefSet) {
		if (recording) {
			printf("recording %d\n", recordReplayIndex);
			memcpy(&replayBuffer[recordReplayIndex], (uint8_t*)inputRef, ReplayBufferSize);
			memcpy(&replayBuffer2[recordReplayIndex], (uint8_t*)inputRef + 0x2c0, ReplayBufferSize);
			recordReplayIndex++;
			replayAvailable = true;
			if (recordReplayIndex >= ReplayLength - 1) {
				recording = false;
				recordedLength = recordReplayIndex - 1;
			}
		}
		if (replaying) {
			printf("replaying %d\n", recordReplayIndex);
			memcpy((uint8_t*)inputRef, &replayBuffer[recordReplayIndex], ReplayBufferSize);
			memcpy((uint8_t*)inputRef + 0x2c0, &replayBuffer2[recordReplayIndex], ReplayBufferSize);
			recordReplayIndex++;
			if (recordReplayIndex >= recordedLength) {
				replaying = false;
			}
		}
	}
	*/

}

//Sets whether or not the specified characters are hit by Jamming Bomb.
void JammingToggle()
{

	if (P1C1Jammed == true)
	{
	
		if(P1C1Slot2Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C1Slot3Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C1Slot4Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C1Slot5Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}

	}

	if (P1C2Jammed == true)
	{

		if (P1C2Slot2Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C2Slot3Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C2Slot4Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C2Slot5Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}

	}

	if (P1C3Jammed == true)
	{

		if (P1C3Slot2Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C3Slot3Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C3Slot4Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C3Slot5Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}

	}

	if (P2C1Jammed == true)
	{

		if (P2C1Slot2Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C1Slot3Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C1Slot4Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C1Slot5Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}

	}

	if (P2C2Jammed == true)
	{

		if (P2C2Slot2Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C2Slot3Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C2Slot4Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C2Slot5Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}

	}

	if (P2C3Jammed == true)
	{

		if (P2C3Slot2Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C3Slot3Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C3Slot4Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C3Slot5Free == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}

	}





}

void SetGlobalPlayerSpeed(float GlobalSpeed)
{
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15B4), &GlobalSpeed, sizeof(GlobalSpeed), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15B4), &GlobalSpeed, sizeof(GlobalSpeed), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15B4), &GlobalSpeed, sizeof(GlobalSpeed), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15B4), &GlobalSpeed, sizeof(GlobalSpeed), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15B4), &GlobalSpeed, sizeof(GlobalSpeed), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15B4), &GlobalSpeed, sizeof(GlobalSpeed), NULL))
	{

	}

}

void GetActiveInstallData()
{

	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1578), &P1C1InstallID1, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F8), &P1C1InstallID2, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1678), &P1C1InstallID3, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x16F8), &P1C1InstallID4, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1778), &P1C1InstallID5, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x157C), &P1C1InstallType1, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15FC), &P1C1InstallType2, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x167C), &P1C1InstallType3, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x16FC), &P1C1InstallType4, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x177C), &P1C1InstallType5, sizeof(int), 0);

	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1578), &P1C2InstallID1, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15F8), &P1C2InstallID2, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1678), &P1C2InstallID3, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x16F8), &P1C2InstallID4, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1778), &P1C2InstallID5, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x157C), &P1C2InstallType1, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15FC), &P1C2InstallType2, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x167C), &P1C2InstallType3, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x16FC), &P1C2InstallType4, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x177C), &P1C2InstallType5, sizeof(int), 0);

	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1578), &P1C3InstallID1, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15F8), &P1C3InstallID2, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1678), &P1C3InstallID3, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x16F8), &P1C3InstallID4, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1778), &P1C3InstallID5, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x157C), &P1C3InstallType1, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15FC), &P1C3InstallType2, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x167C), &P1C3InstallType3, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x16FC), &P1C3InstallType4, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x177C), &P1C3InstallType5, sizeof(int), 0);

	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1578), &P2C1InstallID1, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15F8), &P2C1InstallID2, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1678), &P2C1InstallID3, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x16F8), &P2C1InstallID4, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1778), &P2C1InstallID5, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x157C), &P2C1InstallType1, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15FC), &P2C1InstallType2, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x167C), &P2C1InstallType3, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x16FC), &P2C1InstallType4, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x177C), &P2C1InstallType5, sizeof(int), 0);


	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1578), &P2C2InstallID1, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15F8), &P2C2InstallID2, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1678), &P2C2InstallID3, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x16F8), &P2C2InstallID4, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1778), &P2C2InstallID5, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x157C), &P2C2InstallType1, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15FC), &P2C2InstallType2, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x167C), &P2C2InstallType3, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x16FC), &P2C2InstallType4, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x177C), &P2C2InstallType5, sizeof(int), 0);

	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1578), &P2C3InstallID1, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15F8), &P2C3InstallID2, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1678), &P2C3InstallID3, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x16F8), &P2C3InstallID4, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1778), &P2C3InstallID5, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x157C), &P2C3InstallType1, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15FC), &P2C3InstallType2, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x167C), &P2C3InstallType3, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x16FC), &P2C3InstallType4, sizeof(int), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x177C), &P2C3InstallType5, sizeof(int), 0);

	if (P1C1InstallID2 == 0 && P1C1InstallType2 == 0)
	{
		P1C1Slot2Free = true;
	}
	else
	{
		P1C1Slot2Free = false;
	}

	if (P1C1InstallID3 == 0 && P1C1InstallType3 == 0)
	{
		P1C1Slot3Free = true;
	}
	else
	{
		P1C1Slot3Free = false;
	}

	if (P1C1InstallID4 == 0 && P1C1InstallType4 == 0)
	{
		P1C1Slot4Free = true;
	}
	else
	{
		P1C1Slot4Free = false;
	}

	if (P1C1InstallID5 == 0 && P1C1InstallType5 == 0)
	{
		P1C1Slot5Free = true;
	}
	else
	{
		P1C1Slot5Free = false;
	}

	if (P1C2InstallID2 == 0 && P1C2InstallType2 == 0)
	{
		P1C2Slot2Free = true;
	}
	else
	{
		P1C2Slot2Free = false;
	}

	if (P1C2InstallID3 == 0 && P1C2InstallType3 == 0)
	{
		P1C2Slot3Free = true;
	}
	else
	{
		P1C2Slot3Free = false;
	}

	if (P1C2InstallID4 == 0 && P1C2InstallType4 == 0)
	{
		P1C2Slot4Free = true;
	}
	else
	{
		P1C2Slot4Free = false;
	}

	if (P1C2InstallID5 == 0 && P1C2InstallType5 == 0)
	{
		P1C2Slot5Free = true;
	}
	else
	{
		P1C2Slot5Free = false;
	}

	if (P1C3InstallID2 == 0 && P1C3InstallType2 == 0)
	{
		P1C3Slot2Free = true;
	}
	else
	{
		P1C3Slot2Free = false;
	}

	if (P1C3InstallID3 == 0 && P1C3InstallType3 == 0)
	{
		P1C3Slot3Free = true;
	}
	else
	{
		P1C3Slot3Free = false;
	}

	if (P1C3InstallID4 == 0 && P1C3InstallType4 == 0)
	{
		P1C3Slot4Free = true;
	}
	else
	{
		P1C3Slot4Free = false;
	}

	if (P1C3InstallID5 == 0 && P1C3InstallType5 == 0)
	{
		P1C3Slot5Free = true;
	}
	else
	{
		P1C3Slot5Free = false;
	}

	if (P2C1InstallID2 == 0 && P2C1InstallType2 == 0)
	{
		P2C1Slot2Free = true;
	}
	else
	{
		P2C1Slot2Free = false;
	}

	if (P2C1InstallID3 == 0 && P2C1InstallType3 == 0)
	{
		P2C1Slot3Free = true;
	}
	else
	{
		P2C1Slot3Free = false;
	}

	if (P2C1InstallID4 == 0 && P2C1InstallType4 == 0)
	{
		P2C1Slot4Free = true;
	}
	else
	{
		P2C1Slot4Free = false;
	}

	if (P2C1InstallID5 == 0 && P2C1InstallType5 == 0)
	{
		P2C1Slot5Free = true;
	}
	else
	{
		P2C1Slot5Free = false;
	}

	if (P2C2InstallID2 == 0 && P2C2InstallType2 == 0)
	{
		P2C2Slot2Free = true;
	}
	else
	{
		P2C2Slot2Free = false;
	}

	if (P2C2InstallID3 == 0 && P2C2InstallType3 == 0)
	{
		P2C2Slot3Free = true;
	}
	else
	{
		P2C2Slot3Free = false;
	}

	if (P2C2InstallID4 == 0 && P2C2InstallType4 == 0)
	{
		P2C2Slot4Free = true;
	}
	else
	{
		P2C2Slot4Free = false;
	}

	if (P2C2InstallID5 == 0 && P2C2InstallType5 == 0)
	{
		P2C2Slot5Free = true;
	}
	else
	{
		P2C2Slot5Free = false;
	}

	if (P2C3InstallID2 == 0 && P2C3InstallType2 == 0)
	{
		P2C3Slot2Free = true;
	}
	else
	{
		P2C3Slot2Free = false;
	}

	if (P2C3InstallID3 == 0 && P2C3InstallType3 == 0)
	{
		P2C3Slot3Free = true;
	}
	else
	{
		P2C3Slot3Free = false;
	}

	if (P2C3InstallID4 == 0 && P2C3InstallType4 == 0)
	{
		P2C3Slot4Free = true;
	}
	else
	{
		P2C3Slot4Free = false;
	}

	if (P2C3InstallID5 == 0 && P2C3InstallType5 == 0)
	{
		P2C3Slot5Free = true;
	}
	else
	{
		P2C3Slot5Free = false;
	}




}

void GetDebugData()
{

	//Attempts to get Player 1 Character 1's hurtbox/hurtsphere coordinates.


}


