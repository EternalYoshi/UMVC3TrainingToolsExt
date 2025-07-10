#include "umvc3utils.h"
#include <iostream>
#include "proc.h"
#include "gui.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <chrono>
#include "..\utils\addr.h"
#include <thread>


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
	//Sleep(5000);

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
		//MessageBoxA(0, "Bad News.\nThis tool only supports latest (or at least the version from 2017) Steam executable\nof Ultimate Marvel vs Capcom 3.", 0, MB_ICONINFORMATION);
		//exit(1);
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
	ReadProcessMemory(hProcess, (LPVOID*)0x140d50e58, &sBattleSetting, sizeof(uintptr_t), 0);

	//Reads the match/menu flag.
	ReadProcessMemory(hProcess, (LPVOID*)(sBattleSetting + 0x34C), &GameMode, sizeof(int), 0);

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
		ReadProcessMemory(hProcess, (LPVOID*)0x140d533e0, &gSound, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)0x140d47e68, &sAction, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)0x140d50e58, &sBattleSetting, sizeof(uintptr_t), 0);
		team1ptr = sAction + 0x350;
		team2ptr = sAction + 0x610;

		//Gets better Pointers.
		ReadProcessMemory(hProcess, (LPVOID*)0x140D44A70, &sCharacter, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(sCharacter + 0x58), &Player1CharNodeTree, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(sCharacter + 0x328), &Player2CharNodeTree, sizeof(uintptr_t), 0);

		//Gets More Pointers.
		ReadProcessMemory(hProcess, (LPVOID*)(sAction + 0x350), &Player1TeamTable, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(sAction + 0x610), &Player2TeamTable, sizeof(uintptr_t), 0);

	}
}

//Gets the active characters based on the Table Pointers.
void GetCharacterIDs()
{
	if (GameMode == 5)
	{

		ReadProcessMemory(hProcess, (LPVOID*)(sBattleSetting + 0x44), &P1Character1ID, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(sBattleSetting + 0x44 + 0x58), &P1Character2ID, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(sBattleSetting + 0x44 + 0x58 + 0x58), &P1Character3ID, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(sBattleSetting + 0x44 + 0x58 + 0x58 + 0x58), &P2Character1ID, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(sBattleSetting + 0x44 + 0x58 + 0x58 + 0x58 + 0x58), &P2Character2ID, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(sBattleSetting + 0x44 + 0x58 + 0x58 + 0x58 + 0x58 + 0x58), &P2Character3ID, sizeof(int), 0);

	}
}

//Gets the Active Player pointers.
void GetPlayerData()
{
	if (GameMode == 5)
	{

		uintptr_t TempA = 0;
		uintptr_t TempB = 0;
		ReadProcessMemory(hProcess, (LPVOID*)(Player1CharNodeTree + 0x10), &TempA, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(Player2CharNodeTree + 0x10), &TempB, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(Player1CharNodeTree + 0x8), &P1Character1Data, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(TempA + 0x8), &P1Character2Data, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x18), &P1Character3Data, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(Player2CharNodeTree + 0x8), &P2Character1Data, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(TempB + 0x8), &P2Character2Data, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x18), &P2Character3Data, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data), scriptableFighters[0].fighter, sizeof(Fighter), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data), scriptableFighters[1].fighter, sizeof(Fighter), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data), scriptableFighters[2].fighter, sizeof(Fighter), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data), scriptableFighters[3].fighter, sizeof(Fighter), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data), scriptableFighters[4].fighter, sizeof(Fighter), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data), scriptableFighters[5].fighter, sizeof(Fighter), 0);

		const char* name = nullptr;

		ReadProcessMemory(hProcess, (LPVOID*)(Player1TeamTable + 0x48), &Player1ActiveCharacter, sizeof(int), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(Player2TeamTable + 0x48), &Player2ActiveCharacter, sizeof(int), 0);

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
	if (Turnabout == true)
	{
		Objection();
	}
	if (DarkPhoenix == true)
	{
		PopTheBird();
	}
	if (LockEvidence == true)
	{
		ChangeWrightEvidence();
	}
	ChangeMODOKUnderstanding(MODOKLOU);
	SetDormSpellLevels();
	SetGameSpeed(1);
	SetGlobalPlayerSpeed(P1Char1Speed);
	EndlessInstallBoolUpdate();
	SetDeadpoolTeleport();
	JammingToggleRestart();

}

//Forces the X Factor Timer to stay at a stupid high time to be unlimited.
void EndlessXFactorUpdate()
{
	GetPlayerData();
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

//Checks for valid vtable address at start of struct to hopefully stop crashing/running when input display is either off or not present.
bool InputDisplayValidator()
{

	long vftaddress = 0;
	ReadProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp), &vftaddress, sizeof(vftaddress), 0);

	if (vftaddress == 5380359088)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

void LeftSideInputDisplay()
{
	if (MoveInputDisplay == true)
	{
		//Gets Yet Another Pointer.
		uintptr_t TempA = 0;
		uintptr_t TempB = 0;
		uintptr_t TempC = 0;
		uintptr_t TempD = 0;
		uintptr_t TempE = 0;
		uintptr_t TempF = 0;
		uintptr_t TempG = 0;
		float TempFloat = 0.0;
		float TempFloatTwo = 0.7;
		ReadProcessMemory(hProcess, (LPVOID*)0x140E1BC98, &TempA, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(TempA + 0x70), &TempB, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(TempB + 0x20), &TempC, sizeof(uintptr_t), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(TempC + 0x70), &TempD, sizeof(uintptr_t), 0);
		//ReadProcessMemory(hProcess, (LPVOID*)(TempD), &TempE, sizeof(uintptr_t), 0);

		uBtKeyDisp = TempD;

		if (InputDisplayValidator())
		{
			if (TempA == 0 || TempB == 0 || TempC == 0 || uBtKeyDisp == 0)
			{
			}
			else
			{
				//InputDisplayDataPointer stuff.
				TempFloat = 15;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x70), &TempFloat, sizeof(float), NULL))
				{

				}

				TempFloat = 15;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x74), &TempFloat, sizeof(float), NULL))
				{

				}

				TempFloat = 0;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x78), &TempFloat, sizeof(float), NULL))
				{

				}

				if (!WriteProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x7C), &TempFloat, sizeof(float), NULL))
				{

				}

				TempFloat = 0.00146484;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x80), &TempFloat, sizeof(float), NULL))
				{

				}

				TempFloat = 0.7;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x84), &TempFloat, sizeof(float), NULL))
				{

				}

				TempFloat = 0;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x88), &TempFloat, sizeof(float), NULL))
				{

				}

				if (!WriteProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x8C), &TempFloat, sizeof(float), NULL))
				{

				}

				TempFloat = 0.7;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x90), &TempFloat, sizeof(float), NULL))
				{

				}

				TempFloat = 0.00146484;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x94), &TempFloat, sizeof(float), NULL))
				{

				}

				TempFloat = 0;
				if (!WriteProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x98), &TempFloat, sizeof(float), NULL))
				{

				}

				if (!WriteProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x9C), &TempFloat, sizeof(float), NULL))
				{

				}

				//Part Two. This part gets the uBtKeyDispBtn table for scaling purposes. I wonder if I can do this without
				//affecting perforamnce...

				ReadProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x50), &ButtonCount, sizeof(int), 0);
				ReadProcessMemory(hProcess, (LPVOID*)(uBtKeyDisp + 0x60), &uBtKeyDispBtnTable, sizeof(uBtKeyDispBtnTable), 0);
				TempF = uBtKeyDispBtnTable;
				for (int v = 0; v < ButtonCount; v++)
				{
					ReadProcessMemory(hProcess, (LPVOID*)(TempF), &TempG, sizeof(TempG), 0);
					if (!WriteProcessMemory(hProcess, (LPVOID*)(TempG + 0x60), &TempFloatTwo, sizeof(TempFloatTwo), NULL))
					{

					}

					if (!WriteProcessMemory(hProcess, (LPVOID*)(TempG + 0x64), &TempFloatTwo, sizeof(TempFloatTwo), NULL))
					{

					}

					TempF = TempF + 0x08;
				}

			}

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

	if (VergilSpiralSwordsForever == true)
	{
		VergilSpiralSwords();
	}

	if (LockMODOKLOU == true)
	{
		ChangeMODOKUnderstanding(MODOKLOU);
	}

	if (LockP1Meter == true)
	{
		float P1MeterToFloat = P1Meter;
		if (!WriteProcessMemory(hProcess, (LPVOID*)(team1ptr + 0x78), &P1MeterToFloat, sizeof(P1MeterToFloat), NULL))
		{

		}
	}

	if (LockP2Meter == true)
	{
		float P2MeterToFloat = P2Meter;
		if (!WriteProcessMemory(hProcess, (LPVOID*)(team2ptr + 0x78), &P2MeterToFloat, sizeof(P2MeterToFloat), NULL))
		{

		}
	}

	if (P1C1Jammed == true || P1C2Jammed == true || P1C3Jammed == true || P2C1Jammed == true || P2C2Jammed == true || P2C3Jammed == true)
	{
		JammingToggle();
	}

	if (LockEvidence == true)
	{
		ChangeWrightEvidence();
	}

	LeftSideInputDisplay();

	GetShots();

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
		if (AlsoSetRedHealthP1 == true)
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

		if (AlsoSetRedHealthP2 == true)
		{
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
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F0), &InvestiagtionMode, sizeof(FighterInstall), NULL))
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
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15F0), &InvestiagtionMode, sizeof(FighterInstall), NULL))
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
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15F0), &InvestiagtionMode, sizeof(FighterInstall), NULL))
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
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15F0), &InvestiagtionMode, sizeof(FighterInstall), NULL))
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
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15F0), &InvestiagtionMode, sizeof(FighterInstall), NULL))
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
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15F0), &InvestiagtionMode, sizeof(FighterInstall), NULL))
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

