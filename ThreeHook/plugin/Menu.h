#pragma once
#include "../umvc3/Vector.h"
#include "../umvc3/Matrix.h"
#include "../helper/eKeyboardMan.h"
#include "../utils.h"
#include "../gui/gui_impl_dx9.h"
#include <DirectXMath.h>
#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"

using namespace DirectX;

#define UMVC3HOOK_VERSION "0.2"

/*
int runaheadcount = 0;
bool pAddrSet = false;

uintptr_t mysterytable;
uintptr_t block2;
uintptr_t ptable;
int P1Character1ID;
int P1Character2ID;
int P1Character3ID;
int P2Character1ID;
int P2Character2ID;
int P2Character3ID;
uintptr_t P1Character1Data;
uintptr_t P1Character2Data;
uintptr_t P1Character3Data;
uintptr_t P2Character1Data;
uintptr_t P2Character2Data;
uintptr_t P2Character3Data;
int MODOKLOU = 1;
int FrankLevel = 1;
float PrestigePoints = 0.0;
int WrightEvidenceA = -1;
int WrightEvidenceB = -1;
int WrightEvidenceC = -1;
int SaveStateSlot = 0;
int DormRed = 0;
int DormBlue = 0;
bool DormSpellSet = false;
bool EndlessInstalls = false;
int DeadpoolTeleportCount = 0;
bool FreezeDeadpoolTPCounter = false;
bool P1Character1Dead = false;
bool P1Character2Dead = false;
bool P1Character3Dead = false;
bool P2Character1Dead = false;
bool P2Character2Dead = false;
bool P2Character3Dead = false;
bool EndlessXFactor = false;
bool SpecialFeature = false;
bool HitboxDisplay = false;
float P1Char1Health = 0.0;
float P1Char1RedHealth = 0.0;
float P1Char2Health = 0.0;
float P1Char2RedHealth = 0.0;
float P1Char3Health = 0.0;
float P1Char3RedHealth = 0.0;
float P2Char1Health = 0.0;
float P2Char1RedHealth = 0.0;
float P2Char2Health = 0.0;
float P2Char2RedHealth = 0.0;
float P2Char3Health = 0.0;
float P2Char3RedHealth = 0.0;
int GameMode;

const char* EvidenceTypes[] = { "Nothing","Photo","Watch", "Vase","Folder","Phone","Knife","Sunglasses","Beer","Bonsai", "Doll","Figurine","Plunger","Chicken" };
const char* EvidenceTypesTwo[] = { "Nothing","Photo","Watch", "Vase","Folder","Phone","Knife","Sunglasses","Beer","Bonsai", "Doll","Figurine","Plunger","Chicken" };
const char* EvidenceTypesThree[] = { "Nothing","Photo","Watch", "Vase","Folder","Phone","Knife","Sunglasses","Beer","Bonsai", "Doll","Figurine","Plunger","Chicken" };
static const char* selected_item = "Nothing";
static const char* selected_itemTwo = "Nothing";
static const char* selected_itemThree = "Nothing";

struct MarvelVector {
	float x;
	float y;
	float xVelocity;
};
enum EGroundedState :uint8_t {
	NoGround = 0,
	Grounded = 0xF4,
	InAir = 0xF2,
};

enum ETagState :uint8_t {
	NoTag = 0,
	Active = 0x0D,
	Inactive = 0x09,
	Active2 = 0x68,
	TagginOut = 0xB4,
	TagginIn = 0x6c
};

struct Fighter { // TODO map this out
	char offset[0x14];
	ETagState tagState;
	char offset2[0x3B];
	MarvelVector vector;
	char offset3[0x10];
	EGroundedState  groundedState;
	char offset4[0xc7];
	uint16_t specialState;//specialstate?
	char offset5[0x528];
	uint32_t anim; // anim??
};

struct ScriptableFighter {
	Fighter* fighter = 0;
	intptr_t fighterController = 0; // TODO find better name for this
	char* name = nullptr;
	uintptr_t tickPtr = 0x14004bd30;
	void Tick() {
		((void(__fastcall*)(longlong*))_addr(tickPtr))((longlong*)fighter);
	}
	const char* GetGroundedState() {
		switch (fighter->groundedState)
		{
		case Grounded:
			return "Grounded";
		case InAir:
			return "InAir";
		default:
			return "Unkown";
			break;
		}
	}
	const char* GetTagState() {
		switch (fighter->tagState)
		{
		case Active:
		case Active2:
			return "Active";
		case Inactive:
			return "Inactive";
		case TagginOut:
			return "TagginOut";
		case TagginIn:
			return "TagginIn";
		default:
			return "Unkown";
			break;
		}
	}
};

struct Recording {

	int FrameCount;
	char* InputBinary[3844];

};

enum WrightEvidence
{
	Nothing = -1,
	Photo = 0,
	Watch = 1,
	Vase = 2,
	Folder = 3,
	Phone = 4,
	Knife = 5,
	Sunglasses = 6,
	Beer = 7,
	Bonsai = 8,
	Doll = 9,
	Figurine = 10,
	Plunger = 11,
	Chicken = 12,
};

ScriptableFighter scriptableFighters[6] = { 0 };

*/