void VergilSpiralSwords()
{
	int8_t SSOn = 1;
	int8_t SSOff = 0;
	GetPlayerData();
	GetCharacterIDs();

	if (P1Character1ID == 22)
	{
		if (VergilSpiralSwordsForever == true)
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F0), &VergilSwords, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68f0), &SSOn, sizeof(SSOn), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68f1), &SSOn, sizeof(SSOn), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68f2), &SSOn, sizeof(SSOn), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68f3), &SSOn, sizeof(SSOn), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68f4), &SSOn, sizeof(SSOn), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68f5), &SSOn, sizeof(SSOn), NULL))
			{

			}
		}
		else
		{
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68f0), &SSOff, sizeof(SSOff), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68f1), &SSOff, sizeof(SSOff), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68f2), &SSOff, sizeof(SSOff), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68f3), &SSOff, sizeof(SSOff), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68f4), &SSOff, sizeof(SSOff), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x68f5), &SSOff, sizeof(SSOff), NULL))
			{

			}

		}
	}

	if (P1Character2ID == 22)
	{

	}

	if (P1Character3ID == 22)
	{

	}

	if (P2Character1ID == 22)
	{

	}

	if (P2Character2ID == 22)
	{

	}

	if (P2Character3ID == 22)
	{

	}

}

void TurnOnTheSwords() {}

void ErrorOccured(DWORD errorMessageID)
{
	//MessageBox(hWnd, errorMessageID, "An Error Occured", MB_OK | MB_ICONERROR);


}
/*
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


}
*/


//Sets whether or not the specified characters are hit by Jamming Bomb.
void JammingToggle()
{
	GetPlayerData();

	if (P1C1Jammed == true)
	{

		if (P1C1Slot2Free == true && !P1C1JammedAlready)
		{
			P1C1JammedSlot = 2;
			P1C1JammedAlready = true;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C1Slot3Free == true && !P1C1JammedAlready)
		{
			P1C1JammedSlot = 3;
			P1C1JammedAlready = true;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C1Slot4Free == true && !P1C1JammedAlready)
		{
			P1C1JammedAlready = true;
			P1C1JammedSlot = 4;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C1Slot5Free == true && !P1C1JammedAlready)
		{
			P1C1JammedAlready = true;
			P1C1JammedSlot = 5;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}

	}
	else
	{
		switch (P1C1JammedSlot)
		{
		case 5:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1770), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P1C1JammedSlot = 0;
			P1C1JammedAlready = false;
			break;
		case 4:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x16F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P1C1JammedSlot = 0;
			P1C1JammedAlready = false;
			break;
		case 3:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1670), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P1C1JammedSlot = 0;
			P1C1JammedAlready = false;
			break;
		case 2:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P1C1JammedSlot = 0;
			P1C1JammedAlready = false;
			break;
		default:
			P1C1JammedSlot = 0;
			P1C1JammedAlready = false;
			break;
		}
	}

	if (P1C2Jammed == true)
	{

		if (P1C2Slot2Free == true && !P1C2JammedAlready)
		{
			P1C2JammedSlot = 2;
			P1C2JammedAlready = true;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C2Slot3Free == true && !P1C2JammedAlready)
		{
			P1C2JammedSlot = 3;
			P1C2JammedAlready = true;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C2Slot4Free == true && !P1C2JammedAlready)
		{
			P1C2JammedAlready = true;
			P1C2JammedSlot = 4;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C2Slot5Free == true && !P1C2JammedAlready)
		{
			P1C2JammedAlready = true;
			P1C2JammedSlot = 5;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}

	}
	else
	{
		switch (P1C2JammedSlot)
		{
		case 5:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1770), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P1C2JammedSlot = 0;
			P1C2JammedAlready = false;
			break;
		case 4:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x16F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P1C2JammedSlot = 0;
			P1C2JammedAlready = false;
			break;
		case 3:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1670), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P1C2JammedSlot = 0;
			P1C2JammedAlready = false;
			break;
		case 2:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P1C2JammedSlot = 0;
			P1C2JammedAlready = false;
			break;
		default:
			P1C2JammedSlot = 0;
			P1C2JammedAlready = false;
			break;
		}
	}

	if (P1C3Jammed == true)
	{

		if (P1C3Slot2Free == true && !P1C3JammedAlready)
		{
			P1C3JammedSlot = 2;
			P1C3JammedAlready = true;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C3Slot3Free == true && !P1C3JammedAlready)
		{
			P1C3JammedSlot = 3;
			P1C3JammedAlready = true;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C3Slot4Free == true && !P1C3JammedAlready)
		{
			P1C3JammedAlready = true;
			P1C3JammedSlot = 4;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P1C3Slot5Free == true && !P1C3JammedAlready)
		{
			P1C3JammedAlready = true;
			P1C3JammedSlot = 5;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}

	}
	else
	{
		switch (P1C3JammedSlot)
		{
		case 5:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1770), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P1C3JammedSlot = 0;
			P1C3JammedAlready = false;
			break;
		case 4:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x16F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P1C3JammedSlot = 0;
			P1C3JammedAlready = false;
			break;
		case 3:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1670), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P1C3JammedSlot = 0;
			P1C3JammedAlready = false;
			break;
		case 2:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P1C3JammedSlot = 0;
			P1C3JammedAlready = false;
			break;
		default:
			P1C3JammedSlot = 0;
			P1C3JammedAlready = false;
			break;
		}
	}

	if (P2C1Jammed == true)
	{

		if (P2C1Slot2Free == true && !P2C1JammedAlready)
		{
			P2C1JammedSlot = 2;
			P2C1JammedAlready = true;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C1Slot3Free == true && !P2C1JammedAlready)
		{
			P2C1JammedSlot = 3;
			P2C1JammedAlready = true;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C1Slot4Free == true && !P2C1JammedAlready)
		{
			P2C1JammedAlready = true;
			P2C1JammedSlot = 4;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C1Slot5Free == true && !P2C1JammedAlready)
		{
			P2C1JammedAlready = true;
			P2C1JammedSlot = 5;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}

	}
	else
	{
		switch (P2C1JammedSlot)
		{
		case 5:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1770), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P2C1JammedSlot = 0;
			P2C1JammedAlready = false;
			break;
		case 4:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x16F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P2C1JammedSlot = 0;
			P2C1JammedAlready = false;
			break;
		case 3:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1670), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P2C1JammedSlot = 0;
			P2C1JammedAlready = false;
			break;
		case 2:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P2C1JammedSlot = 0;
			P2C1JammedAlready = false;
			break;
		default:
			P2C1JammedSlot = 0;
			P2C1JammedAlready = false;
			break;
		}
	}

	if (P2C2Jammed == true)
	{

		if (P2C2Slot2Free == true && !P2C2JammedAlready)
		{
			P2C2JammedSlot = 2;
			P2C2JammedAlready = true;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C2Slot3Free == true && !P2C2JammedAlready)
		{
			P2C2JammedSlot = 3;
			P2C2JammedAlready = true;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C2Slot4Free == true && !P2C2JammedAlready)
		{
			P2C2JammedAlready = true;
			P2C2JammedSlot = 4;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C2Slot5Free == true && !P2C2JammedAlready)
		{
			P2C2JammedAlready = true;
			P2C2JammedSlot = 5;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}

	}
	else
	{
		switch (P2C2JammedSlot)
		{
		case 5:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1770), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P2C2JammedSlot = 0;
			P2C2JammedAlready = false;
			break;
		case 4:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x16F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P2C2JammedSlot = 0;
			P2C2JammedAlready = false;
			break;
		case 3:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1670), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P2C2JammedSlot = 0;
			P2C2JammedAlready = false;
			break;
		case 2:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P2C2JammedSlot = 0;
			P2C2JammedAlready = false;
			break;
		default:
			P2C2JammedSlot = 0;
			P2C2JammedAlready = false;
			break;
		}
	}

	if (P2C3Jammed == true)
	{

		if (P2C3Slot2Free == true && !P2C3JammedAlready)
		{
			P2C3JammedSlot = 2;
			P2C3JammedAlready = true;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C3Slot3Free == true && !P2C3JammedAlready)
		{
			P2C3JammedSlot = 3;
			P2C3JammedAlready = true;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C3Slot4Free == true && !P2C3JammedAlready)
		{
			P2C3JammedAlready = true;
			P2C3JammedSlot = 4;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}
		else if (P2C3Slot5Free == true && !P2C3JammedAlready)
		{
			P2C3JammedAlready = true;
			P2C3JammedSlot = 5;
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			else
			{
				return;
			}
		}

	}
	else
	{
		switch (P2C3JammedSlot)
		{
		case 5:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1770), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P2C3JammedSlot = 0;
			P2C3JammedAlready = false;
			break;
		case 4:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x16F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P2C3JammedSlot = 0;
			P2C3JammedAlready = false;
			break;
		case 3:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1670), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P2C3JammedSlot = 0;
			P2C3JammedAlready = false;
			break;
		case 2:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
			{

			}
			P2C3JammedSlot = 0;
			P2C3JammedAlready = false;
			break;
		default:
			P2C3JammedSlot = 0;
			P2C3JammedAlready = false;
			break;
		}
	}

}

void JammingToggleRestart()
{

	if (P1C1Jammed == true)
	{
		switch (P1C1JammedSlot)
		{
		case 2:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 3:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 4:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 5:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		default:
			break;




		}
	}

	if (P1C2Jammed == true)
	{
		switch (P1C2JammedSlot)
		{
		case 2:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 3:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 4:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 5:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		default:
			break;




		}
	}

	if (P1C3Jammed == true)
	{
		switch (P1C3JammedSlot)
		{
		case 2:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 3:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 4:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 5:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		default:
			break;




		}
	}

	if (P2C1Jammed == true)
	{
		switch (P2C1JammedSlot)
		{
		case 2:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 3:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 4:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 5:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		default:
			break;




		}
	}

	if (P2C2Jammed == true)
	{
		switch (P2C2JammedSlot)
		{
		case 2:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 3:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 4:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 5:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		default:
			break;




		}
	}

	if (P2C3Jammed == true)
	{
		switch (P2C3JammedSlot)
		{
		case 2:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 3:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1670), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 4:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x16F0), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		case 5:
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1770), &Jammed, sizeof(FighterInstall), NULL))
			{

			}
			break;
		default:
			break;




		}
	}

}

void SetGlobalPlayerSpeed(float GlobalSpeed)
{

	GetPlayerData();

	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15B4), &P1Char1Speed, sizeof(P1Char1Speed), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15B4), &P1Char1Speed, sizeof(P1Char1Speed), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15B4), &P1Char1Speed, sizeof(P1Char1Speed), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15B4), &P2Char1Speed, sizeof(P2Char1Speed), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15B4), &P2Char1Speed, sizeof(P2Char1Speed), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15B4), &P2Char1Speed, sizeof(P2Char1Speed), NULL))
	{

	}

}

void SetGameSpeed(float GameSpeed)
{

	uintptr_t PointerToGameSpeedA = 0;
	VirtualProtect((LPVOID*)0x140E177e8, 8, PAGE_EXECUTE_READWRITE, &oldPageProtection);

	if (!ReadProcessMemory(hProcess, (LPVOID*)(0x140E177e8), &PointerToGameSpeedA, sizeof(uintptr_t), NULL))
	{

	}


	if (!WriteProcessMemory(hProcess, (LPVOID*)(PointerToGameSpeedA + 0x8c), &GameSpeed, sizeof(GameSpeed), NULL))
	{

	}


	VirtualProtect((LPVOID*)0x140B12D10, 8, oldPageProtection, &oldPageProtection);

	/*
	VirtualProtect((LPVOID*)0x140B12D10, 5, PAGE_EXECUTE_READWRITE, &oldPageProtection);

	ReadProcessMemory(hProcess, (LPVOID*)0x140B12D10, &gameName, sizeof(gameName), 0);

	VirtualProtect((LPVOID*)0x140B12D10, 5, oldPageProtection, &oldPageProtection);
	*/
}