/*
//Adjusts Frank's Level.
void ChangeFrankLevel(int FrankLevel)
{
	//Gets the needed values in memory.
	uintptr_t mysterytable = *(uintptr_t*)_addr(0x140d533e0);
	uintptr_t block2 = *(uintptr_t*)_addr(0x140d47e68);
	uintptr_t ptable = *(uintptr_t*)_addr(0x140d50e58);
	int P1Character1ID = *(uintptr_t*)_addr((ptable + 0x44));
	int P1Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58));
	int P1Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58));
	int P2Character1ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58));
	int P2Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58));
	int P2Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58 + 0x58));

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

		uintptr_t P1Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0);
		*((int*)(P1Character1Data + 0x69C4)) = FrankLevel;
		eLog::Message(__FUNCTION__, "P1Character1 Frank's Level: " + *((int*)(P1Character1Data + 0x69C4)));
		//eLog::Message(__FUNCTION__, "P1Character1 Frank's PrestigePoints: " + *((float*)(P1Character1Data + 0x69DC)));
		//std::cout << "P1Character1 Frank's Level: " << std::hex << *((int*)(P1Character1Data + 0x69C4)) << std::endl;
		//std::cout << "P1Character1 Frank's PrestigePoints: " << std::hex << *((float*)(P1Character1Data + 0x69DC)) << std::endl;
		*((float*)(P1Character1Data + 0x69DC)) = PrestigePoints;

	}

	if (P1Character2ID == 9)
	{
		uintptr_t P1Character2Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438));
		*((int*)(P1Character2Data + 0x69C4)) = FrankLevel;
		*((float*)(P1Character2Data + 0x69DC)) = PrestigePoints;
	}

	if (P1Character3ID == 9)
	{
		uintptr_t P1Character3Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 2));
		*((int*)(P1Character3Data + 0x69C4)) = FrankLevel;
		*((float*)(P1Character3Data + 0x69DC)) = PrestigePoints;
	}

	if (P2Character1ID == 9)
	{
		uintptr_t P2Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 3));
		*((int*)(P2Character1Data + 0x69C4)) = FrankLevel;
		*((float*)(P2Character1Data + 0x69DC)) = PrestigePoints;
	}

	if (P2Character2ID == 9)
	{
		uintptr_t P2Character2Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 4));
		*((int*)(P2Character2Data + 0x69C4)) = FrankLevel;
		*((float*)(P2Character2Data + 0x69DC)) = PrestigePoints;
	}

	if (P2Character3ID == 9)
	{
		uintptr_t P2Character3Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 5));
		*((int*)(P2Character3Data + 0x69C4)) = FrankLevel;
		*((float*)(P2Character3Data + 0x69DC)) = PrestigePoints;
	}
}

//Sets Evidence based on selection.
void ChangeWrightEvidence()
{
	//Gets the needed values in memory.
	uintptr_t mysterytable = *(uintptr_t*)_addr(0x140d533e0);
	uintptr_t block2 = *(uintptr_t*)_addr(0x140d47e68);
	uintptr_t ptable = *(uintptr_t*)_addr(0x140d50e58);
	int P1Character1ID = *(uintptr_t*)_addr((ptable + 0x44));
	int P1Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58));
	int P1Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58));
	int P2Character1ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58));
	int P2Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58));
	int P2Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58 + 0x58));

	//Checks Character ID.
	if (P1Character1ID == 23)
	{
		uintptr_t P1Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0);
		*((int*)(P1Character1Data + 0x6918)) = WrightEvidenceA;
		*((int*)(P1Character1Data + 0x691C)) = WrightEvidenceB;
		*((int*)(P1Character1Data + 0x6920)) = WrightEvidenceC;
	}
	if (P1Character2ID == 23)
	{
		uintptr_t P1Character2Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438));
		*((int*)(P1Character2Data + 0x6918)) = WrightEvidenceA;
		*((int*)(P1Character2Data + 0x691C)) = WrightEvidenceB;
		*((int*)(P1Character2Data + 0x6920)) = WrightEvidenceC;
	}
	if (P1Character3ID == 23)
	{
		uintptr_t P1Character3Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 2));
		*((int*)(P1Character3Data + 0x6918)) = WrightEvidenceA;
		*((int*)(P1Character3Data + 0x691C)) = WrightEvidenceB;
		*((int*)(P1Character3Data + 0x6920)) = WrightEvidenceC;
	}
	if (P2Character1ID == 23)
	{
		uintptr_t P2Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 3));
		*((int*)(P2Character1Data + 0x6918)) = WrightEvidenceA;
		*((int*)(P2Character1Data + 0x691C)) = WrightEvidenceB;
		*((int*)(P2Character1Data + 0x6920)) = WrightEvidenceC;
	}
	if (P2Character2ID == 23)
	{
		uintptr_t P2Character2Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 4));
		*((int*)(P2Character2Data + 0x6918)) = WrightEvidenceA;
		*((int*)(P2Character2Data + 0x691C)) = WrightEvidenceB;
		*((int*)(P2Character2Data + 0x6920)) = WrightEvidenceC;
	}
	if (P2Character3ID == 23)
	{
		uintptr_t P2Character3Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 5));
		*((int*)(P2Character3Data + 0x6918)) = WrightEvidenceA;
		*((int*)(P2Character3Data + 0x691C)) = WrightEvidenceB;
		*((int*)(P2Character3Data + 0x6920)) = WrightEvidenceC;
	}

}

//Adjusts MODOK's level of Understanding.
void ChangeMODOKUnderstanding(int MODOKLOU)
{
	//Gets the needed values in memory.
	uintptr_t mysterytable = *(uintptr_t*)_addr(0x140d533e0);
	uintptr_t block2 = *(uintptr_t*)_addr(0x140d47e68);
	uintptr_t ptable = *(uintptr_t*)_addr(0x140d50e58);
	int P1Character1ID = *(uintptr_t*)_addr((ptable + 0x44));
	int P1Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58));
	int P1Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58));
	int P2Character1ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58));
	int P2Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58));
	int P2Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58 + 0x58));

	if (P1Character1ID == 38)
	{
		uintptr_t P1Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0);
		*((int*)(P1Character1Data + 0x68F0)) = MODOKLOU;
		*((int*)(P1Character1Data + 0x68F4)) = MODOKLOU;
	}

	if (P1Character2ID == 38)
	{
		uintptr_t P1Character2Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438));
		*((int*)(P1Character2Data + 0x68F0)) = MODOKLOU;
		*((int*)(P1Character2Data + 0x68F4)) = MODOKLOU;
	}

	if (P1Character3ID == 38)
	{
		uintptr_t P1Character3Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 2));
		*((int*)(P1Character3Data + 0x68F0)) = MODOKLOU;
		*((int*)(P1Character3Data + 0x68F4)) = MODOKLOU;
	}

	if (P2Character1ID == 38)
	{
		uintptr_t P2Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 3));
		*((int*)(P2Character1Data + 0x68F0)) = MODOKLOU;
		*((int*)(P2Character1Data + 0x68F4)) = MODOKLOU;
	}

	if (P2Character2ID == 38)
	{
		uintptr_t P2Character2Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 4));
		*((int*)(P2Character2Data + 0x68F0)) = MODOKLOU;
		*((int*)(P2Character2Data + 0x68F4)) = MODOKLOU;
	}

	if (P2Character3ID == 38)
	{
		uintptr_t P2Character3Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 5));
		*((int*)(P2Character3Data + 0x68F0)) = MODOKLOU;
		*((int*)(P2Character3Data + 0x68F4)) = MODOKLOU;
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

//Sets Dormammu Spell Charges.
void SetDormSpellLevels()
{

	//Gets the needed values in memory.
	uintptr_t mysterytable = *(uintptr_t*)_addr(0x140d533e0);
	uintptr_t block2 = *(uintptr_t*)_addr(0x140d47e68);
	uintptr_t ptable = *(uintptr_t*)_addr(0x140d50e58);
	int P1Character1ID = *(uintptr_t*)_addr((ptable + 0x44));
	int P1Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58));
	int P1Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58));
	int P2Character1ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58));
	int P2Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58));
	int P2Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58 + 0x58));

	if (*((int*)(mysterytable + 0xAA0)) != 0)
	{

		//Sets Both Spell Values.
		if (P1Character1ID == 39)
		{
			uintptr_t P1Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0);
			*((int*)(P1Character1Data + 0x68F0)) = DormRed;
			*((int*)(P1Character1Data + 0x68F4)) = DormBlue;
		}

		if (P1Character2ID == 39)
		{
			uintptr_t P1Character2Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438));
			*((int*)(P1Character2Data + 0x68F0)) = DormRed;
			*((int*)(P1Character2Data + 0x68F4)) = DormBlue;
		}

		if (P1Character3ID == 39)
		{
			uintptr_t P1Character3Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 2));
			*((int*)(P1Character3Data + 0x68F0)) = DormRed;
			*((int*)(P1Character3Data + 0x68F4)) = DormBlue;
		}

		if (P2Character1ID == 39)
		{
			uintptr_t P2Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 3));
			*((int*)(P2Character1Data + 0x68F0)) = DormRed;
			*((int*)(P2Character1Data + 0x68F4)) = DormBlue;
		}

		if (P2Character2ID == 39)
		{
			uintptr_t P2Character2Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 4));
			*((int*)(P2Character2Data + 0x68F0)) = DormRed;
			*((int*)(P2Character2Data + 0x68F4)) = DormBlue;
		}

		if (P2Character3ID == 39)
		{
			uintptr_t P2Character3Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 5));
			*((int*)(P2Character3Data + 0x68F0)) = DormRed;
			*((int*)(P2Character3Data + 0x68F4)) = DormBlue;
		}

	}

}

//Sets Infnite Install Time for Several Characters.
void EndlessInstallBoolUpdate()
{
	//Gets the needed values in memory.
	uintptr_t mysterytable = *(uintptr_t*)_addr(0x140d533e0);
	uintptr_t block2 = *(uintptr_t*)_addr(0x140d47e68);
	uintptr_t ptable = *(uintptr_t*)_addr(0x140d50e58);
	int P1Character1ID = *(uintptr_t*)_addr((ptable + 0x44));
	int P1Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58));
	int P1Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58));
	int P2Character1ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58));
	int P2Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58));
	int P2Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58 + 0x58));

	if (*((int*)(mysterytable + 0xAA0)) != 0)
	{

		//Checks for these characters as these are the ones compatible with this method.
		if (P1Character1ID == 1 || P1Character1ID == 7 || P1Character1ID == 11 || P1Character1ID == 13 || P1Character1ID == 15 || P1Character1ID == 16
			|| P1Character1ID == 17 || P1Character1ID == 20 || P1Character1ID == 21 || P1Character1ID == 22 || P1Character1ID == 23 || P1Character1ID == 24 || P1Character1ID == 28
			|| P1Character1ID == 37 || P1Character1ID == 44)
		{

			uintptr_t P1Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0);
			if (*((char*)(P1Character1Data + 0x15F8)) == 17 || *((char*)(P1Character1Data + 0x15F8)) == 16)
			{
				*((float*)(P1Character1Data + 0x1600)) = 216000;
			}
		}

		//Checks for these characters as these are the ones compatible with this method.
		if (P1Character2ID == 1 || P1Character2ID == 7 || P1Character2ID == 11 || P1Character2ID == 13 || P1Character2ID == 15 || P1Character2ID == 16
			|| P1Character2ID == 17 || P1Character2ID == 20 || P1Character2ID == 21 || P1Character2ID == 22 || P1Character2ID == 23 || P1Character2ID == 24 || P1Character2ID == 28
			|| P1Character2ID == 37 || P1Character2ID == 44)
		{

			uintptr_t P1Character2Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438));
			if (*((char*)(P1Character2Data + 0x15F8)) == 17 || *((char*)(P1Character2Data + 0x15F8)) == 16)
			{
				*((float*)(P1Character2Data + 0x1600)) = 216000;
			}
		}

		//Checks for these characters as these are the ones compatible with this method.
		if (P1Character3ID == 1 || P1Character3ID == 7 || P1Character3ID == 11 || P1Character3ID == 13 || P1Character3ID == 15 || P1Character3ID == 16
			|| P1Character3ID == 17 || P1Character3ID == 20 || P1Character3ID == 21 || P1Character3ID == 22 || P1Character3ID == 23 || P1Character3ID == 24 || P1Character3ID == 28
			|| P1Character3ID == 37 || P1Character3ID == 44)
		{

			uintptr_t P1Character3Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 2));
			if (*((char*)(P1Character3Data + 0x15F8)) == 17 || *((char*)(P1Character3Data + 0x15F8)) == 16)
			{
				*((float*)(P1Character3Data + 0x1600)) = 216000;
			}
		}

		//Checks for these characters as these are the ones compatible with this method.
		if (P2Character1ID == 1 || P2Character1ID == 7 || P2Character1ID == 11 || P2Character1ID == 13 || P2Character1ID == 15 || P2Character1ID == 16
			|| P2Character1ID == 17 || P2Character1ID == 20 || P2Character1ID == 21 || P2Character1ID == 22 || P2Character1ID == 23 || P2Character1ID == 24 || P2Character1ID == 28
			|| P2Character1ID == 37 || P2Character1ID == 44)
		{

			uintptr_t P2Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 3));
			if (*((char*)(P2Character1Data + 0x15F8)) == 17 || *((char*)(P2Character1Data + 0x15F8)) == 16)
			{
				*((float*)(P2Character1Data + 0x1600)) = 216000;
			}
		}

		//Checks for these characters as these are the ones compatible with this method.
		if (P2Character2ID == 1 || P2Character2ID == 7 || P2Character2ID == 11 || P2Character2ID == 13 || P2Character2ID == 15 || P2Character2ID == 16
			|| P2Character2ID == 17 || P2Character2ID == 20 || P2Character2ID == 21 || P2Character2ID == 22 || P2Character2ID == 23 || P2Character2ID == 24 || P2Character2ID == 28
			|| P2Character2ID == 37 || P2Character2ID == 44)
		{

			uintptr_t P2Character2Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 4));
			if (*((char*)(P2Character2Data + 0x15F8)) == 17 || *((char*)(P2Character2Data + 0x15F8)) == 16)
			{
				*((float*)(P2Character2Data + 0x1600)) = 216000;
			}
		}

		//Checks for these characters as these are the ones compatible with this method.
		if (P2Character3ID == 1 || P2Character3ID == 7 || P2Character3ID == 11 || P2Character3ID == 13 || P2Character3ID == 15 || P2Character3ID == 16
			|| P2Character3ID == 17 || P2Character3ID == 20 || P2Character3ID == 21 || P2Character3ID == 22 || P2Character3ID == 23 || P2Character3ID == 24 || P2Character3ID == 28
			|| P2Character3ID == 37 || P2Character3ID == 44)
		{

			uintptr_t P2Character3Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 5));
			if (*((char*)(P2Character3Data + 0x15F8)) == 17 || *((char*)(P2Character3Data + 0x15F8)) == 16)
			{
				*((float*)(P2Character3Data + 0x1600)) = 216000;
			}
		}
	}
}

void SetDeadpoolTeleport()
{

	//Gets the needed values in memory.
	uintptr_t mysterytable = *(uintptr_t*)_addr(0x140d533e0);
	uintptr_t block2 = *(uintptr_t*)_addr(0x140d47e68);
	uintptr_t ptable = *(uintptr_t*)_addr(0x140d50e58);
	int P1Character1ID = *(uintptr_t*)_addr((ptable + 0x44));
	int P1Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58));
	int P1Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58));
	int P2Character1ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58));
	int P2Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58));
	int P2Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58 + 0x58));

	if (*((int*)(mysterytable + 0xAA0)) != 0)
	{
		if (P1Character1ID == 40)
		{
			uintptr_t P1Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0);
			*((int*)(P1Character1Data + 0x68F0)) = DeadpoolTeleportCount;
		}

		if (P1Character2ID == 40)
		{
			uintptr_t P1Character2Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438));
			*((int*)(P1Character2Data + 0x68F0)) = DeadpoolTeleportCount;
		}

		if (P1Character3ID == 40)
		{
			uintptr_t P1Character3Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 2));
			*((int*)(P1Character3Data + 0x68F0)) = DeadpoolTeleportCount;
		}

		if (P2Character1ID == 40)
		{
			uintptr_t P2Character1Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 3));
			*((int*)(P2Character1Data + 0x68F0)) = DeadpoolTeleportCount;
		}

		if (P2Character2ID == 40)
		{
			uintptr_t P2Character2Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 4));
			*((int*)(P2Character2Data + 0x68F0)) = DeadpoolTeleportCount;
		}

		if (P2Character3ID == 40)
		{
			uintptr_t P2Character3Data = *(uintptr_t*)_addr(mysterytable + 0xAA0 + (0x438 * 5));
			*((int*)(P2Character3Data + 0x68F0)) = DeadpoolTeleportCount;
		}
	}

}

//Saves Raw Replay Data
void SaveReplayP1()
{

	uintptr_t block3 = *(uintptr_t*)_addr(0x140D510A0);
	uintptr_t P1RecordingData = *(uintptr_t*)_addr(block3 + 0x90);
	//int P1RecordingFrameTotal = *(uintptr_t*)_addr(P1RecordingData + 0x40);
	uintptr_t P1RecordingRawInput = *(uintptr_t*)_addr(P1RecordingData + 0x44);

	Recording P1Recording = {
	*(uintptr_t*)_addr(P1RecordingData + 0x40),

	};
	//*P1Recording.FrameCount = *(uintptr_t*)_addr(P1RecordingData + 0x40);



}

void DeathBoolUpdate()
{

}

//Carries over changes To restart.
void RestartWithChanges()
{
	ChangeFrankLevel(FrankLevel);
	ChangeWrightEvidence();
	ChangeMODOKUnderstanding(MODOKLOU);
	SetDormSpellLevels();
	EndlessInstallBoolUpdate();
	SetDeadpoolTeleport();
}

void EndlessXFactorUpdate()
{

	//Gets the needed values in memory. First these pointers.
	uintptr_t mysterytable = *(uintptr_t*)_addr(0x140d533e0);
	uintptr_t block2 = *(uintptr_t*)_addr(0x140d47e68);
	uintptr_t ptable = *(uintptr_t*)_addr(0x140d50e58);
	uintptr_t team1ptr = _addr((block2 + 0x350));
	uintptr_t team2ptr = _addr((block2 + 0x610));

	uintptr_t P1C1 = *(uintptr_t*)_addr(mysterytable + 0xAA0);
	uintptr_t P1C2 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 1));
	uintptr_t P1C3 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 2));
	uintptr_t P2C1 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 3));
	uintptr_t P2C2 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 4));
	uintptr_t P2C3 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 5));

	//Player 1.
	uintptr_t P1C1Hurtboxes = (P1C1 + 0x4E10);
	//std::list<Collision> P1HurtboxList;
	int HopPoint = P1C1Hurtboxes + 0x10;
	int Thing = 5379632656;




}

void TurnOnHitboxDisplay()
{

	//First we get the hurtbox data. To be continued....

}

bool CheckTheMode()
{
	//Checks for Training Mode. If not in Training Mode, it returns false.
	uintptr_t strangetable = *(uintptr_t*)_addr(0x140d50e58);
	GameMode = *((int*)(strangetable + 0x34C));
	//printf("%d\n", GameMode);
	if (GameMode == 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetIndividualCharacterHealth()
{





}
*/