void ResetGameSpeed()
{
	GameSpeed = 1.0;
	uintptr_t PointerToGameSpeedA = 0;
	VirtualProtect((LPVOID*)0x140E177e8, 8, PAGE_EXECUTE_READWRITE, &oldPageProtection);

	if (!ReadProcessMemory(hProcess, (LPVOID*)(0x140E177e8), &PointerToGameSpeedA, sizeof(uintptr_t), NULL))
	{

	}



	if (!WriteProcessMemory(hProcess, (LPVOID*)(PointerToGameSpeedA + 0x8c), &GameSpeed, sizeof(GameSpeed), NULL))
	{

	}


	VirtualProtect((LPVOID*)0x140B12D10, 8, oldPageProtection, &oldPageProtection);

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

void SetMeters()
{
	float P1MeterToFloat = P1Meter;
	float P2MeterToFloat = P2Meter;


	if (!WriteProcessMemory(hProcess, (LPVOID*)(team1ptr + 0x78), &P1MeterToFloat, sizeof(P1MeterToFloat), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(team2ptr + 0x78), &P2MeterToFloat, sizeof(P2MeterToFloat), NULL))
	{

	}

}

void GetShots()
{

	ReadProcessMemory(hProcess, (LPVOID*)0x140D47F98, &sShotList, sizeof(uintptr_t), 0);
	//ReadProcessMemory(hProcess, (LPVOID*)0x140D47FA8, &sShotListEnd, sizeof(uintptr_t), 0);
	//ReadProcessMemory(hProcess, (LPVOID*)0x140D47FA0, &sTargetNode, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)0x140D47FA0, &P1Shots, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)0x140D47FC8, &P2Shots, sizeof(uintptr_t), 0);
	//ReadProcessMemory(hProcess, (LPVOID*)0x140D47F90, &sShotList, sizeof(uintptr_t), 0);
	//ReadProcessMemory(hProcess, (LPVOID*)(sShotList + 0x18), &sShotListEnd, sizeof(uintptr_t), 0);
	//ReadProcessMemory(hProcess, (LPVOID*)(sShotList + 0x10), &sTargetNode, sizeof(uintptr_t), 0);


	//uintptr_t d = 0;
	//int c = 0;
	//while (sTargetNode != sShotListEnd)
	//{

	//	//d = sTargetNode;
	//	ReadProcessMemory(hProcess, (LPVOID*)(sTargetNode +0x08), &sTargetNode, sizeof(uintptr_t), 0);
	//	c++;
	//}

	//ShotCount = c;




}

void GetDebugData()
{

	//Attempts to get Player 1 Character 1's hurtbox/hurtsphere coordinates.


}

static std::string HashToTypeName(uintptr_t Hash)
{

	switch (Hash)
	{

	//Ryu Shots.
	case 0x140ad4eb0:
		return "uShotLaserReflectSinkuHadouken";

	case 0x140ad5330:
		return "uShotLaserSinkuHadouken";

	case 0x140ad5780:
		return "uShotVulcanHadouken";

	//Chun-Li Shots.

	case 0x140a9ff60:
		return "uShotVulcanKikouken";

	case 0x140a9faf0:
		return "uShotShieldKikousyou";

	//Gouki A.K.A. Akuma Shots.
	case 0x140ab4640:
		return "uShotVulcanGouHadouken";

	case 0x140ab41b0:
		return "uShotLaserMessatsuGouHadou";

	case 0x140ab4a90:
		return "uShotVulcanTenmaGouZanku";

	//Chris Shots.
	case 0x140a9b050:
		return "uShotLaserSatelliteLaser";

	case 0x140a9b4c0:
		return "uShotMissileGrenadeLauncher";

	case 0x140a9b920:
		return "uShotShieldFlameGun";

	case 0x140a9bd70:
		return "uShotVulcanFireGrenadeBomb";

	case 0x140a9c210:
		return "uShotVulcanFireGrenadeExplode";

	case 0x140a9c670:
		return "uShotVulcanFireGrenadeGroundFlame";

	case 0x140a9cad0:
		return "uShotVulcanGrenadeLauncherExplode";

	case 0x140a9cf30:
		return "uShotVulcanHandGrenadeBomb";

	case 0x140a9d380:
		return "uShotVulcanHandGrenadeExplode";

	case 0x140a9d7e0:
		return "uShotVulcanHandGun";

	case 0x140a9dc30:
		return "uShotVulcanLandMine";

	case 0x140a9e0a0:
		return "uShotVulcanLandMineExplode";

	case 0x140a9e4f0:
		return "uShotVulcanMachineGun";

	case 0x140a9e940:
		return "uShotVulcanMagnumGun";

	case 0x140a9ed90:
		return "uShotVulcanRocketLauncher";

	case 0x140a9f210:
		return "uShotVulcanSatelliteLaserLock";

	case 0x140a9f6a0:
		return "uShotVulcanShotGun";

	//Wesker's Samurai Edge.
	case 0x140ae5b40:
		return "uShotVulcanWeskerHandGun";

	//Viewtiful Joe's Shots.
	case 0x140ae4990:
		return "uShotVulcanBoomerang";

	case 0x140ae4e10:
		return "uShotVulcanShockingPinkBomb";

	case 0x140ae5290:
		return "uShotVulcanShockingPinkExplode";

	case 0x140ae56f0:
		return "uShotVulcanSixCannon";

	//Dante's Shots.		
	case 0x140aa10d0:
		return "uShotLaserCrystal";

	case 0x140aa1540:
		return "uShotLaserMillionCarat";

	case 0x140aa19b0:
		return "uShotLaserThunderBolt";

	case 0x140aa1e30:
		return "uShotLaserTwister";

	case 0x140aa2280:
		return "uShotMissileHysteric";

	case 0x140aa26f0:
		return "uShotOptionGrapple";

	case 0x140aa2b60:
		return "uShotShieldJamSession";

	case 0x140aa2fd0:
		return "uShotShieldTempest";

	case 0x140aa3420:
		return "uShotVulcanAcidRain";

	case 0x140aa3890:
		return "uShotVulcanAirPlay";

	case 0x140aa3cf0:
		return "uShotVulcanBulletDanceFinish";

	case 0x140aa4150:
		return "uShotVulcanDrive";

	case 0x140aa45a0:
		return "uShotVulcanEbonyAndIvory";

	case 0x140aa49f0:
		return "uShotVulcanFireWorks";

	case 0x140aa4e40:
		return "uShotVulcanMultiLock";

	case 0x140aa5290:
		return "uShotVulcanDualShot";

	//Trish.

	case 0x140adfaa0:
		return "uShotMissileTrishBullet";

	case 0x140adff10:
		return "uShotOptionSpadaBoomerang";

	case 0x140ae03a0:
		return "uShotVulcanBarrier";

	case 0x140ae07f0:
		return "uShotVulcanBarrierSpread";

	case 0x140ae0c40:
		return "uShotVulcanEnergyBolt";

	//Frank.

	case 0x140ab06e0:
		return "uShotLaserLikeAMegaman";//Unused.

	case 0x140ab0b50:
		return "uShotShieldWeaponCartSpread";

	case 0x140ab0ff0:
		return "uShotShieldWildPitchHit";

	case 0x140ab1460:
		return "uShotVulcanThrowWeapon";

	case 0x140ab18b0:
		return "uShotVulcanThrowWeaponB";

	case 0x140ab1d20:
		return "uShotVulcanWeaponCart";

	case 0x140ab2330:
		return "uShotVulcanWeaponCartSpread";

	case 0x140ab2780:
		return "uShotVulcanWildPitch";

	//Spencer.
	case 0x140ad91f0:
		return "uShotOptionAttackWire";

	case 0x140ad96a0:
		return "uShotOptionSwingWire";

	case 0x140ad9b00:
		return "uShotVulcanFurisosoguShi";//Bionic Bomber...?

	case 0x140ad9f50:
		return "uShotVulcanSwingWire";

	//Arthur.

	case 0x140a97a90:
		return "uShotMissileArrow";

	case 0x140a97f20:
		return "uShotOptionSickleBoomerang";

	case 0x140a983c0:
		return "uShotShieldKnightShield";

	case 0x140a98840:
		return "uShotVulcanAxe";

	case 0x140a98ca0:
		return "uShotVulcanBraceletOfGoddess";

	case 0x140a99100:
		return "uShotVulcanCrossSword";

	case 0x140a995a0:
		return "uShotVulcanGreatSorcery";

	case 0x140a99a20:
		return "uShotVulcanJavelin";

	case 0x140a99e90:
		return "uShotVulcanKnife";

	case 0x140a9a2f0:
		return "uShotVulcanTorch";

	case 0x140a9a750:
		return "uShotVulcanTorchFirePillar";

	//Amaterasu.

	case 0x140a94a10:
		return "uShotVulcanBakuen";

	case 0x140a94e80:
		return "uShotVulcanExtentThunder";

	case 0x140a952e0:
		return "uShotVulcanHyoran";

	case 0x140a95760:
		return "uShotVulcanMagatama";

	case 0x140a95be0:
		return "uShotVulcanOneSpark";

	case 0x140a96070:
		return "uShotVulcanOneSparkZan";

	case 0x140a964c0:
		return "uShotVulcanRaikou";

	case 0x140a96910:
		return "uShotVulcanRaikouSub";

	case 0x140a96d60:
		return "uShotVulcanTekadama";

	case 0x140a971c0:
		return "uShotVulcanTekadamaExplode";

	case 0x140a97620:
		return "uShotVulcanTsutamaki";

	//Zero.

	case 0x140ae5fb0:
		return "uShotLaserRaikousen";

	case 0x140ae6420:
		return "uShotLaserRekkouha";

	case 0x140ae6870:
		return "uShotVulcanGenmuZero";

	case 0x140ae6cc0:
		return "uShotVulcanHadangeki";

	case 0x140ae7110:
		return "uShotVulcanZeroBuster";


	//Tron Bonne.

	case 0x140ae1090:
		return "uShotVulcanChushokuRash";

	case 0x140ae1530:
		return "uShotVulcanKingKobun";

	case 0x140ae19a0:
		return "uShotVulcanKobunLauncher";

	case 0x140ae1df0:
		return "uShotVulcanRockThrow";
 
	case 0x140ae2260:
		return "uShotVulcanShikiDan";

	case 0x140ae26b0:
		return "uShotVulcanShikiDanKobun";

	//Morrigan.

	case 0x140ac5860:
		return "uShotOptionSoulFistAbsorb";

	case 0x140ac5d10:
		return "uShotVulcanFinishingShower";

	case 0x140ac61b0:
		return "uShotVulcanShilhouetteBlade";

	case 0x140ac6600:
		return "uShotVulcanSoulFist";

	//Hsien-Ko/Lei-Lei.

	case 0x140abf2e0:
		return "uShotVulcanAnkihouBomb";

	case 0x140abf790:
		return "uShotVulcanAnkihouBombExplode";

	case 0x140abfbf0:
		return "uShotVulcanAnkihouKobun";

	case 0x140ac00c0:
		return "uShotVulcanAnkihouRolling";

	case 0x140ac0570:
		return "uShotVulcanHenkyouKi";

	case 0x140ac09c0:
		return "uShotVulcanTenraiha";

	case 0x140ac0e20:
		return "uShotVulcanZireitou";


	//Felicia.

	case 0x140aafdc0:
		return "uShotVulcanPleaseHelpMe";

	case 0x140ab0270:
		return "uShotVulcanSandSplash";

	//Crimson Viper.

	case 0x140aa03b0:
		return "uShotLaserOpticBlast";

	case 0x140aa0830:
		return "uShotVulcanBurningKick";

	case 0x140aa0c80:
		return "uShotVulcanSeismoHammer";

	//Haggar.

	case 0x140ab4ee0:
		return "uShotVulcanBodyPressFinish";

	case 0x140ab5350:
		return "uShotVulcanIronPipeRolling";

	//Jill.
	case 0x140abee70:
		return "uShotVulcanJillMachineGun";


	//Strider Hiryu.

	case 0x140ab9110:
		return "uShotShieldFormationD";

	case 0x140ab95c0:
		return "uShotShieldOuroboros";

	case 0x140ab9a70:
		return "uShotShieldOuroborosExit";

	case 0x140ab9ec0:
		return "uShotVulcanFormationATiger";

	case 0x140aba350:
		return "uShotVulcanFormationBHawk";

	case 0x140aba7c0:
		return "uShotVulcanFormationCBom";

	case 0x140abac50:
		return "uShotVulcanFormationCExplode";

	case 0x140abb0c0:
		return "uShotVulcanFormationCSpread";

	case 0x140abb550:
		return "uShotVulcanFormationDSpread";

	case 0x140abb9c0:
		return "uShotVulcanLegionHawk";

	case 0x140abbe10:
		return "uShotVulcanLegionTiger";

	case 0x140abc260:
		return "uShotVulcanOuroborosShot";

	//Vergil Shots.
	case 0x140ae33f0:
		return "uShotShieldSpiralSword";

	case 0x140ae3840:
		return "uShotShieldStormSword";

	case 0x140ae2f90:
		return "uShotShieldRaidSword";

	case 0x140ae4540:
		return "uShotVulcanStormSwordSpread";

	case 0x140ae2b00:
		return "uShotOptionRoundTrip";

	case 0x140ae3ca0:
		return "uShotVulcanJigenZan";// AKA Judgment Cut.

	case 0x140ae40f0:
		return "uShotVulcanRapidSlash";


	//Mr. Wright.


	//Red Arremer AKA Firebrand.


	//Nemesis.


	//Spider-Man.


	//Captain America's Shield Slash.
	case 0x140a9aba0:
		return "uShotOptionShieldSlash";

	//Wolverine does not have any shot files.
	// 
	//Magneto Shots.
	case 0x140ac1b30:
		return "uShotMissileGravitySqueeze";

	case 0x140ac1f90:
		return "uShotVulcanHyperGravitation";

	case 0x140ac23f0:
		return "uShotVulcanMagneticBlast";

	case 0x140ac2840:
		return "uShotVulcanMagneticTempst";

	case 0x140ac2ca0:
		return "uShotVulcanMagnetoAtkSitM";


		
	//Taskmaster Shots.
	case 0x140add7c0:
		return "uShotVulcanArrowRain";

	case 0x140addc30:
		return "uShotVulcanArrowShot";

	case 0x140ade080:
		return "uShotVulcanBullet";








	//case "":
	//	return "";


	default:
		return "?????";
	}

}

void GetHurtboxData()
{
	//The Hurtboxes aren't really stored in the same way as the character data stuff I'm used to working with normally.
	//Have to use this funky looking method to get the characters' hurtboxes during runtime.
	uintptr_t t;
	uintptr_t tt;
	uintptr_t ttt;
	uintptr_t tttt;
	uintptr_t v;
	uintptr_t w = 0;
	uintptr_t ww;
	uintptr_t g = 0;
	uintptr_t gg;
	uintptr_t pp = 0;
	uintptr_t ppp;
	uintptr_t qq;
	P1C1HurtboxCount = 0;
	P1C2HurtboxCount = 0;
	P1C3HurtboxCount = 0;
	P2C1HurtboxCount = 0;
	P2C2HurtboxCount = 0;
	P2C3HurtboxCount = 0;
	P1ProjectileCount = 0;
	P2ProjectileCount = 0;
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x4E10), &t, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(t + 0x30), &tt, sizeof(uintptr_t), 0);
	tttt = tt;

#pragma region Player 1 Character 1

	//This is for getting the hurtbox count.
	for (size_t i = 0; i < 100; i++)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(tt), &ttt, sizeof(uintptr_t), 0);;
		if (ttt == 0) {
			break;
		}
		//tt++;;
		tt = tt + 8;
		P1C1HurtboxCount++;
	}

	P1C1Hurtboxes.clear();
	P1C1Hurtboxes.resize(P1C1HurtboxCount);

	uintptr_t* HurtboxPointers = new uintptr_t[P1C1HurtboxCount];
	tt = tttt;

	//Once More, but this time for the Hurtbox pointers.
	for (int j = 0; j < P1C1HurtboxCount; j++)
	{

		ReadProcessMemory(hProcess, (LPVOID*)(tt), &v, sizeof(uintptr_t), 0);;
		HurtboxPointers[j] = v;
		//Stuff to get the relevant data.
		//P1C1Hurtboxes[j]->CollData;
		ReadProcessMemory(hProcess, (LPVOID*)(v), &P1C1Hurtboxes[j].PointerIdentifier, sizeof(long), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x10), &P1C1Hurtboxes[j].PointerToMoreData, sizeof(long), 0);

		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x20), &P1C1Hurtboxes[j].SecondaryX, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x24), &P1C1Hurtboxes[j].SecondaryY, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x28), &P1C1Hurtboxes[j].SecondaryZ, sizeof(float), 0);

		ReadProcessMemory(hProcess, (LPVOID*)((P1C1Hurtboxes[j].PointerToMoreData + 0x20)),
			&P1C1Hurtboxes[j].CollData.Coordinates.X, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P1C1Hurtboxes[j].PointerToMoreData + 0x24)),
			&P1C1Hurtboxes[j].CollData.Coordinates.Y, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P1C1Hurtboxes[j].PointerToMoreData + 0x28)),
			&P1C1Hurtboxes[j].CollData.Coordinates.Z, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P1C1Hurtboxes[j].PointerToMoreData + 0x2C)),
			&P1C1Hurtboxes[j].CollData.Radius, sizeof(float), 0);





		tt = tt + 8;

	}

#pragma endregion

#pragma region Player 1 Character 2

	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x4E10), &t, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(t + 0x30), &tt, sizeof(uintptr_t), 0);
	tttt = tt;

	//This is for getting the hurtbox count.
	for (size_t i = 0; i < 100; i++)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(tt), &ttt, sizeof(uintptr_t), 0);;
		if (ttt == 0) {
			break;
		}
		//tt++;;
		tt = tt + 8;
		P1C2HurtboxCount++;
	}

	P1C2Hurtboxes.clear();
	P1C2Hurtboxes.resize(P1C2HurtboxCount);

	uintptr_t* HurtboxPointers2 = new uintptr_t[P1C2HurtboxCount];
	tt = tttt;

	//Once More, but this time for the Hurtbox pointers.
	for (int j = 0; j < P1C2HurtboxCount; j++)
	{

		ReadProcessMemory(hProcess, (LPVOID*)(tt), &v, sizeof(uintptr_t), 0);;
		HurtboxPointers2[j] = v;
		//Stuff to get the relevant data.
		//P1C2Hurtboxes[j]->CollData;
		ReadProcessMemory(hProcess, (LPVOID*)(v), &P1C2Hurtboxes[j].PointerIdentifier, sizeof(long), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x10), &P1C2Hurtboxes[j].PointerToMoreData, sizeof(long), 0);

		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x20), &P1C2Hurtboxes[j].SecondaryX, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x24), &P1C2Hurtboxes[j].SecondaryY, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x28), &P1C2Hurtboxes[j].SecondaryZ, sizeof(float), 0);

		ReadProcessMemory(hProcess, (LPVOID*)((P1C2Hurtboxes[j].PointerToMoreData + 0x20)),
			&P1C2Hurtboxes[j].CollData.Coordinates.X, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P1C2Hurtboxes[j].PointerToMoreData + 0x24)),
			&P1C2Hurtboxes[j].CollData.Coordinates.Y, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P1C2Hurtboxes[j].PointerToMoreData + 0x28)),
			&P1C2Hurtboxes[j].CollData.Coordinates.Z, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P1C2Hurtboxes[j].PointerToMoreData + 0x2C)),
			&P1C2Hurtboxes[j].CollData.Radius, sizeof(float), 0);





		tt = tt + 8;

	}

#pragma endregion

#pragma region Player 1 Character 3

	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x4E10), &t, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(t + 0x30), &tt, sizeof(uintptr_t), 0);
	tttt = tt;

	//This is for getting the hurtbox count.
	for (size_t i = 0; i < 100; i++)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(tt), &ttt, sizeof(uintptr_t), 0);;
		if (ttt == 0) {
			break;
		}
		//tt++;;
		tt = tt + 8;
		P1C3HurtboxCount++;
	}

	P1C3Hurtboxes.clear();
	P1C3Hurtboxes.resize(P1C3HurtboxCount);

	uintptr_t* HurtboxPointers3 = new uintptr_t[P1C3HurtboxCount];
	tt = tttt;

	//Once More, but this time for the Hurtbox pointers.
	for (int j = 0; j < P1C3HurtboxCount; j++)
	{

		ReadProcessMemory(hProcess, (LPVOID*)(tt), &v, sizeof(uintptr_t), 0);;
		HurtboxPointers3[j] = v;
		//Stuff to get the relevant data.
		//P1C3Hurtboxes[j]->CollData;
		ReadProcessMemory(hProcess, (LPVOID*)(v), &P1C3Hurtboxes[j].PointerIdentifier, sizeof(long), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x10), &P1C3Hurtboxes[j].PointerToMoreData, sizeof(long), 0);

		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x20), &P1C3Hurtboxes[j].SecondaryX, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x24), &P1C3Hurtboxes[j].SecondaryY, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x28), &P1C3Hurtboxes[j].SecondaryZ, sizeof(float), 0);

		ReadProcessMemory(hProcess, (LPVOID*)((P1C3Hurtboxes[j].PointerToMoreData + 0x20)),
			&P1C3Hurtboxes[j].CollData.Coordinates.X, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P1C3Hurtboxes[j].PointerToMoreData + 0x24)),
			&P1C3Hurtboxes[j].CollData.Coordinates.Y, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P1C3Hurtboxes[j].PointerToMoreData + 0x28)),
			&P1C3Hurtboxes[j].CollData.Coordinates.Z, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P1C3Hurtboxes[j].PointerToMoreData + 0x2C)),
			&P1C3Hurtboxes[j].CollData.Radius, sizeof(float), 0);





		tt = tt + 8;

	}

#pragma endregion

#pragma region Player 2 Character 1

	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x4E10), &t, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(t + 0x30), &tt, sizeof(uintptr_t), 0);
	tttt = tt;

	//This is for getting the hurtbox count.
	for (size_t i = 0; i < 100; i++)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(tt), &ttt, sizeof(uintptr_t), 0);;
		if (ttt == 0) {
			break;
		}
		//tt++;;
		tt = tt + 8;
		P2C1HurtboxCount++;
	}

	P2C1Hurtboxes.clear();
	P2C1Hurtboxes.resize(P2C1HurtboxCount);

	uintptr_t* HurtboxPointers4 = new uintptr_t[P2C1HurtboxCount];
	tt = tttt;

	//Once More, but this time for the Hurtbox pointers.
	for (int j = 0; j < P2C1HurtboxCount; j++)
	{

		ReadProcessMemory(hProcess, (LPVOID*)(tt), &v, sizeof(uintptr_t), 0);;
		HurtboxPointers4[j] = v;
		//Stuff to get the relevant data.
		//P2C1Hurtboxes[j]->CollData;
		ReadProcessMemory(hProcess, (LPVOID*)(v), &P2C1Hurtboxes[j].PointerIdentifier, sizeof(long), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x10), &P2C1Hurtboxes[j].PointerToMoreData, sizeof(long), 0);

		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x20), &P2C1Hurtboxes[j].SecondaryX, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x24), &P2C1Hurtboxes[j].SecondaryY, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x28), &P2C1Hurtboxes[j].SecondaryZ, sizeof(float), 0);

		ReadProcessMemory(hProcess, (LPVOID*)((P2C1Hurtboxes[j].PointerToMoreData + 0x20)),
			&P2C1Hurtboxes[j].CollData.Coordinates.X, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P2C1Hurtboxes[j].PointerToMoreData + 0x24)),
			&P2C1Hurtboxes[j].CollData.Coordinates.Y, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P2C1Hurtboxes[j].PointerToMoreData + 0x28)),
			&P2C1Hurtboxes[j].CollData.Coordinates.Z, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P2C1Hurtboxes[j].PointerToMoreData + 0x2C)),
			&P2C1Hurtboxes[j].CollData.Radius, sizeof(float), 0);





		tt = tt + 8;

	}

#pragma endregion

#pragma region Player 2 Character 2

	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x4E10), &t, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(t + 0x30), &tt, sizeof(uintptr_t), 0);
	tttt = tt;

	//This is for getting the hurtbox count.
	for (size_t i = 0; i < 100; i++)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(tt), &ttt, sizeof(uintptr_t), 0);;
		if (ttt == 0) {
			break;
		}
		//tt++;;
		tt = tt + 8;
		P2C2HurtboxCount++;
	}

	P2C2Hurtboxes.clear();
	P2C2Hurtboxes.resize(P2C2HurtboxCount);

	uintptr_t* HurtboxPointers5 = new uintptr_t[P2C2HurtboxCount];
	tt = tttt;

	//Once More, but this time for the Hurtbox pointers.
	for (int j = 0; j < P2C2HurtboxCount; j++)
	{

		ReadProcessMemory(hProcess, (LPVOID*)(tt), &v, sizeof(uintptr_t), 0);;
		HurtboxPointers5[j] = v;
		//Stuff to get the relevant data.
		//P2C2Hurtboxes[j]->CollData;
		ReadProcessMemory(hProcess, (LPVOID*)(v), &P2C2Hurtboxes[j].PointerIdentifier, sizeof(long), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x10), &P2C2Hurtboxes[j].PointerToMoreData, sizeof(long), 0);

		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x20), &P2C2Hurtboxes[j].SecondaryX, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x24), &P2C2Hurtboxes[j].SecondaryY, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x28), &P2C2Hurtboxes[j].SecondaryZ, sizeof(float), 0);

		ReadProcessMemory(hProcess, (LPVOID*)((P2C2Hurtboxes[j].PointerToMoreData + 0x20)),
			&P2C2Hurtboxes[j].CollData.Coordinates.X, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P2C2Hurtboxes[j].PointerToMoreData + 0x24)),
			&P2C2Hurtboxes[j].CollData.Coordinates.Y, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P2C2Hurtboxes[j].PointerToMoreData + 0x28)),
			&P2C2Hurtboxes[j].CollData.Coordinates.Z, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P2C2Hurtboxes[j].PointerToMoreData + 0x2C)),
			&P2C2Hurtboxes[j].CollData.Radius, sizeof(float), 0);





		tt = tt + 8;

	}

#pragma endregion