struct vector {
	float X;
	float Y;
	float Z;
};

enum EMenuSubMenus {
	SM_Settings,
	SM_Total
};

struct HBoxInfo {
	vector pos;
	unsigned long color;
	bool enabled;
	float size;
	HBoxInfo(float x, float y, float z, unsigned long col, float sz, bool en) {
		pos.X = x;
		pos.Y = y;
		pos.Z = z;
		color = col;
		enabled = en;
		size = sz;
	}
};

class UMVC3Menu {
public:
	bool	 m_bIsActive = false;
	bool	 m_bSubmenuActive[SM_Total] = {};
	bool     m_bPressingKey = false;
	bool	 m_bIsFocused = false;
	bool	 m_bCustomCameraPos = false;
	bool	 m_bCustomCameraRot = false;
	bool	 m_bCustomCameraFOV = false;
	bool	 m_bFreeCam = false;
	bool	 m_bDisableHUD = false;
	bool	 m_bDisableHUDTotal = false;
	bool	 m_bDisableEffects = false;
	float	 m_fFreeCameraSpeed = 155.0f;
	float	 m_fFreeCameraRotationSpeed = 0.25f;

	int* m_pCurrentVarToChange = nullptr;

	// camera

	Matrix	 camMat = {};
	Vector   camRot = {};
	Vector   camPos = {};
	Vector   camTarget = {};
	float	 camFov = 0;


	UMVC3Menu();

	void	 OnActivate();
	void	 OnToggleFreeCamera();
	void	 OnToggleHUD();
	void	 Draw();
	void	 Process();
	void	 UpdateControls();
	void	 UpdateFreecam();

	void	 DrawCameraTab();
	void	 DrawMiscTab();



	void	 DrawSettings();


	void	 DrawKeyBind(char* name, int* var);
	void	 KeyBind(int* var, char* bindName, char* name);

	//TODO put these in better place
	static vector getP1Pos();
	static bool startedHitbox();
	static int getHBoxCount(int fighterIndex);
	static int getFighterIndex(uintptr_t ptr);
	static HBoxInfo getHBoxPos(int index, int fighterIndex);
	static HBoxInfo getHitBoxPos(int index);
	static int getHitboxCount();
	static glm::vec2 screenPosition(glm::vec3& _coord);
	static float getXMultiplier();
	static float* getXMultiplierPtr();
	static void setXMultiplier(float val);
	static float getYMultiplier();
	static float* getYMultiplierPtr();
	static void setYMultiplier(float val);
	static void setCameraPtr(uintptr_t camptr);

};


extern UMVC3Menu* TheMenu;