#pragma region Player 2 Character 3

	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x4E10), &t, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(t + 0x30), &tt, sizeof(uintptr_t), 0);
	tttt = tt;

	//This is for getting the hurtbox count.
	for (size_t i = 0; i < 100; i++)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(tt), &ttt, sizeof(uintptr_t), 0);;
		if (ttt == 0) {
			break;
		}
		//tt++;;
		tt = tt + 8;
		P2C3HurtboxCount++;
	}

	P2C3Hurtboxes.clear();
	P2C3Hurtboxes.resize(P2C3HurtboxCount);

	uintptr_t* HurtboxPointers6 = new uintptr_t[P2C3HurtboxCount];
	tt = tttt;

	//Once More, but this time for the Hurtbox pointers.
	for (int j = 0; j < P2C3HurtboxCount; j++)
	{

		ReadProcessMemory(hProcess, (LPVOID*)(tt), &v, sizeof(uintptr_t), 0);;
		HurtboxPointers6[j] = v;
		//Stuff to get the relevant data.
		//P2C3Hurtboxes[j]->CollData;
		ReadProcessMemory(hProcess, (LPVOID*)(v), &P2C3Hurtboxes[j].PointerIdentifier, sizeof(long), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x10), &P2C3Hurtboxes[j].PointerToMoreData, sizeof(long), 0);

		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x20), &P2C3Hurtboxes[j].SecondaryX, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x24), &P2C3Hurtboxes[j].SecondaryY, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(v + 0x28), &P2C3Hurtboxes[j].SecondaryZ, sizeof(float), 0);

		ReadProcessMemory(hProcess, (LPVOID*)((P2C3Hurtboxes[j].PointerToMoreData + 0x20)),
			&P2C3Hurtboxes[j].CollData.Coordinates.X, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P2C3Hurtboxes[j].PointerToMoreData + 0x24)),
			&P2C3Hurtboxes[j].CollData.Coordinates.Y, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P2C3Hurtboxes[j].PointerToMoreData + 0x28)),
			&P2C3Hurtboxes[j].CollData.Coordinates.Z, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)((P2C3Hurtboxes[j].PointerToMoreData + 0x2C)),
			&P2C3Hurtboxes[j].CollData.Radius, sizeof(float), 0);





		tt = tt + 8;

	}

#pragma endregion

	//For Shot files.
	//Player 1 Shots. Let's try to iterate through this properly.

	w = 0;
	g = 0;


	ww = 0x140D47F98;
	while (w != -1 && w != 0x140D47F98)
	{

		ReadProcessMemory(hProcess, (LPVOID*)((ww + 0x8)), &w, sizeof(uintptr_t), 0);
		if (w != -1)
		{
			ww = w;
		}
		if (w != -1 && w != 0x140D47F98)
		{
			P1ProjectileCount++;
		}

	}

	//Player 2 Shots. Let's try to iterate through this properly.
	gg = 0x140D47F98;
	while (g != -1 && g != 0x140d47fc0)
	{

		ReadProcessMemory(hProcess, (LPVOID*)((gg + 0x30)), &g, sizeof(uintptr_t), 0);
		if (g != -1)
		{
			gg = g;
		}
		if (g != -1 && g != 0x140d47fc0)
		{
			P2ProjectileCount++;
		}

	}

	pp = 0;

	ActiveShots.clear();
	//ActiveShots.resize((P1ProjectileCount + P2ProjectileCount));
	ActiveShots.resize((static_cast<std::vector<ProjectileData, std::allocator<ProjectileData>>::size_type>(P1ProjectileCount) + P2ProjectileCount));


	ppp = 0x140D47F98;
	for (int j = 0; j < P1ProjectileCount; j++)
	{
		if (pp != -1 && pp != 0x140D47F98)
		{
			ReadProcessMemory(hProcess, (LPVOID*)((ppp + 0x8)), &pp, sizeof(uintptr_t), 0);
			if (pp != -1)
			{
				ppp = pp;
			}
			qq = (pp - 0x1450);
			ReadProcessMemory(hProcess, (LPVOID*)(qq), &ActiveShots[j].ShotVTAddress, sizeof(uintptr_t), 0);
			ActiveShots[j].ProjName = HashToTypeName(ActiveShots[j].ShotVTAddress);

			ReadProcessMemory(hProcess, (LPVOID*)((qq + 0x50)), &ActiveShots[j].PossibleAbsPos.X, sizeof(float), 0);
			ReadProcessMemory(hProcess, (LPVOID*)((qq + 0x54)), &ActiveShots[j].PossibleAbsPos.Y, sizeof(float), 0);
			ReadProcessMemory(hProcess, (LPVOID*)((qq + 0x58)), &ActiveShots[j].PossibleAbsPos.Z, sizeof(float), 0);

			ReadProcessMemory(hProcess, (LPVOID*)((qq + 0x60)), &ActiveShots[j].PossibleQuat.X, sizeof(float), 0);
			ReadProcessMemory(hProcess, (LPVOID*)((qq + 0x64)), &ActiveShots[j].PossibleQuat.Y, sizeof(float), 0);
			ReadProcessMemory(hProcess, (LPVOID*)((qq + 0x68)), &ActiveShots[j].PossibleQuat.Z, sizeof(float), 0);
			ReadProcessMemory(hProcess, (LPVOID*)((qq + 0x6C)), &ActiveShots[j].PossibleQuat.W, sizeof(float), 0);

			ReadProcessMemory(hProcess, (LPVOID*)((qq + 0x70)), &ActiveShots[j].PossibleScale.X, sizeof(float), 0);
			ReadProcessMemory(hProcess, (LPVOID*)((qq + 0x74)), &ActiveShots[j].PossibleScale.Y, sizeof(float), 0);
			ReadProcessMemory(hProcess, (LPVOID*)((qq + 0x78)), &ActiveShots[j].PossibleScale.Z, sizeof(float), 0);

		}
	}

	/*
	//Player 1 Shots. Let's try to iterate through this properly.
	ww = 0x140D47F98;
	while (w != -1 && w != 0x140D47F98)
	{

		ReadProcessMemory(hProcess, (LPVOID*)((ww + 0x8)), &w, sizeof(uintptr_t), 0);
		if (w != -1)
		{
			ww = w;
		}
		if (w != -1 && w != 0x140D47F98)
		{
			P1ProjectileCount++;
		}

	}
	*/


	for (int j = 0; j < P2ProjectileCount; j++)
	{

	}


}

void GetHitboxDataPart1()
{

	uintptr_t u;
	uintptr_t uu;
	uintptr_t uuu;
	uintptr_t uuuu;
	uintptr_t w;
	uintptr_t pp = 0;
	uintptr_t ppp;
	P1C1HitboxCount = 0;
	P1C2HitboxCount = 0;
	P1C3HitboxCount = 0;
	P2C1HitboxCount = 0;
	P2C2HitboxCount = 0;
	P2C3HitboxCount = 0;

#pragma region P1C1
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x4200), &u, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(u + 0x30), &uu, sizeof(uintptr_t), 0);
	uuuu = uu;

	//Active Hitbox Count.
	ReadProcessMemory(hProcess, (LPVOID*)(u + 0x20), &P1C1HitboxCount, sizeof(int), 0);

	P1C1Hitboxes.clear();
	P1C1Hitboxes.resize(P1C1HitboxCount);

	uintptr_t* HurtboxPointers = new uintptr_t[P1C1HitboxCount];
	uu = uuuu;


	//Once More, but this time for the Hitbox pointers.
	for (int j = 0; j < P1C1HitboxCount; j++)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(uu), &w, sizeof(uintptr_t), 0);;

		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x20), &P1C1Hitboxes[j].GlobalX, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x24), &P1C1Hitboxes[j].GlobalY, sizeof(float), 0);
		//ReadProcessMemory(hProcess, (LPVOID*)(w + 0x28), &P1C1Hitboxes[j].GlobalZ, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x38), &P1C1Hitboxes[j].Radius, sizeof(float), 0);


		uu = uu + 8;

	}
#pragma endregion

#pragma region P1C2
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x4200), &u, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(u + 0x30), &uu, sizeof(uintptr_t), 0);
	uuuu = uu;

	//Active Hitbox Count.
	ReadProcessMemory(hProcess, (LPVOID*)(u + 0x20), &P1C2HitboxCount, sizeof(int), 0);

	P1C2Hitboxes.clear();
	P1C2Hitboxes.resize(P1C2HitboxCount);

	uu = uuuu;


	//Once More, but this time for the Hitbox pointers.
	for (int j = 0; j < P1C2HitboxCount; j++)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(uu), &w, sizeof(uintptr_t), 0);;

		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x20), &P1C2Hitboxes[j].GlobalX, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x24), &P1C2Hitboxes[j].GlobalY, sizeof(float), 0);
		//ReadProcessMemory(hProcess, (LPVOID*)(w + 0x28), &P1C2Hitboxes[j].GlobalZ, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x38), &P1C2Hitboxes[j].Radius, sizeof(float), 0);


		uu = uu + 8;

	}
#pragma endregion

#pragma region P1C3
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x4200), &u, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(u + 0x30), &uu, sizeof(uintptr_t), 0);
	uuuu = uu;

	//Active Hitbox Count.
	ReadProcessMemory(hProcess, (LPVOID*)(u + 0x20), &P1C3HitboxCount, sizeof(int), 0);

	P1C3Hitboxes.clear();
	P1C3Hitboxes.resize(P1C3HitboxCount);

	uu = uuuu;


	//Once More, but this time for the Hitbox pointers.
	for (int j = 0; j < P1C3HitboxCount; j++)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(uu), &w, sizeof(uintptr_t), 0);;

		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x20), &P1C3Hitboxes[j].GlobalX, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x24), &P1C3Hitboxes[j].GlobalY, sizeof(float), 0);
		//ReadProcessMemory(hProcess, (LPVOID*)(w + 0x28), &P1C3Hitboxes[j].GlobalZ, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x38), &P1C3Hitboxes[j].Radius, sizeof(float), 0);


		uu = uu + 8;

	}
#pragma endregion

#pragma region P2C1
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x4200), &u, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(u + 0x30), &uu, sizeof(uintptr_t), 0);
	uuuu = uu;

	//Active Hitbox Count.
	ReadProcessMemory(hProcess, (LPVOID*)(u + 0x20), &P2C1HitboxCount, sizeof(int), 0);

	P2C1Hitboxes.clear();
	P2C1Hitboxes.resize(P2C1HitboxCount);

	uu = uuuu;


	//Once More, but this time for the Hitbox pointers.
	for (int j = 0; j < P2C1HitboxCount; j++)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(uu), &w, sizeof(uintptr_t), 0);;

		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x20), &P2C1Hitboxes[j].GlobalX, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x24), &P2C1Hitboxes[j].GlobalY, sizeof(float), 0);
		//ReadProcessMemory(hProcess, (LPVOID*)(w + 0x28), &P2C1Hitboxes[j].GlobalZ, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x38), &P2C1Hitboxes[j].Radius, sizeof(float), 0);


		uu = uu + 8;

	}
#pragma endregion

#pragma region P2C2
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x4200), &u, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(u + 0x30), &uu, sizeof(uintptr_t), 0);
	uuuu = uu;

	//Active Hitbox Count.
	ReadProcessMemory(hProcess, (LPVOID*)(u + 0x20), &P2C2HitboxCount, sizeof(int), 0);

	P2C2Hitboxes.clear();
	P2C2Hitboxes.resize(P2C2HitboxCount);

	uu = uuuu;


	//Once More, but this time for the Hitbox pointers.
	for (int j = 0; j < P2C2HitboxCount; j++)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(uu), &w, sizeof(uintptr_t), 0);;

		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x20), &P2C2Hitboxes[j].GlobalX, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x24), &P2C2Hitboxes[j].GlobalY, sizeof(float), 0);
		//ReadProcessMemory(hProcess, (LPVOID*)(w + 0x28), &P2C2Hitboxes[j].GlobalZ, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x38), &P2C2Hitboxes[j].Radius, sizeof(float), 0);


		uu = uu + 8;

	}
#pragma endregion

#pragma region P2C3
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x4200), &u, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(u + 0x30), &uu, sizeof(uintptr_t), 0);
	uuuu = uu;

	//Active Hitbox Count.
	ReadProcessMemory(hProcess, (LPVOID*)(u + 0x20), &P2C3HitboxCount, sizeof(int), 0);

	P2C3Hitboxes.clear();
	P2C3Hitboxes.resize(P2C3HitboxCount);

	uu = uuuu;


	//Once More, but this time for the Hitbox pointers.
	for (int j = 0; j < P2C3HitboxCount; j++)
	{
		ReadProcessMemory(hProcess, (LPVOID*)(uu), &w, sizeof(uintptr_t), 0);;

		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x20), &P2C3Hitboxes[j].GlobalX, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x24), &P2C3Hitboxes[j].GlobalY, sizeof(float), 0);
		//ReadProcessMemory(hProcess, (LPVOID*)(w + 0x28), &P2C3Hitboxes[j].GlobalZ, sizeof(float), 0);
		ReadProcessMemory(hProcess, (LPVOID*)(w + 0x38), &P2C3Hitboxes[j].Radius, sizeof(float), 0);


		uu = uu + 8;

	}
#pragma endregion

	////For Shot files.
	////Player 1 Shots. Let's try to iterate through this properly.

	//w = 0;
	//uintptr_t ww;
	//uintptr_t g = 0;
	//uintptr_t gg;

	//ww = 0x140D47F98;
	//while (w != -1 && w != 0x140D47F98)
	//{

	//	ReadProcessMemory(hProcess, (LPVOID*)((ww + 0x8)), &w, sizeof(uintptr_t), 0);
	//	if (w != -1)
	//	{
	//		ww = w;
	//	}
	//	if (w != -1 && w != 0x140D47F98)
	//	{
	//		P1ProjectileCount++;
	//	}

	//}

	////Player 2 Shots. Let's try to iterate through this properly.
	//gg = 0x140D47F98;
	//while (g != -1 && g != 0x140D47F98)
	//{

	//	ReadProcessMemory(hProcess, (LPVOID*)((gg + 0x30)), &g, sizeof(uintptr_t), 0);
	//	if (g != -1)
	//	{
	//		gg = g;
	//	}
	//	if (g != -1 && g != 0x140D47F98)
	//	{
	//		P2ProjectileCount++;
	//	}

	//}
	//
	//pp = 0;
	//
	//ActiveShots.clear();
	////ActiveShots.resize((P1ProjectileCount + P2ProjectileCount));
	//ActiveShots.resize((static_cast<std::vector<ProjectileData, std::allocator<ProjectileData>>::size_type>(P1ProjectileCount) + P2ProjectileCount));

}

void RemoveAllInstalls()
{

	//Player 1 Character 1.
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1670), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x16F0), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1770), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}

	//Player 1 Character 2.
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1670), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x16F0), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1770), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}

	//Player 1 Character 3.
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1670), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x16F0), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1770), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}

	//Player 2 Character 1.
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1670), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x16F0), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1770), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}

	//Player 2 Character 2.
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1670), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x16F0), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1770), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}

	//Player 2 Character 3.
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x15F0), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1670), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x16F0), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}
	if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1770), &EmptyInstall, sizeof(FighterInstall), NULL))
	{

	}




}

void TheKillingFloor()
{
	GetPlayerData();
	int FlagA = 89;
	int FlagB = 0;
	int FlagC = 1050672;
	int FlagD = 85;
	int FlagE = 87;
	int FlagF = 131104;
	int FlagG = 73728;
	int FlagH = 525313;
	int FlagI = 131104;
	int FlagJ = 73404422;
	//char FlagK = 131104;

	if (DeathSiteY > 0)
	{
		if (KOActiveCharacterAtStart)
		{

			//X Coordinate.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x50), &DeathSiteX, sizeof(float), NULL))
			{

			}
			//Y Coordinate.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x54), &DeathSiteY, sizeof(float), NULL))
			{

			}

			//Air/Ground State.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x14F4), &FlagF, sizeof(int), NULL))
			{

			}

			//Something Part 1.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1504), &FlagG, sizeof(int), NULL))
			{

			}

			//Something Part 2.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1508), &FlagH, sizeof(int), NULL))
			{

			}

			Sleep(15);
			//Sets Health and Red Health to 0.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1550), &FlagB, sizeof(int), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1558), &FlagB, sizeof(int), NULL))
			{

			}

			Sleep(15);

			//Action/Anm thing.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1310), &FlagD, sizeof(int), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1314), &FlagE, sizeof(int), NULL))
			{

			}

			//TagState Flag Thing.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x14F0), &FlagC, sizeof(int), NULL))
			{

			}

			//I dunno what this is.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x14F8), &FlagJ, sizeof(int), NULL))
			{

			}

		}
		if (KOActiveOpponentAtStart)
		{
			//X Coordinate.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x50), &DeathSiteX, sizeof(float), NULL))
			{

			}
			//Y Coordinate.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x54), &DeathSiteY, sizeof(float), NULL))
			{

			}

			//Air/Ground State.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x14F4), &FlagF, sizeof(int), NULL))
			{

			}

			//Something Part 1.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1504), &FlagG, sizeof(int), NULL))
			{

			}

			//Something Part 2.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1508), &FlagH, sizeof(int), NULL))
			{

			}

			Sleep(16.67);
			//Sets Health and Red Health to 0.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1550), &FlagB, sizeof(int), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1558), &FlagB, sizeof(int), NULL))
			{

			}

			Sleep(16.67);

			//Action/Anm thing.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1310), &FlagD, sizeof(int), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1314), &FlagE, sizeof(int), NULL))
			{

			}

			//TagState Flag Thing.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x14F0), &FlagC, sizeof(int), NULL))
			{

			}

			//I dunno what this is.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x14F8), &FlagJ, sizeof(int), NULL))
			{

			}

		}
	}
	else
	{
		if (KOActiveCharacterAtStart)
		{
			//Sets Health and Red Health to 0.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1550), &FlagB, sizeof(int), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1558), &FlagB, sizeof(int), NULL))
			{

			}

			Sleep(16.67);

			//X Coordinate.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x50), &DeathSiteX, sizeof(float), NULL))
			{

			}

			//Action/Anm thing.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1310), &FlagA, sizeof(int), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1314), &FlagA, sizeof(int), NULL))
			{

			}



			//TagState Flag Thing.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x14F0), &FlagC, sizeof(int), NULL))
			{

			}

		}
		if (KOActiveOpponentAtStart)
		{
			//Sets Health and Red Health to 0.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1550), &FlagB, sizeof(int), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1558), &FlagB, sizeof(int), NULL))
			{

			}

			Sleep(16.67);

			//X Coordinate.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x50), &DeathSiteX, sizeof(float), NULL))
			{

			}

			//Action/Anm thing.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1310), &FlagA, sizeof(int), NULL))
			{

			}
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1314), &FlagA, sizeof(int), NULL))
			{

			}



			//TagState Flag Thing.
			if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x14F0), &FlagC, sizeof(int), NULL))
			{

			}

		}
	}

}

void DeathDelay()
{



	if (FrameDelayofDeath > 0)
	{
		Sleep(FrameDelayofDeath * 16.67);
		TheKillingFloor();
	}
	else
	{
		TheKillingFloor();
	}



}

void KOToggles()
{
	int FlagB = 0;
	int FlagC = 1050672;
	GetPlayerData();
	//Sleep(160);
	if (Player1Character1Dead == true)
	{
		//Sets Health and Red Health to 0.
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1550), &FlagB, sizeof(int), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x1558), &FlagB, sizeof(int), NULL))
		{

		}

		//TagState Flag Thing.
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x14F0), &FlagC, sizeof(int), NULL))
		{

		}

	}
	else {}
	if (Player1Character2Dead == true)
	{
		//Sets Health and Red Health to 0.
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1550), &FlagB, sizeof(int), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x1558), &FlagB, sizeof(int), NULL))
		{

		}

		//TagState Flag Thing.
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x14F0), &FlagC, sizeof(int), NULL))
		{

		}

	}
	else {}
	if (Player1Character3Dead == true)
	{
		//Sets Health and Red Health to 0.
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1550), &FlagB, sizeof(int), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x1558), &FlagB, sizeof(int), NULL))
		{

		}

		//TagState Flag Thing.
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x14F0), &FlagC, sizeof(int), NULL))
		{

		}

	}
	else {}

	if (Player2Character1Dead == true)
	{
		//Sets Health and Red Health to 0.

		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1550), &FlagB, sizeof(int), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x1558), &FlagB, sizeof(int), NULL))
		{

		}

		//TagState Flag Thing.
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x14F0), &FlagC, sizeof(int), NULL))
		{

		}
	}
	else {}
	if (Player2Character2Dead == true)
	{
		//Sets Health and Red Health to 0.
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1550), &FlagB, sizeof(int), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x1558), &FlagB, sizeof(int), NULL))
		{

		}

		//TagState Flag Thing.
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x14F0), &FlagC, sizeof(int), NULL))
		{

		}
	}
	else {}
	if (Player2Character3Dead == true)
	{
		//Sets Health and Red Health to 0.
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1550), &FlagB, sizeof(int), NULL))
		{

		}
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x1558), &FlagB, sizeof(int), NULL))
		{

		}

		//TagState Flag Thing.
		if (!WriteProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x14F0), &FlagC, sizeof(int), NULL))
		{

		}
	}

	else {}

}

void GetHitstunTimers()
{
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x63EC), &P1C1GroundHitstunTimer, sizeof(float), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character1Data + 0x63F0), &P1C1AirHitstunTimer, sizeof(float), 0);

	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x63EC), &P1C2GroundHitstunTimer, sizeof(float), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character2Data + 0x63F0), &P1C2AirHitstunTimer, sizeof(float), 0);

	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x63EC), &P1C3GroundHitstunTimer, sizeof(float), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P1Character3Data + 0x63F0), &P1C3AirHitstunTimer, sizeof(float), 0);

	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x63EC), &P2C1GroundHitstunTimer, sizeof(float), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character1Data + 0x63F0), &P2C1AirHitstunTimer, sizeof(float), 0);

	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x63EC), &P2C2GroundHitstunTimer, sizeof(float), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character2Data + 0x63F0), &P2C2AirHitstunTimer, sizeof(float), 0);

	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x63EC), &P2C3GroundHitstunTimer, sizeof(float), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(P2Character3Data + 0x63F0), &P2C3AirHitstunTimer, sizeof(float), 0);
	//inline float P1C1GroundHitstunTimer;
	//inline float P1C1AirHitstunTimer;
	//inline float P1C2GroundHitstunTimer;
	//inline float P1C2AirHitstunTimer;
	//inline float P1C3GroundHitstunTimer;
	//inline float P1C3AirHitstunTimer;
	//inline float P2C1GroundHitstunTimer;
	//inline float P2C1AirHitstunTimer;
	//inline float P2C2GroundHitstunTimer;
	//inline float P2C2AirHitstunTimer;
	//inline float P2C3GroundHitstunTimer;
	//inline float P2C3AirHitstunTimer;
}

void WackyStuff()
{
	uintptr_t LayerA = 0;
	uintptr_t LayerB = 0;
	ReadProcessMemory(hProcess, (LPVOID*)0x140E17698, &LayerA, sizeof(uintptr_t), 0);

	ReadProcessMemory(hProcess, (LPVOID*)(LayerA + 0x88), &LayerB, sizeof(uintptr_t), 0);
	ReadProcessMemory(hProcess, (LPVOID*)(LayerB), &DebugPointerOfSorts, sizeof(uintptr_t), 0);

	if (DebugPointerOfSorts == 0x140b1f450)
	{
		float NoTime = 0;
		if (!WriteProcessMemory(hProcess, (LPVOID*)(LayerB + 0x40), &NoTime, sizeof(float), NULL))
		{

		}
	}

}

//Sets all settings back to default.
void ResetSettings()
{
	GetMainPointers();
	GetPlayerData();
	GetCharacterIDs();
	p1Pos = -125.0f;
	p2Pos = 125.0f;
	MODOKLOU = 1;
	FrankLevel = 1;
	PrestigePoints = 0.0;
	WrightEvidenceA = -1;
	WrightEvidenceB = -1;
	WrightEvidenceC = -1;
	SaveStateSlot = 0;
	DormRed = 0;
	DormBlue = 0;
	DormSpellSet = false;
	EndlessInstalls = false;
	DeadpoolTeleportCount = 0;
	FreezeDeadpoolTPCounter = false;
	P1Character1Dead = false;
	P1Character2Dead = false;
	P1Character3Dead = false;
	P2Character1Dead = false;
	P2Character2Dead = false;
	P2Character3Dead = false;
	EndlessXFactor = false;
	EndlessXFactorP1 = false;
	EndlessXFactorP2 = false;
	SpecialFeature = false;
	HitboxDisplay = false;
	SetIndividualHP = false;
	AlsoSetRedHealth = false;
	P1Char1Health = 1.0;
	P1Char1RedHealth = 1.0;
	P1Char2Health = 1.0;
	P1Char2RedHealth = 1.0;
	P1Char3Health = 1.0;
	P1Char3RedHealth = 1.0;
	P2Char1Health = 1.0;
	P2Char1RedHealth = 1.0;
	P2Char2Health = 1.0;
	P2Char2RedHealth = 1.0;
	P2Char3Health = 1.0;
	P2Char3RedHealth = 1.0;
	P1Char1HealthNew = 1.0;
	P1Char2HealthNew = 1.0;
	P1Char3HealthNew = 1.0;
	P2Char1HealthNew = 1.0;
	P2Char2HealthNew = 1.0;
	P2Char3HealthNew = 1.0;
	P1Char1Slow = false;
	P1Char2Slow = false;
	P1Char3Slow = false;
	P2Char1Slow = false;
	P2Char2Slow = false;
	P2Char3Slow = false;
	DarkPhoenix = false;
	Turnabout = false;
	LockMODOKLOU = false;
	P1C1MaxHP = 0;
	P1C2MaxHP = 0;
	P1C3MaxHP = 0;
	P2C1MaxHP = 0;
	P2C2MaxHP = 0;
	P2C3MaxHP = 0;
	CharacterSpeed = 1.0;
	ModifyGameSpeed = false;
	GameSpeed = 1.0;
	QuickRestartType = 3;
	selected_item = "Nothing";
	selected_itemTwo = "Nothing";
	selected_itemThree = "Nothing";
	P1C1Slot2Free = false;
	P1C1Slot3Free = false;
	P1C1Slot4Free = false;
	P1C1Slot5Free = false;
	P1C2Slot2Free = false;
	P1C2Slot3Free = false;
	P1C2Slot4Free = false;
	P1C2Slot5Free = false;
	P1C3Slot2Free = false;
	P1C3Slot3Free = false;
	P1C3Slot4Free = false;
	P1C3Slot5Free = false;
	P2C1Slot2Free = false;
	P2C1Slot3Free = false;
	P2C1Slot4Free = false;
	P2C1Slot5Free = false;
	P2C2Slot2Free = false;
	P2C2Slot3Free = false;
	P2C2Slot4Free = false;
	P2C2Slot5Free = false;
	P2C3Slot2Free = false;
	P2C3Slot3Free = false;
	P2C3Slot4Free = false;
	P2C3Slot5Free = false;
	P1C1Jammed = false;
	P1C2Jammed = false;
	P1C3Jammed = false;
	P2C1Jammed = false;
	P2C2Jammed = false;
	P2C3Jammed = false;
	P1C1JammedAlready = false;
	P1C2JammedAlready = false;
	P1C3JammedAlready = false;
	P2C1JammedAlready = false;
	P2C2JammedAlready = false;
	P2C3JammedAlready = false;
	P1Meter = 10000;
	P2Meter = 10000;
	P1C1JammedSlot = 0;
	P1C2JammedSlot = 0;
	P1C3JammedSlot = 0;
	P2C1JammedSlot = 0;
	P2C2JammedSlot = 0;
	P2C3JammedSlot = 0;
	KOActiveCharacterAtStart = false;
	KOActiveOpponentAtStart = false;
	FrameDelayofDeath = 0;
	DeathSiteX = 0.0;
	DeathSiteY = 0.0;
	Player1Character1Dead = false;
	Player1Character2Dead = false;
	Player1Character3Dead = false;
	Player2Character1Dead = false;
	Player2Character2Dead = false;
	Player2Character3Dead = false;
	P1Char1Speed = 1.0;
	P1Char2Speed = 1.0;
	P1Char3Speed = 1.0;
	P2Char1Speed = 1.0;
	P2Char2Speed = 1.0;
	P2Char3Speed = 1.0;
	MoveInputDisplay = false;
	LockEvidence = false;
	LockP1Meter = false;
	LockP2Meter = false;
	AlsoSetRedHealthP1 = false;
	AlsoSetRedHealthP2 = false;
	VergilSpiralSwordsForever = false;
	ResetGameSpeed();
	SetGlobalPlayerSpeed(1);
	RemoveAllInstalls();
}

//uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
//{
//	uintptr_t modBaseAddr = 0;
//	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
//	if (hSnap != INVALID_HANDLE_VALUE)
//	{
//		MODULEENTRY32 modEntry;
//		modEntry.dwSize = sizeof(modEntry);
//		if (Module32First(hSnap, &modEntry))
//		{
//			do
//			{
//				if (!_wcsicmp(modEntry.szModule, modName))
//				{
//					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
//					break;
//				}
//			} while (Module32Next(hSnap, &modEntry));
//		}
//	}
//	CloseHandle(hSnap);
//	return modBaseAddr;
//}