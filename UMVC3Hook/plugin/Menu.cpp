//By Eternal Yoshi. Thanks to ermaccer for the new hook & HKHaan and Sheep for collecting the pointers and data needed to make this possible.
#include "Menu.h"
#include "Settings.h"
#include "..\sigscan.h"
#include "../gui/notifications.h"
#include "../gui/imgui/imgui.h"
#include "../umvc3/Camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <commdlg.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

#define undefined4 int
static int64 timer = GetTickCount64();

float p1Pos = -125.0f;
float p2Pos = 125.0f;
bool restarted = false;
int restartTimer = 0;

UMVC3Menu* TheMenu = new UMVC3Menu();

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
bool SetIndividualHP = false;
bool AlsoSetRedHealth = false;
float P1Char1Health = 1.0;
float P1Char1RedHealth = 1.0;
float P1Char2Health = 1.0;
float P1Char2RedHealth = 1.0;
float P1Char3Health = 1.0;
float P1Char3RedHealth = 1.0;
float P2Char1Health = 1.0;
float P2Char1RedHealth = 1.0;
float P2Char2Health = 1.0;
float P2Char2RedHealth = 1.0;
float P2Char3Health = 1.0;
float P2Char3RedHealth = 1.0;
bool P1Char1Slow = false;
bool P1Char2Slow = false;
bool P1Char3Slow = false;
bool P2Char1Slow = false;
bool P2Char2Slow = false;
bool P2Char3Slow = false;
bool DarkPhoenix = false;
bool Turnabout = false;

#define longlong  long long
#define ulonglong  unsigned long long
#define undefined8  long long*
#define undefined7  long long
#define undefined2  int

typedef int (*code)(longlong* param_1);
typedef int (*codenoarg)();
typedef void (*method)(void);
#define undefined int
#define undefined4 int
#define CONCAT71(a,b) (a<<32 | b)
#define backupsize 10000//100000
#define prebackup 0 //100000

constexpr int backupSize = 1000;
constexpr int teamBackupSize = 0x960;

uintptr_t inputRef;
bool inputRefSet = false;
bool recording = false;
bool recordingP1 = false;
bool recordingP2 = false;
bool replaying = false;
bool replayingP1 = false;
bool replayingP2 = false;
bool replayAvailable = false;
bool replayAvailableP1 = false;
bool replayAvailableP2 = false;

bool replayAvailable2 = false;
bool replayAvailable2P1 = false;
bool replayAvailable2P2 = false;

bool replayAvailable3 = false;
bool replayAvailable3P1 = false;
bool replayAvailable3P2 = false;

bool replayAvailable4 = false;
bool replayAvailable4P1 = false;
bool replayAvailable4P2 = false;

bool replayAvailable5 = false;
bool replayAvailable5P1 = false;
bool replayAvailable5P2 = false;

int recordReplayIndex = 0;
int recordReplayIndex2 = 0;
int recordReplayIndex3 = 0;
int recordReplayIndex4 = 0;
int recordReplayIndex5 = 0;
int recordReplayIndexP1 = 0;
int recordReplayIndex2P1 = 0;
int recordReplayIndex3P1 = 0;
int recordReplayIndex4P1 = 0;
int recordReplayIndex5P1 = 0;
int recordReplayIndexP2 = 0;
int recordReplayIndex2P2 = 0;
int recordReplayIndex3P2 = 0;
int recordReplayIndex4P2 = 0;
int recordReplayIndex5P2 = 0;
int recordedLength = 0;
int recordedLengthP1 = 0;
int recordedLengthP2 = 0;
int recordedLength2 = 0;
int recordedLength2P1 = 0;
int recordedLength2P2 = 0;
int recordedLength3 = 0;
int recordedLength3P1 = 0;
int recordedLength3P2 = 0;
int recordedLength4 = 0;
int recordedLength4P1 = 0;
int recordedLength4P2 = 0;
int recordedLength5 = 0;
int recordedLength5P1 = 0;
int recordedLength5P2 = 0;
int RecordingSlot = 1;
#define ReplayLength (60*120)
#define ReplayBufferSize 550
uint8_t replayBuffer[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer2[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer3[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer4[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer5[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer6[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer7[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer8[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer9[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer10[ReplayLength][ReplayBufferSize];

int GameMode;
std::vector<uintptr_t> hitboxes;
bool startedHitboxViewer = false;

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

struct FighterInstall
{
	int IdentifyingHashA;
	int IdentifyingHashB;
	int InstallID;
	int mType;
	float Duration;
	int PossibleRelatedAnmchrEntry;
	int mAblF;
	int mAb2F;
	float mLifeAdd;
	float RedHealthRegen;//Either Converts Yellow Health to Red Health or heals current Red Health.
	float MeterRegen;//Meter Regen.
	float YellowHealthRegen;//Gained Yellow Health becomes Red Health. Can lose total HP with this and be set to 1HP.
	float mRLifeAdd;
	float MeterGain;
	float mRLifeAddRate;
	float DamageMultiplier;
	float DefenseMultiplier;//Incoming Damage Multiplier.
	float SpeedMultiplier;
	float mSpArmorDmgRate;
	int mSpArmorShield;
	int UnknowmSpArmorShieldAddChr;
	int mUserI;
	int Unknown58;
	int Unknown5C;
	int Unknown60;
	int mUserF;
	int Unknown68;
	int Unknown6C;
	int Unknown70;
	int Unknown74;
	int Unknown78;
	int Unknown7C;

};

FighterInstall EmptyInstall =
{
	1084673544,//IdentifyingHashA
	1,//IdentifyingHashA
	0,//InstallID
	0,//mType
	0.0,//Duration
	0,//PossibleRelatedAnmchrEntry
	0,
	0,
	0,//mLifeAdd
	0,
	0,
	0,
	1.0,
	1.0,
	1.0,
	1.0,
	1.0,
	1.0,
	-1.0,
	-1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	-1,
	-0
};

FighterInstall DarkPhoenixInstall =
{
	1084673544,//IdentifyingHashA
	1,//IdentifyingHashA
	32784,//InstallID
	18,//mType
	0.0,//Duration
	224,//PossibleRelatedAnmchrEntry
	0,
	0,
	-2.34,//mLifeAdd
	0,
	0,
	0,
	1.0,
	1.0,
	1.0,
	1.0,
	1.0,
	1.0,
	-1.0,
	-1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	-1,
	-0
};

FighterInstall TurnaboutMode =
{
	1084673544,//IdentifyingHashA
	1,//IdentifyingHashA
	17,//InstallID
	10,//mType
	999999.0,//Duration
	188,//PossibleRelatedAnmchrEntry
	0,
	0,
	0,//mLifeAdd
	0,
	0,
	0,
	1.0,
	1.0,
	1.0,
	1.0,
	1.0,
	1.2,
	-1.0,
	-1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	-1,
	-0
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

//Checks Character ID. Gotta rewrite this function later.
bool CheckCharacterID(int CharacterID, int TargetID, uintptr_t mysterytable)
{
	if (CharacterID == TargetID)
	{
		return true;
	}
	else return false;


}

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

//Attempts to set characters at the specified health values.
void SetIndividualCharacterHealth()
{

	//Gets the needed values in memory.
	uintptr_t mysterytable = *(uintptr_t*)_addr(0x140d533e0);
	uintptr_t block2 = *(uintptr_t*)_addr(0x140d47e68);
	uintptr_t ptable = *(uintptr_t*)_addr(0x140d50e58);

	//Character Actor Data Pointers.
	uintptr_t P1C1 = *(uintptr_t*)_addr(mysterytable + 0xaa0 + (0x438 * 0));
	uintptr_t P1C2 = *(uintptr_t*)_addr(mysterytable + 0xaa0 + (0x438 * 1));
	uintptr_t P1C3 = *(uintptr_t*)_addr(mysterytable + 0xaa0 + (0x438 * 2));
	uintptr_t P2C1 = *(uintptr_t*)_addr(mysterytable + 0xaa0 + (0x438 * 3));
	uintptr_t P2C2 = *(uintptr_t*)_addr(mysterytable + 0xaa0 + (0x438 * 4));
	uintptr_t P2C3 = *(uintptr_t*)_addr(mysterytable + 0xaa0 + (0x438 * 5));

	//Gets the Max HP Value for the active characters. They are ints instead of floats.
	int P1C1MaxHP = *(uintptr_t*)_addr((P1C1 + 0x154C));
	int P1C2MaxHP = *(uintptr_t*)_addr((P1C2 + 0x154C));
	int P1C3MaxHP = *(uintptr_t*)_addr((P1C3 + 0x154C));
	int P2C1MaxHP = *(uintptr_t*)_addr((P2C1 + 0x154C));
	int P2C2MaxHP = *(uintptr_t*)_addr((P2C2 + 0x154C));
	int P2C3MaxHP = *(uintptr_t*)_addr((P2C3 + 0x154C));

	if (SetIndividualHP == true)
	{

		//P1 Character 1.
		*(float*)_addr(P1C1 + 0x1550) = P1C1MaxHP * P1Char1Health;

		//P1 Character 2.
		*(float*)_addr(P1C2 + 0x1550) = P1C2MaxHP * P1Char3Health;

		//P1 Character 3.
		*(float*)_addr(P1C3 + 0x1550) = P1C3MaxHP * P1Char3Health;

		//P2 Character 1.
		*(float*)_addr(P2C1 + 0x1550) = P2C1MaxHP * P2Char1Health;

		//P2 Character 2.
		*(float*)_addr(P2C2 + 0x1550) = P2C2MaxHP * P2Char2Health;

		//P2 Character 3.
		*(float*)_addr(P2C3 + 0x1550) = P2C3MaxHP * P2Char3Health;

		//Applies the requested values to the characters' red health as well.
		if (AlsoSetRedHealth == true)
		{
			//P1 Character 1.
			*(float*)_addr(P1C1 + 0x1558) = P1C1MaxHP * P1Char1Health;

			//P1 Character 2.
			*(float*)_addr(P1C2 + 0x1558) = P1C2MaxHP * P1Char3Health;

			//P1 Character 3.
			*(float*)_addr(P1C3 + 0x1558) = P1C3MaxHP * P1Char3Health;

			//P2 Character 1.
			*(float*)_addr(P2C1 + 0x1558) = P2C1MaxHP * P2Char1Health;

			//P2 Character 2.
			*(float*)_addr(P2C2 + 0x1558) = P2C2MaxHP * P2Char2Health;

			//P2 Character 3.
			*(float*)_addr(P2C3 + 0x1558) = P2C3MaxHP * P2Char3Health;
		}

	}



}

void SetSlowStatus()
{
	//Gets the needed values in memory.
	uintptr_t mysterytable = *(uintptr_t*)_addr(0x140d533e0);
	uintptr_t block2 = *(uintptr_t*)_addr(0x140d47e68);
	uintptr_t ptable = *(uintptr_t*)_addr(0x140d50e58);

	//Character Actor Data Pointers.
	uintptr_t P1C1 = *(uintptr_t*)_addr(mysterytable + 0xaa0 + (0x438 * 0));
	uintptr_t P1C2 = *(uintptr_t*)_addr(mysterytable + 0xaa0 + (0x438 * 1));
	uintptr_t P1C3 = *(uintptr_t*)_addr(mysterytable + 0xaa0 + (0x438 * 2));
	uintptr_t P2C1 = *(uintptr_t*)_addr(mysterytable + 0xaa0 + (0x438 * 3));
	uintptr_t P2C2 = *(uintptr_t*)_addr(mysterytable + 0xaa0 + (0x438 * 4));
	uintptr_t P2C3 = *(uintptr_t*)_addr(mysterytable + 0xaa0 + (0x438 * 5));










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
	//uintptr_t P1C1Hurtboxes = (P1C1 + 0x4E10);
	//std::list<Collision> P1HurtboxList;
	//int HopPoint = P1C1Hurtboxes + 0x10;
	//int Thing = 5379632656;

	//Team1.
	*(float*)_addr(team1ptr + 0xC0) = 7000.0;

	//Team2.
	*(float*)_addr(team2ptr + 0xC0) = 7000.0;


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

void PopTheBird()
{
	//Gets the needed values in memory. First these pointers.
	uintptr_t mysterytable = *(uintptr_t*)_addr(0x140d533e0);
	uintptr_t ptable = *(uintptr_t*)_addr(0x140d50e58);

	uintptr_t P1C1 = *(uintptr_t*)_addr(mysterytable + 0xAA0);
	uintptr_t P1C2 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 1));
	uintptr_t P1C3 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 2));
	uintptr_t P2C1 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 3));
	uintptr_t P2C2 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 4));
	uintptr_t P2C3 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 5));

	int P1Character1ID = *(uintptr_t*)_addr((ptable + 0x44));
	int P1Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58));
	int P1Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58));
	int P2Character1ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58));
	int P2Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58));
	int P2Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58 + 0x58));

	if (P1Character1ID == 36)
	{
		if (DarkPhoenix == true)
		{
			*((FighterInstall*)(P1C1 + 0x15F0)) = DarkPhoenixInstall;
			//For The Portrait.
			*((int*)(P1C1 + 0x6930)) = 1;
			*((int*)(P1C1 + 0x1548)) = 1;
			*((int*)(P1C1 + 0x1588)) = 0;

		}
		else
		{
			*((FighterInstall*)(P1C1 + 0x15F0)) = EmptyInstall;
			*((int*)(P1C1 + 0x6930)) = 0;
			*((int*)(P1C1 + 0x1548)) = 0;
			*((int*)(P1C1 + 0x1588)) = 16;

		}


	}
	if (P1Character2ID == 36)
	{
		if (DarkPhoenix == true)
		{
			*((FighterInstall*)(P1C2 + 0x15F0)) = DarkPhoenixInstall;
			//For The Portrait.
			*((int*)(P1C2 + 0x6930)) = 1;
			*((int*)(P1C2 + 0x1548)) = 1;
			*((int*)(P1C2 + 0x1588)) = 0;

		}
		else
		{
			*((FighterInstall*)(P1C2 + 0x15F0)) = EmptyInstall;
			*((int*)(P1C2 + 0x6930)) = 0;
			*((int*)(P1C2 + 0x1548)) = 0;
			*((int*)(P1C2 + 0x1588)) = 16;

		}
	}
	if (P1Character3ID == 36)
	{
		if (DarkPhoenix == true)
		{
			*((FighterInstall*)(P1C3 + 0x15F0)) = DarkPhoenixInstall;
			//For The Portrait.
			*((int*)(P1C3 + 0x6930)) = 1;
			*((int*)(P1C3 + 0x1548)) = 1;
			*((int*)(P1C3 + 0x1548)) = 0;
		}
		else
		{
			*((FighterInstall*)(P1C3 + 0x15F0)) = EmptyInstall;
			*((int*)(P1C3 + 0x6930)) = 0;
			*((int*)(P1C3 + 0x1548)) = 0;
			*((int*)(P1C3 + 0x1548)) = 16;
		}
	}
	if (P2Character1ID == 36)
	{
		if (DarkPhoenix == true)
		{
			*((FighterInstall*)(P2C1 + 0x15F0)) = DarkPhoenixInstall;
			//For The Portrait.
			*((int*)(P2C1 + 0x6930)) = 1;
			*((int*)(P2C1 + 0x1548)) = 1;
			*((int*)(P2C1 + 0x1548)) = 0;
		}
		else
		{
			*((FighterInstall*)(P2C1 + 0x15F0)) = EmptyInstall;
			*((int*)(P2C1 + 0x6930)) = 0;
			*((int*)(P2C1 + 0x1548)) = 0;
			*((int*)(P2C1 + 0x1548)) = 16;
		}
	}
	if (P2Character2ID == 36)
	{
		if (DarkPhoenix == true)
		{
			*((FighterInstall*)(P2C2 + 0x15F0)) = DarkPhoenixInstall;
			//For The Portrait.
			*((int*)(P2C2 + 0x6930)) = 1;
			*((int*)(P2C2 + 0x1548)) = 1;
			*((int*)(P2C2 + 0x1548)) = 0;
		}
		else
		{
			*((FighterInstall*)(P2C2 + 0x15F0)) = EmptyInstall;
			*((int*)(P2C2 + 0x6930)) = 0;
			*((int*)(P2C2 + 0x1548)) = 0;
			*((int*)(P2C2 + 0x1548)) = 16;
		}
	}
	if (P2Character3ID == 36)
	{
		if (DarkPhoenix == true)
		{
			*((FighterInstall*)(P2C3 + 0x15F0)) = DarkPhoenixInstall;
			//For The Portrait.
			*((int*)(P2C3 + 0x6930)) = 1;
			*((int*)(P2C3 + 0x1548)) = 1;
			*((int*)(P2C3 + 0x1548)) = 0;
		}
		else
		{
			*((FighterInstall*)(P2C3 + 0x15F0)) = EmptyInstall;
			*((int*)(P2C3 + 0x6930)) = 0;
			*((int*)(P2C3 + 0x1548)) = 0;
			*((int*)(P2C3 + 0x1548)) = 16;
		}
	}



}

void Objection()
{

	//Gets the needed values in memory. First these pointers.
	uintptr_t mysterytable = *(uintptr_t*)_addr(0x140d533e0);
	uintptr_t ptable = *(uintptr_t*)_addr(0x140d50e58);

	uintptr_t P1C1 = *(uintptr_t*)_addr(mysterytable + 0xAA0);
	uintptr_t P1C2 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 1));
	uintptr_t P1C3 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 2));
	uintptr_t P2C1 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 3));
	uintptr_t P2C2 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 4));
	uintptr_t P2C3 = *(uintptr_t*)_addr((mysterytable + 0xAA0) + (0x438 * 5));

	int P1Character1ID = *(uintptr_t*)_addr((ptable + 0x44));
	int P1Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58));
	int P1Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58));
	int P2Character1ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58));
	int P2Character2ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58));
	int P2Character3ID = *(uintptr_t*)_addr((ptable + 0x44 + 0x58 + 0x58 + 0x58 + 0x58 + 0x58));

	if (P1Character1ID == 23)
	{
		if (Turnabout == true)
		{
			*((FighterInstall*)(P1C1 + 0x15F0)) = TurnaboutMode;
			*((int*)(P1C1 + 0x1548)) = 2;
		}
		else
		{
			*((FighterInstall*)(P1C1 + 0x15F0)) = EmptyInstall;
			*((int*)(P1C1 + 0x1548)) = 0;
		}
	}
	if (P1Character2ID == 23)
	{
		if (Turnabout == true)
		{
			*((FighterInstall*)(P1C2 + 0x15F0)) = TurnaboutMode;
			*((int*)(P1C2 + 0x1548)) = 2;
		}
		else
		{
			*((FighterInstall*)(P1C2 + 0x15F0)) = EmptyInstall;
			*((int*)(P1C2 + 0x1548)) = 0;
		}
	}
	if (P1Character3ID == 23)
	{
		if (Turnabout == true)
		{
			*((FighterInstall*)(P1C3 + 0x15F0)) = TurnaboutMode;
			*((int*)(P1C3 + 0x1548)) = 2;
		}
		else
		{
			*((FighterInstall*)(P1C3 + 0x15F0)) = EmptyInstall;
			*((int*)(P1C3 + 0x1548)) = 0;
		}
	}
	if (P2Character1ID == 23)
	{
		if (Turnabout == true)
		{
			*((FighterInstall*)(P2C1 + 0x15F0)) = TurnaboutMode;
			*((int*)(P2C1 + 0x1548)) = 2;
		}
		else
		{
			*((FighterInstall*)(P2C1 + 0x15F0)) = EmptyInstall;
			*((int*)(P2C1 + 0x1548)) = 0;
		}
	}
	if (P2Character2ID == 23)
	{
		if (Turnabout == true)
		{
			*((FighterInstall*)(P2C2 + 0x15F0)) = TurnaboutMode;
			*((int*)(P2C2 + 0x1548)) = 2;
		}
		else
		{
			*((FighterInstall*)(P2C2 + 0x15F0)) = EmptyInstall;
			*((int*)(P2C2 + 0x1548)) = 0;
		}
	}
	if (P2Character3ID == 23)
	{
		if (Turnabout == true)
		{
			*((FighterInstall*)(P2C3 + 0x15F0)) = TurnaboutMode;
			*((int*)(P2C3 + 0x1548)) = 2;
		}
		else
		{
			*((FighterInstall*)(P2C3 + 0x15F0)) = EmptyInstall;
			*((int*)(P2C3 + 0x1548)) = 0;
		}
	}

}

static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

UMVC3Menu::UMVC3Menu()
{
}

void UMVC3Menu::OnActivate()
{
	m_bIsActive ^= 1;
}

void UMVC3Menu::OnToggleFreeCamera()
{
	if (m_bIsActive)
		return;

	m_bFreeCam ^= 1;
}

void UMVC3Menu::OnToggleHUD()
{
	if (m_bIsActive)
		return;

	m_bDisableHUD ^= 1;
}

void GetTheImportantAddresses()
{

	auto sigger = sigscan::get();

	if (pAddrSet == false)
	{
		pAddrSet = true;
		auto mysterytable = *(uintptr_t*)_addr(0x140d533e0);
		auto block2 = *(uintptr_t*)_addr(0x140d47e68);
		for (size_t i = 0; i < 6; i++)
		{
			scriptableFighters[i].fighter = *(Fighter**)(mysterytable + 0xaa0 + (0x438 * i));
		}
	}
	else {
		for (size_t i = 0; i < 6; i++)
		{
			if (scriptableFighters[i].name == nullptr) {
				ImGui::Text("p%d:%llx", i, scriptableFighters[i].fighter);
			}
			else {
				ImGui::Text("p%d:%llx %llx %s", i, scriptableFighters[i].fighter, scriptableFighters[i].tickPtr, scriptableFighters[i].name);
			}
			if (scriptableFighters[i].fighter->tagState == ETagState::Active || scriptableFighters[i].fighter->tagState == ETagState::Active2)
			{
				ImGui::Text("  X:%d", (int)scriptableFighters[i].fighter->vector.x);
				ImGui::SameLine();
				ImGui::Text("- Y:%d", (int)scriptableFighters[i].fighter->vector.y);
				ImGui::SameLine();
				ImGui::Text("- Vel:%d", (int)scriptableFighters[i].fighter->vector.xVelocity);

				ImGui::Text("  Anim:%x", scriptableFighters[i].fighter->anim);
				ImGui::Text("  Special:%x", scriptableFighters[i].fighter->specialState);

				ImGui::Text("  Grounded:%s", scriptableFighters[i].GetGroundedState());
				ImGui::Text("  TagState:%s", scriptableFighters[i].GetTagState());
			}
		}


		auto block2 = *(uintptr_t*)_addr(0x140d47e68);
		auto team1 = block2 + 0x350;
		auto team2 = block2 + 0x610;
		ImGui::Text("Team1:%llx ActiveFighter %x", team1, team1 + 0x48);
		ImGui::Text("Team2:%llx ActiveFighter %x", team2, team2 + 0x48);
	}

}

//Based on the hook by SanHKHaan. Modified for use with multiple slots.
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

void UMVC3Menu::Draw()
{
	if (!m_bIsActive)
		return;
	FILE* pRec;
	Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
	InjectHook(_addr(0x140289c5a), tramp->Jump(FUN_1402b41b0), PATCH_CALL);
	auto block2 = *(uintptr_t*)_addr(0x140d47e68);
	auto val = *(unsigned char*)(block2 + 0x118);
	if (val != 0)
	{
		restarted = true;
		restartTimer = 0;
	}
	else if (restarted)
	{
		GetTheImportantAddresses();
		hitboxes.clear();
		restartTimer += 1;
		if (restartTimer > 5)//0.166667 sec wait
		{
			RestartWithChanges();

			{
				auto ptr = ((uintptr_t)scriptableFighters[0].fighter) + 0x50;
				*((float*)ptr) = p1Pos;
			}
			{
				auto ptr = ((uintptr_t)scriptableFighters[3].fighter) + 0x50;
				*((float*)ptr) = p2Pos;
			}
			restarted = false;
			SetIndividualCharacterHealth();

		}


	}

	ImGui::GetIO().MouseDrawCursor = true;

	ImGui::Begin("UMVC3 Training Tools Mod V0.3 by Eternal Yoshi", &m_bIsActive, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoNav);
	//ImGui::BeginChild("##scrolling", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

	//Start of my port of the Training Mode Tools mod I made in Spring. Still more to do.
	ImGui::Text("Toggle this with F1");
	ImGui::Separator();
	if (ImGui::BeginTabBar("##tabs"))
	{

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

		if (EndlessXFactor == true)
		{
			if (CheckTheMode() == true)
			{
				EndlessXFactorUpdate();
			}
		}

		if (DarkPhoenix == true)
		{
			PopTheBird();
		}

		//General training settings.
		if (ImGui::BeginTabItem("Extra Settings"))
		{
			ImGui::Text("Remember! These Parameters will only take\neffect when this window is open.");

			ImGui::Separator();

			if (ImGui::Button("Restart Training Mode"))
			{
				if (CheckTheMode() == true)
				{
					*(unsigned char*)(block2 + 0x118) = 3;
				}
			}

			ImGui::SeparatorText("Positioning");

			ImGui::Text("Player 1 Position");
			if (ImGui::SliderFloat("P1 X Position", &p1Pos, -800.0f, 800.0f))
			{

			}

			ImGui::Text("Player 2 Position");
			if (ImGui::SliderFloat("P2 X Position", &p2Pos, -800.0f, 800.0f))
			{

			}

			ImGui::Text("Position Presets");
			if (ImGui::Button("Default"))
			{

				p1Pos = -125.0f;
				p2Pos = 125.0f;


			}
			ImGui::SameLine();
			if (ImGui::Button("Default P2 Side"))
			{

				p1Pos = 125.0f;
				p2Pos = -125.0f;


			}
			if (ImGui::Button("Left Corner"))
			{

				p1Pos = -750.0f;
				p2Pos = -800.0f;


			}
			ImGui::SameLine();
			if (ImGui::Button("Left Corner P2 Side"))
			{

				p1Pos = -800.0f;
				p2Pos = -750.0f;


			}

			if (ImGui::Button("Right Corner"))
			{

				p1Pos = 750.0f;
				p2Pos = 800.0f;


			}
			ImGui::SameLine();
			if (ImGui::Button("Right Corner P2 Side"))
			{

				p1Pos = 800.0f;
				p2Pos = 750.0f;


			}

			ImGui::SeparatorText("Etc");

			if (ImGui::Checkbox("Endless X-Factor", &EndlessXFactor))
			{

			}

			ImGui::Separator();

			ImGui::Text("More features coming soon!");

			if (SaveStateSlot)
			{
				if (ImGui::SliderInt("Save State Slot", &SaveStateSlot, 1, 10))
				{

				}

				if (ImGui::Button("Save"))
				{

				}

				if (ImGui::Button("Load"))
				{

				}

			}


			ImGui::SeparatorText("Incomplete Stuff");
			if (ImGui::Button("Turn On Hitboxes"))
			{
				if (CheckTheMode() == true)
				{
					
					hitboxes.clear();
					startedHitboxViewer = false;
					for (uint8_t* i = (uint8_t*)0x14200000; i < (uint8_t*)0x14900000; i++)
					{
						auto t = *(uintptr_t*)i;
						if (t == 0x140a6b000) {
							printf("hitboxes %llx\n", i);
							hitboxes.push_back((uintptr_t)i);
						}
						pAddrSet = true;
						auto mysterytable = *(uintptr_t*)_addr(0x140d533e0);
						auto block2 = *(uintptr_t*)_addr(0x140d47e68);
						for (size_t i = 0; i < 6; i++)
						{
							scriptableFighters[i].fighter = *(Fighter**)(mysterytable + 0xaa0 + (0x438 * i));
						}
					}
					
				}
			}
			if (ImGui::Button("Turn Off Hitboxes"))
			{
				
				if (CheckTheMode() == true)
				{
					hitboxes.clear();
					startedHitboxViewer = false;
					pAddrSet = false;
					for (size_t i = 0; i < 6; i++)
					{
						scriptableFighters[i].fighter = nullptr;
					}
				}
				
			}

			ImGui::Separator();

			ImGui::Text("V0.3 By Eternal Yoshi & HkHaan96\nand Ermaccer for the original hook");

			ImGui::EndTabItem();

		}

		//Player and Status Related  Settings such as Health.
		if (ImGui::BeginTabItem("Status Options"))
		{
			ImGui::Text("Remember! These Parameters will only take\neffect when this window is open.");

			ImGui::SeparatorText("Character HP");

			//Toggle for using individual character health.
			if (ImGui::Checkbox("Set Individual Character Health(Applies during restarts).", &SetIndividualHP))
			{

			}

			//Toggle for Applying Health Settings to Red Health.
			if (ImGui::Checkbox("Also Apply To Red Health", &AlsoSetRedHealth))
			{

			}

			if (P1Char1Health)
			{
				ImGui::Text("Player 1 Character 1 Health");
				if (ImGui::SliderFloat("P1C1 HP %", &P1Char1Health, 0.01f, 1.0f))
				{
					if (CheckTheMode() == true)
					{

					}
				}
			}

			if (P1Char2Health)
			{
				ImGui::Text("Player 1 Character 2 Health");
				if (ImGui::SliderFloat("P1C2 HP %", &P1Char2Health, 0.01f, 1.0f))
				{
					if (CheckTheMode() == true)
					{

					}
				}
			}

			if (P1Char3Health)
			{
				ImGui::Text("Player 1 Character 3 Health");
				if (ImGui::SliderFloat("P1C3 HP %", &P1Char3Health, 0.01f, 1.0f))
				{
					if (CheckTheMode() == true)
					{

					}
				}
			}

			if (P2Char1Health)
			{
				ImGui::Text("Player 2 Character 1 Health");
				if (ImGui::SliderFloat("P2C1 HP %", &P2Char1Health, 0.01f, 1.0f))
				{
					if (CheckTheMode() == true)
					{

					}
				}
			}

			if (P2Char2Health)
			{
				ImGui::Text("Player 2 Character 2 Health");
				if (ImGui::SliderFloat("P2C2 HP %", &P2Char2Health, 0.01f, 1.0f))
				{
					if (CheckTheMode() == true)
					{

					}
				}
			}

			if (P2Char3Health)
			{
				ImGui::Text("Player 2 Character 3 Health");
				if (ImGui::SliderFloat("P2C3 HP %", &P2Char3Health, 0.01f, 1.0f))
				{
					if (CheckTheMode() == true)
					{

					}
				}
			}

			ImGui::Separator();

			ImGui::Text("V0.3 By Eternal Yoshi, thanks to HkHaan96 for help\n and Ermaccer for the original hook");

			ImGui::EndTabItem();

		}

		//Character specific parameters like Frank West levels.
		if (ImGui::BeginTabItem("Character Settings"))
		{

			ImGui::Text("Remember! These Parameters will only take\neffect when this window is open.");

			ImGui::SeparatorText("Frank West");

			if (FrankLevel)
			{
				ImGui::Text("Frank West's Level");
				if (ImGui::SliderInt("Lv", &FrankLevel, 1, 5))
				{
					if (CheckTheMode() == true)
					{
						ChangeFrankLevel(FrankLevel);
					}
				}
			}

			ImGui::SeparatorText("Phoenix Wright");

			ImGui::Text("Mr. Wright's Evidence");
			if (WrightEvidenceA || WrightEvidenceB || WrightEvidenceC)
			{
				ImGui::Text("Evidence Slot 1");

				if (ImGui::BeginCombo("##WrightEvidencecombo", selected_item))
				{
					if (CheckTheMode() == true)
					{
						for (int n = 0; n < IM_ARRAYSIZE(EvidenceTypes); n++)
						{
							bool is_selected = (selected_item == EvidenceTypes[n]);
							if (ImGui::Selectable(EvidenceTypes[n], is_selected))
							{
								selected_item = EvidenceTypes[n];
								WrightEvidenceA = EvidenceNameToID(selected_item);
								ChangeWrightEvidence();

							}
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				ImGui::Text("Evidence Slot 2");

				if (ImGui::BeginCombo("##WrightEvidencecombo2", selected_itemTwo))
				{
					if (CheckTheMode() == true)
					{
						for (int n = 0; n < IM_ARRAYSIZE(EvidenceTypesTwo); n++)
						{
							bool is_selected = (selected_itemTwo == EvidenceTypesTwo[n]);
							if (ImGui::Selectable(EvidenceTypesTwo[n], is_selected))
							{
								selected_itemTwo = EvidenceTypesTwo[n];
								WrightEvidenceB = EvidenceNameToID(selected_itemTwo);
								ChangeWrightEvidence();

							}
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				ImGui::Text("Evidence Slot 3");

				if (ImGui::BeginCombo("##WrightEvidencecombo3", selected_itemThree))
				{
					if (CheckTheMode() == true)
					{
						for (int n = 0; n < IM_ARRAYSIZE(EvidenceTypesThree); n++)
						{
							bool is_selected = (selected_itemThree == EvidenceTypesThree[n]);
							if (ImGui::Selectable(EvidenceTypesThree[n], is_selected))
							{
								selected_itemThree = EvidenceTypesThree[n];
								WrightEvidenceC = EvidenceNameToID(selected_itemThree);
								ChangeWrightEvidence();

							}
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

			}

			ImGui::Text("Turnabout Toggle");
			if (ImGui::Checkbox("TurnaboutToggle", &Turnabout))
			{
				if (CheckTheMode() == true)
				{
					Objection();
				}
			}

			ImGui::SeparatorText("Phoenix/Jean");
			ImGui::Text("Dark Phoenix Toggle");
			if (ImGui::Checkbox("DarkPhoenixToggle", &DarkPhoenix))
			{
				if (CheckTheMode() == true)
				{
					PopTheBird();
				}
			}


			ImGui::SeparatorText("MODOK");
			if (MODOKLOU)
			{
				ImGui::Text("MODOK Level of Understanding");
				if (ImGui::SliderInt("LOU", &MODOKLOU, 1, 10))
				{
					if (CheckTheMode() == true)
					{
						ChangeMODOKUnderstanding(MODOKLOU);
					}
				}
			}

			ImGui::SeparatorText("Dormammu");
			ImGui::Text("Dormammu's Spell Charges");

			ImGui::Text("Power Of The Destructor/Red");
			if (ImGui::SliderInt("DRed", &DormRed, 0, 3))
			{
				if (CheckTheMode() == true)
				{
					DormSpellSet = true;
					SetDormSpellLevels();
				}
			}

			ImGui::Text("Power Of The Creator/Blue");
			if (ImGui::SliderInt("DBlue", &DormBlue, 0, 3))
			{
				if (CheckTheMode() == true)
				{
					DormSpellSet = true;
					SetDormSpellLevels();
				}
			}

			ImGui::SeparatorText("Deadpool");
			ImGui::Text("Deadpool Teleport");
			if (ImGui::SliderInt("TeleportCount", &DeadpoolTeleportCount, 0, 2))
			{
				if (CheckTheMode() == true)
				{
					SetDeadpoolTeleport();
				}
			}

			if (ImGui::Checkbox("LockTeleportCount", &FreezeDeadpoolTPCounter))
			{
				if (CheckTheMode() == true)
				{
				}
			}

			ImGui::SeparatorText("Etc.");
			ImGui::Text("Endless Install Toggle");
			if (ImGui::Checkbox("EndlessInstallToggle", &EndlessInstalls))
			{
				if (CheckTheMode() == true)
				{
					EndlessInstallBoolUpdate();
				}
			}

			ImGui::Separator();



			/*
			ImGui::Text("Player 1 Character 1 Death Toggle");
			if (ImGui::Checkbox("IsP1C1Dead", &P1Character1Dead))
			{

			}

			ImGui::Text("Player 1 Character 2 Death Toggle");
			if (ImGui::Checkbox("IsP1C2Dead", &P1Character2Dead))
			{

			}

			ImGui::Text("Player 1 Character 3 Death Toggle");

			if (ImGui::Checkbox("IsP1C3Dead", &P1Character3Dead))
			{

			}

			ImGui::Text("Player 2 Character 1 Death Toggle");

			if (ImGui::Checkbox("IsP2C31Dead", &P2Character1Dead))
			{

			}

			ImGui::Text("Player 2 Character 2 Death Toggle");
			if (ImGui::Checkbox("IsP2C2Dead", &P2Character2Dead))
			{

			}

			ImGui::Text("Player 2 Character 3 Death Toggle");
			if (ImGui::Checkbox("IsP2C3Dead", &P2Character3Dead))
			{

			}
			*/


			ImGui::Separator();

			ImGui::Text("Turnabout Mode Toggle\n(Coming whenever I can figure it out!)");

			ImGui::Separator();

			ImGui::Text("V0.3 By Eternal Yoshi, thanks to HkHaan96 for help\nand Ermaccer for the original hook");

			ImGui::EndTabItem();

		}

		//For Recording And Playback Stuff.
		if (ImGui::BeginTabItem("Record & Playback"))
		{

			ImGui::Text("Remember! These Parameters will only take\neffect when this window is open.");


			switch (RecordingSlot)
			{

			case 2:

				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record Both")) {
						if (CheckTheMode() == true)
						{
							//recording = true;
							//replaying = false;
							recordingP1 = true;
							replayingP1 = false;
							recordingP2 = true;
							replayingP2 = false;
							recordReplayIndex2P1 = 0;
							recordReplayIndex2P2 = 0;

						}
					}
				}
				//ImGui::SameLine();
				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record P1")) {
						if (CheckTheMode() == true)
						{
							recordingP1 = true;
							replayingP1 = false;
							//recording = false;
							//replaying = false;
							recordingP2 = false;
							replayingP2 = false;
							recordReplayIndex2P1 = 0;
						}
					}
					ImGui::SameLine();
				}
				//ImGui::SameLine();
				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record P2")) {
						if (CheckTheMode() == true)
						{
							recordingP2 = true;
							replayingP2 = false;
							//recording = false;
							//replaying = false;
							recordingP1 = false;
							replayingP1 = false;
							recordReplayIndex2P2 = 0;
						}
					}
				}

				else if (recordReplayIndex2P1 > 90 || recordReplayIndex2P2 > 90) {
					if (ImGui::Button("Stop Recording")) {
						if (CheckTheMode() == true)
						{
							//recording = false;
							recordingP1 = false;
							recordingP2 = false;
							recordedLength2P1 = recordReplayIndex2P1 - 1;
							recordedLength2P2 = recordReplayIndex2P2 - 1;
						}
					}
				}

				//Playback Code Below.
				ImGui::Separator();

				if (replayAvailable2P1 && replayAvailable2P2 && !recordingP1 && !recordingP2) {
					if (!replayingP1 && !replayingP2) {
						if (ImGui::Button("Playback Both")) {
							if (CheckTheMode() == true)
							{

								replayingP1 = true;
								replayingP2 = true;
								recordReplayIndex2 = 0;
								recordReplayIndex2P1 = 0;
								recordReplayIndex2P2 = 0;

							}

						}
					}
					else if (recordReplayIndex2P1 > 90 || recordReplayIndex2P2 > 90) {
						if (ImGui::Button("Stop Playback"))
						{
							if (CheckTheMode() == true)
							{
								replayingP1 = false;
								replayingP2 = false;

							}
						}
					}
				}

				if (replayAvailable2P1 && !recordingP1) {
					if (!replayingP1) {
						if (ImGui::Button("Playback P1")) {
							if (CheckTheMode() == true)
							{
								replayingP1 = true;
								recordReplayIndex2P1 = 0;
							}

						}
					}
					else if (recordReplayIndex2P1 > 90) {
						if (ImGui::Button("Stop Playback P1"))
						{
							if (CheckTheMode() == true)
							{
								replayingP1 = false;
							}
						}
					}
					ImGui::SameLine();
				}

				if (replayAvailable2P2 && !recordingP2) {
					if (!replayingP2) {
						if (ImGui::Button("Playback P2")) {
							if (CheckTheMode() == true)
							{
								replayingP2 = true;
								recordReplayIndex2P2 = 0;
							}

						}
					}
					else if (recordReplayIndex2P2 > 90) {
						if (ImGui::Button("Stop Playback P2"))
						{
							if (CheckTheMode() == true)
							{
								replayingP2 = false;
							}
						}
					}
				}


				ImGui::Separator();

				//Saves Recordings. Needs to be adjusted.
				if (ImGui::Button("Save P1 Recording"))
				{
					if (CheckTheMode() == true)
					{
						if (!recording && replayAvailable2P1)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };

							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
							if (GetSaveFileName(&ofn))
							{
								pRec = fopen(ofn.lpstrFile, "wb");
								fwrite(replayBuffer3, 550, recordedLength2P1, pRec);
								fclose(pRec);
							}
						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Save P2 Recording"))
				{
					if (CheckTheMode() == true)
					{
						if (!recording && replayAvailable2P2)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };

							char szFile[_MAX_PATH] = "RecP2";
							const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetSaveFileName(&ofn))
							{
								pRec = fopen(ofn.lpstrFile, "wb");
								fwrite(replayBuffer4, 550, recordedLength2P2, pRec);
								fclose(pRec);

							}
						}
					}
				}

				if (ImGui::Button("Load Recording in P1"))
				{
					if (CheckTheMode() == true)
					{
						if (!recordingP1 && !replayingP1)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };
							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetOpenFileName(&ofn))
							{
								if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
								{
									MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

								}
								else
								{
									//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
									std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
									mRP.unsetf(std::ios::skipws);
									std::ifstream::pos_type pos = mRP.tellg();
									int filesize = pos;

									if (filesize % 550 != 0)
									{
										MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
									}
									else
									{
										//Gets the file size, sets the needed variables to play the replay when loaded, 
										//and then copies the file into an array in memory.
										int length = pos / 550;
										replayAvailable2P1 = true;
										recordedLength2P1 = length;
										recordReplayIndex2P1 = 0;

										mRP.seekg(0, std::ios::beg);

										std::vector<BYTE> pChars;
										pChars.reserve(filesize);

										pChars.insert(pChars.begin(),
											std::istream_iterator<BYTE>(mRP),
											std::istream_iterator<BYTE>());

										//For loop for inserting the frames in the proper place. Based on code from recording.
										//Need to fix this.
										for (int i = 0; i < recordedLength2P1; i++)
										{
											memcpy(&replayBuffer3[i], &pChars[(i * 550)], ReplayBufferSize);
										}
									}


								}

							}


						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Load Recording in P2"))
				{
					if (CheckTheMode() == true)
					{
						if (!recordingP2 && !replayingP2)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };
							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetOpenFileName(&ofn))
							{
								if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
								{
									MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

								}
								else
								{
									//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
									std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
									mRP.unsetf(std::ios::skipws);
									std::ifstream::pos_type pos = mRP.tellg();
									int filesize = pos;

									if (filesize % 550 != 0)
									{
										MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
									}
									else
									{
										//Gets the file size, sets the needed variables to play the replay when loaded, 
										//and then copies the file into an array in memory.
										int length = pos / 550;
										replayAvailable2P2 = true;
										recordedLength2P2 = length;
										recordReplayIndex2P2 = 0;

										mRP.seekg(0, std::ios::beg);

										std::vector<BYTE> pChars;
										pChars.reserve(filesize);

										pChars.insert(pChars.begin(),
											std::istream_iterator<BYTE>(mRP),
											std::istream_iterator<BYTE>());

										//For loop for inserting the frames in the proper place. Based on code from recording.
										for (int i = 0; i < recordedLength2P2; i++)
										{
											memcpy(&replayBuffer4[i], &pChars[(i * 550)], ReplayBufferSize);
										}
									}


								}

							}


						}
					}
				}


				break;

			case 3:

				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record Both")) {
						if (CheckTheMode() == true)
						{
							//recording = true;
							//replaying = false;
							recordingP1 = true;
							replayingP1 = false;
							recordingP2 = true;
							replayingP2 = false;
							recordReplayIndex3P1 = 0;
							recordReplayIndex3P2 = 0;

						}
					}
				}
				//ImGui::SameLine();
				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record P1")) {
						if (CheckTheMode() == true)
						{
							recordingP1 = true;
							replayingP1 = false;
							//recording = false;
							//replaying = false;
							recordingP2 = false;
							replayingP2 = false;
							recordReplayIndex3P1 = 0;
						}
					}
					ImGui::SameLine();
				}
				//ImGui::SameLine();
				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record P2")) {
						if (CheckTheMode() == true)
						{
							recordingP2 = true;
							replayingP2 = false;
							//recording = false;
							//replaying = false;
							recordingP1 = false;
							replayingP1 = false;
							recordReplayIndex3P2 = 0;
						}
					}
				}

				else if (recordReplayIndex3P1 > 90 || recordReplayIndex3P2 > 90) {
					if (ImGui::Button("Stop Recording")) {
						if (CheckTheMode() == true)
						{
							//recording = false;
							recordingP1 = false;
							recordingP2 = false;
							recordedLength3P1 = recordReplayIndex3P1 - 1;
							recordedLength3P2 = recordReplayIndex3P2 - 1;
						}
					}
				}

				//Playback Code Below.
				ImGui::Separator();

				if (replayAvailable3P1 && replayAvailable3P2 && !recordingP1 && !recordingP2) {
					if (!replayingP1 && !replayingP2) {
						if (ImGui::Button("Playback Both")) {
							if (CheckTheMode() == true)
							{

								replayingP1 = true;
								replayingP2 = true;
								recordReplayIndex3 = 0;
								recordReplayIndex3P1 = 0;
								recordReplayIndex3P2 = 0;

							}

						}
					}
					else if (recordReplayIndex3P1 > 90 || recordReplayIndex3P2 > 90) {
						if (ImGui::Button("Stop Playback"))
						{
							if (CheckTheMode() == true)
							{
								replayingP1 = false;
								replayingP2 = false;

							}
						}
					}
				}

				if (replayAvailable3P1 && !recordingP1) {
					if (!replayingP1) {
						if (ImGui::Button("Playback P1")) {
							if (CheckTheMode() == true)
							{
								replayingP1 = true;
								recordReplayIndex3P1 = 0;
							}

						}
					}
					else if (recordReplayIndex3P1 > 90) {
						if (ImGui::Button("Stop Playback P1"))
						{
							if (CheckTheMode() == true)
							{
								replayingP1 = false;
							}
						}
					}
					ImGui::SameLine();
				}

				if (replayAvailable3P2 && !recordingP2) {
					if (!replayingP2) {
						if (ImGui::Button("Playback P2")) {
							if (CheckTheMode() == true)
							{
								replayingP2 = true;
								recordReplayIndex3P2 = 0;
							}

						}
					}
					else if (recordReplayIndex3P2 > 90) {
						if (ImGui::Button("Stop Playback P2"))
						{
							if (CheckTheMode() == true)
							{
								replayingP2 = false;
							}
						}
					}
				}


				ImGui::Separator();

				//Saves Recordings. Needs to be adjusted.
				if (ImGui::Button("Save P1 Recording"))
				{
					if (CheckTheMode() == true)
					{
						if (!recording && replayAvailable3P1)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };

							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
							if (GetSaveFileName(&ofn))
							{
								pRec = fopen(ofn.lpstrFile, "wb");
								fwrite(replayBuffer5, 550, recordedLength3P1, pRec);
								fclose(pRec);
							}
						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Save P2 Recording"))
				{
					if (CheckTheMode() == true)
					{
						if (!recording && replayAvailable3P2)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };

							char szFile[_MAX_PATH] = "RecP2";
							const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetSaveFileName(&ofn))
							{
								pRec = fopen(ofn.lpstrFile, "wb");
								fwrite(replayBuffer6, 550, recordedLength3P2, pRec);
								fclose(pRec);

							}
						}
					}
				}

				if (ImGui::Button("Load Recording in P1"))
				{
					if (CheckTheMode() == true)
					{
						if (!recordingP1 && !replayingP1)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };
							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetOpenFileName(&ofn))
							{
								if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
								{
									MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

								}
								else
								{
									//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
									std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
									mRP.unsetf(std::ios::skipws);
									std::ifstream::pos_type pos = mRP.tellg();
									int filesize = pos;

									if (filesize % 550 != 0)
									{
										MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
									}
									else
									{
										//Gets the file size, sets the needed variables to play the replay when loaded, 
										//and then copies the file into an array in memory.
										int length = pos / 550;
										replayAvailable3P1 = true;
										recordedLength3P1 = length;
										recordReplayIndex3P1 = 0;

										mRP.seekg(0, std::ios::beg);

										std::vector<BYTE> pChars;
										pChars.reserve(filesize);

										pChars.insert(pChars.begin(),
											std::istream_iterator<BYTE>(mRP),
											std::istream_iterator<BYTE>());

										//For loop for inserting the frames in the proper place. Based on code from recording.
										//Need to fix this.
										for (int i = 0; i < recordedLength3P1; i++)
										{
											memcpy(&replayBuffer5[i], &pChars[(i * 550)], ReplayBufferSize);
										}
									}


								}

							}


						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Load Recording in P2"))
				{
					if (CheckTheMode() == true)
					{
						if (!recordingP2 && !replayingP2)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };
							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetOpenFileName(&ofn))
							{
								if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
								{
									MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

								}
								else
								{
									//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
									std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
									mRP.unsetf(std::ios::skipws);
									std::ifstream::pos_type pos = mRP.tellg();
									int filesize = pos;

									if (filesize % 550 != 0)
									{
										MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
									}
									else
									{
										//Gets the file size, sets the needed variables to play the replay when loaded, 
										//and then copies the file into an array in memory.
										int length = pos / 550;
										replayAvailable3P2 = true;
										recordedLength3P2 = length;
										recordReplayIndex3P2 = 0;

										mRP.seekg(0, std::ios::beg);

										std::vector<BYTE> pChars;
										pChars.reserve(filesize);

										pChars.insert(pChars.begin(),
											std::istream_iterator<BYTE>(mRP),
											std::istream_iterator<BYTE>());

										//For loop for inserting the frames in the proper place. Based on code from recording.
										for (int i = 0; i < recordedLength3P2; i++)
										{
											memcpy(&replayBuffer6[i], &pChars[(i * 550)], ReplayBufferSize);
										}
									}


								}

							}


						}
					}
				}


				break;

			case 4:

				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record Both")) {
						if (CheckTheMode() == true)
						{
							//recording = true;
							//replaying = false;
							recordingP1 = true;
							replayingP1 = false;
							recordingP2 = true;
							replayingP2 = false;
							recordReplayIndex4P1 = 0;
							recordReplayIndex4P2 = 0;

						}
					}
				}
				//ImGui::SameLine();
				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record P1")) {
						if (CheckTheMode() == true)
						{
							recordingP1 = true;
							replayingP1 = false;
							//recording = false;
							//replaying = false;
							recordingP2 = false;
							replayingP2 = false;
							recordReplayIndex4P1 = 0;
						}
					}
					ImGui::SameLine();
				}
				//ImGui::SameLine();
				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record P2")) {
						if (CheckTheMode() == true)
						{
							recordingP2 = true;
							replayingP2 = false;
							//recording = false;
							//replaying = false;
							recordingP1 = false;
							replayingP1 = false;
							recordReplayIndex4P2 = 0;
						}
					}
				}

				else if (recordReplayIndex4P1 > 90 || recordReplayIndex4P2 > 90) {
					if (ImGui::Button("Stop Recording")) {
						if (CheckTheMode() == true)
						{
							//recording = false;
							recordingP1 = false;
							recordingP2 = false;
							recordedLength4P1 = recordReplayIndex4P1 - 1;
							recordedLength4P2 = recordReplayIndex4P2 - 1;
						}
					}
				}

				//Playback Code Below.
				ImGui::Separator();

				if (replayAvailable4P1 && replayAvailable4P2 && !recordingP1 && !recordingP2) {
					if (!replayingP1 && !replayingP2) {
						if (ImGui::Button("Playback Both")) {
							if (CheckTheMode() == true)
							{

								replayingP1 = true;
								replayingP2 = true;
								recordReplayIndex4 = 0;
								recordReplayIndex4P1 = 0;
								recordReplayIndex4P2 = 0;

							}

						}
					}
					else if (recordReplayIndex4P1 > 90 || recordReplayIndex4P2 > 90) {
						if (ImGui::Button("Stop Playback"))
						{
							if (CheckTheMode() == true)
							{
								replayingP1 = false;
								replayingP2 = false;

							}
						}
					}
				}

				if (replayAvailable4P1 && !recordingP1) {
					if (!replayingP1) {
						if (ImGui::Button("Playback P1")) {
							if (CheckTheMode() == true)
							{
								replayingP1 = true;
								recordReplayIndex4P1 = 0;
							}

						}
					}
					else if (recordReplayIndex4P1 > 90) {
						if (ImGui::Button("Stop Playback P1"))
						{
							if (CheckTheMode() == true)
							{
								replayingP1 = false;
							}
						}
					}
					ImGui::SameLine();
				}

				if (replayAvailable4P2 && !recordingP2) {
					if (!replayingP2) {
						if (ImGui::Button("Playback P2")) {
							if (CheckTheMode() == true)
							{
								replayingP2 = true;
								recordReplayIndex4P2 = 0;
							}

						}
					}
					else if (recordReplayIndex4P2 > 90) {
						if (ImGui::Button("Stop Playback P2"))
						{
							if (CheckTheMode() == true)
							{
								replayingP2 = false;
							}
						}
					}
				}


				ImGui::Separator();

				//Saves Recordings. Needs to be adjusted.
				if (ImGui::Button("Save P1 Recording"))
				{
					if (CheckTheMode() == true)
					{
						if (!recording && replayAvailable4P1)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };

							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
							if (GetSaveFileName(&ofn))
							{
								pRec = fopen(ofn.lpstrFile, "wb");
								fwrite(replayBuffer7, 550, recordedLength4P1, pRec);
								fclose(pRec);
							}
						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Save P2 Recording"))
				{
					if (CheckTheMode() == true)
					{
						if (!recording && replayAvailable4P2)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };

							char szFile[_MAX_PATH] = "RecP2";
							const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetSaveFileName(&ofn))
							{
								pRec = fopen(ofn.lpstrFile, "wb");
								fwrite(replayBuffer8, 550, recordedLength4P2, pRec);
								fclose(pRec);

							}
						}
					}
				}

				if (ImGui::Button("Load Recording in P1"))
				{
					if (CheckTheMode() == true)
					{
						if (!recordingP1 && !replayingP1)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };
							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetOpenFileName(&ofn))
							{
								if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
								{
									MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

								}
								else
								{
									//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
									std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
									mRP.unsetf(std::ios::skipws);
									std::ifstream::pos_type pos = mRP.tellg();
									int filesize = pos;

									if (filesize % 550 != 0)
									{
										MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
									}
									else
									{
										//Gets the file size, sets the needed variables to play the replay when loaded, 
										//and then copies the file into an array in memory.
										int length = pos / 550;
										replayAvailable4P1 = true;
										recordedLength4P1 = length;
										recordReplayIndex4P1 = 0;

										mRP.seekg(0, std::ios::beg);

										std::vector<BYTE> pChars;
										pChars.reserve(filesize);

										pChars.insert(pChars.begin(),
											std::istream_iterator<BYTE>(mRP),
											std::istream_iterator<BYTE>());

										//For loop for inserting the frames in the proper place. Based on code from recording.
										//Need to fix this.
										for (int i = 0; i < recordedLength4P1; i++)
										{
											memcpy(&replayBuffer7[i], &pChars[(i * 550)], ReplayBufferSize);
										}
									}


								}

							}


						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Load Recording in P2"))
				{
					if (CheckTheMode() == true)
					{
						if (!recordingP2 && !replayingP2)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };
							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetOpenFileName(&ofn))
							{
								if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
								{
									MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

								}
								else
								{
									//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
									std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
									mRP.unsetf(std::ios::skipws);
									std::ifstream::pos_type pos = mRP.tellg();
									int filesize = pos;

									if (filesize % 550 != 0)
									{
										MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
									}
									else
									{
										//Gets the file size, sets the needed variables to play the replay when loaded, 
										//and then copies the file into an array in memory.
										int length = pos / 550;
										replayAvailable4P2 = true;
										recordedLength4P2 = length;
										recordReplayIndex4P2 = 0;

										mRP.seekg(0, std::ios::beg);

										std::vector<BYTE> pChars;
										pChars.reserve(filesize);

										pChars.insert(pChars.begin(),
											std::istream_iterator<BYTE>(mRP),
											std::istream_iterator<BYTE>());

										//For loop for inserting the frames in the proper place. Based on code from recording.
										for (int i = 0; i < recordedLength4P2; i++)
										{
											memcpy(&replayBuffer8[i], &pChars[(i * 550)], ReplayBufferSize);
										}
									}


								}

							}


						}
					}
				}


				break;

			case 5:

				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record Both")) {
						if (CheckTheMode() == true)
						{
							//recording = true;
							//replaying = false;
							recordingP1 = true;
							replayingP1 = false;
							recordingP2 = true;
							replayingP2 = false;
							recordReplayIndex5P1 = 0;
							recordReplayIndex5P2 = 0;

						}
					}
				}
				//ImGui::SameLine();
				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record P1")) {
						if (CheckTheMode() == true)
						{
							recordingP1 = true;
							replayingP1 = false;
							//recording = false;
							//replaying = false;
							recordingP2 = false;
							replayingP2 = false;
							recordReplayIndex5P1 = 0;
						}
					}
					ImGui::SameLine();
				}
				//ImGui::SameLine();
				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record P2")) {
						if (CheckTheMode() == true)
						{
							recordingP2 = true;
							replayingP2 = false;
							//recording = false;
							//replaying = false;
							recordingP1 = false;
							replayingP1 = false;
							recordReplayIndex5P2 = 0;
						}
					}
				}

				else if (recordReplayIndex5P1 > 90 || recordReplayIndex5P2 > 90) {
					if (ImGui::Button("Stop Recording")) {
						if (CheckTheMode() == true)
						{
							//recording = false;
							recordingP1 = false;
							recordingP2 = false;
							recordedLength5P1 = recordReplayIndex5P1 - 1;
							recordedLength5P2 = recordReplayIndex5P2 - 1;
						}
					}
				}

				//Playback Code Below.
				ImGui::Separator();

				if (replayAvailable5P1 && replayAvailable5P2 && !recordingP1 && !recordingP2) {
					if (!replayingP1 && !replayingP2) {
						if (ImGui::Button("Playback Both")) {
							if (CheckTheMode() == true)
							{

								replayingP1 = true;
								replayingP2 = true;
								recordReplayIndex5 = 0;
								recordReplayIndex5P1 = 0;
								recordReplayIndex5P2 = 0;

							}

						}
					}
					else if (recordReplayIndex5P1 > 90 || recordReplayIndex5P2 > 90) {
						if (ImGui::Button("Stop Playback"))
						{
							if (CheckTheMode() == true)
							{
								replayingP1 = false;
								replayingP2 = false;

							}
						}
					}
				}

				if (replayAvailable5P1 && !recordingP1) {
					if (!replayingP1) {
						if (ImGui::Button("Playback P1")) {
							if (CheckTheMode() == true)
							{
								replayingP1 = true;
								recordReplayIndex5P1 = 0;
							}

						}
					}
					else if (recordReplayIndex5P1 > 90) {
						if (ImGui::Button("Stop Playback P1"))
						{
							if (CheckTheMode() == true)
							{
								replayingP1 = false;
							}
						}
					}
					ImGui::SameLine();
				}

				if (replayAvailable5P2 && !recordingP2) {
					if (!replayingP2) {
						if (ImGui::Button("Playback P2")) {
							if (CheckTheMode() == true)
							{
								replayingP2 = true;
								recordReplayIndex5P2 = 0;
							}

						}
					}
					else if (recordReplayIndex5P2 > 90) {
						if (ImGui::Button("Stop Playback P2"))
						{
							if (CheckTheMode() == true)
							{
								replayingP2 = false;
							}
						}
					}
				}


				ImGui::Separator();

				//Saves Recordings. Needs to be adjusted.
				if (ImGui::Button("Save P1 Recording"))
				{
					if (CheckTheMode() == true)
					{
						if (!recording && replayAvailable5P1)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };

							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
							if (GetSaveFileName(&ofn))
							{
								pRec = fopen(ofn.lpstrFile, "wb");
								fwrite(replayBuffer9, 550, recordedLength5P1, pRec);
								fclose(pRec);
							}
						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Save P2 Recording"))
				{
					if (CheckTheMode() == true)
					{
						if (!recording && replayAvailable5P2)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };

							char szFile[_MAX_PATH] = "RecP2";
							const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetSaveFileName(&ofn))
							{
								pRec = fopen(ofn.lpstrFile, "wb");
								fwrite(replayBuffer10, 550, recordedLength5P2, pRec);
								fclose(pRec);

							}
						}
					}
				}

				if (ImGui::Button("Load Recording in P1"))
				{
					if (CheckTheMode() == true)
					{
						if (!recordingP1 && !replayingP1)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };
							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetOpenFileName(&ofn))
							{
								if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
								{
									MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

								}
								else
								{
									//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
									std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
									mRP.unsetf(std::ios::skipws);
									std::ifstream::pos_type pos = mRP.tellg();
									int filesize = pos;

									if (filesize % 550 != 0)
									{
										MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
									}
									else
									{
										//Gets the file size, sets the needed variables to play the replay when loaded, 
										//and then copies the file into an array in memory.
										int length = pos / 550;
										replayAvailable5P1 = true;
										recordedLength5P1 = length;
										recordReplayIndex5P1 = 0;

										mRP.seekg(0, std::ios::beg);

										std::vector<BYTE> pChars;
										pChars.reserve(filesize);

										pChars.insert(pChars.begin(),
											std::istream_iterator<BYTE>(mRP),
											std::istream_iterator<BYTE>());

										//For loop for inserting the frames in the proper place. Based on code from recording.
										//Need to fix this.
										for (int i = 0; i < recordedLength5P1; i++)
										{
											memcpy(&replayBuffer9[i], &pChars[(i * 550)], ReplayBufferSize);
										}
									}


								}

							}


						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Load Recording in P2"))
				{
					if (CheckTheMode() == true)
					{
						if (!recordingP2 && !replayingP2)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };
							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetOpenFileName(&ofn))
							{
								if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
								{
									MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

								}
								else
								{
									//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
									std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
									mRP.unsetf(std::ios::skipws);
									std::ifstream::pos_type pos = mRP.tellg();
									int filesize = pos;

									if (filesize % 550 != 0)
									{
										MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
									}
									else
									{
										//Gets the file size, sets the needed variables to play the replay when loaded, 
										//and then copies the file into an array in memory.
										int length = pos / 550;
										replayAvailable5P2 = true;
										recordedLength5P2 = length;
										recordReplayIndex5P2 = 0;

										mRP.seekg(0, std::ios::beg);

										std::vector<BYTE> pChars;
										pChars.reserve(filesize);

										pChars.insert(pChars.begin(),
											std::istream_iterator<BYTE>(mRP),
											std::istream_iterator<BYTE>());

										//For loop for inserting the frames in the proper place. Based on code from recording.
										for (int i = 0; i < recordedLength5P2; i++)
										{
											memcpy(&replayBuffer10[i], &pChars[(i * 550)], ReplayBufferSize);
										}
									}


								}

							}


						}
					}
				}

				break;

			case 1:
			default:

				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record Both")) {
						if (CheckTheMode() == true)
						{
							//recording = true;
							//replaying = false;
							recordingP1 = true;
							replayingP1 = false;
							recordingP2 = true;
							replayingP2 = false;
							//recordReplayIndex = 0;
							recordReplayIndexP1 = 0;
							recordReplayIndexP2 = 0;

						}
					}
				}
				//ImGui::SameLine();
				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record P1")) {
						if (CheckTheMode() == true)
						{
							recordingP1 = true;
							replayingP1 = false;
							//recording = false;
							//replaying = false;
							recordingP2 = false;
							replayingP2 = false;
							recordReplayIndexP1 = 0;
						}
					}
					ImGui::SameLine();
				}
				//ImGui::SameLine();
				if (!recordingP1 && !recordingP2) {
					if (ImGui::Button("Record P2")) {
						if (CheckTheMode() == true)
						{
							recordingP2 = true;
							replayingP2 = false;
							//recording = false;
							//replaying = false;
							recordingP1 = false;
							replayingP1 = false;
							recordReplayIndexP2 = 0;
						}
					}
				}

				else if (recordReplayIndexP1 > 90 || recordReplayIndexP2 > 90) {
					if (ImGui::Button("Stop Recording")) {
						if (CheckTheMode() == true)
						{
							//recording = false;
							recordingP1 = false;
							recordingP2 = false;
							recordedLengthP1 = recordReplayIndexP1 - 1;
							recordedLengthP2 = recordReplayIndexP2 - 1;
						}
					}
				}

				//Playback Code Below.
				ImGui::Separator();

				if (replayAvailableP1 && replayAvailableP2 && !recordingP1 && !recordingP2) {
					if (!replayingP1 && !replayingP2) {
						if (ImGui::Button("Playback Both")) {
							if (CheckTheMode() == true)
							{

								replayingP1 = true;
								replayingP2 = true;
								recordReplayIndex = 0;
								recordReplayIndexP1 = 0;
								recordReplayIndexP2 = 0;

							}

						}
					}
					else if (recordReplayIndexP1 > 90 || recordReplayIndexP2 > 90) {
						if (ImGui::Button("Stop Playback"))
						{
							if (CheckTheMode() == true)
							{
								replayingP1 = false;
								replayingP2 = false;

							}
						}
					}
				}

				if (replayAvailableP1 && !recordingP1) {
					if (!replayingP1) {
						if (ImGui::Button("Playback P1")) {
							if (CheckTheMode() == true)
							{
								replayingP1 = true;
								recordReplayIndexP1 = 0;
							}

						}
					}
					else if (recordReplayIndexP1 > 90) {
						if (ImGui::Button("Stop Playback P1"))
						{
							if (CheckTheMode() == true)
							{
								replayingP1 = false;
							}
						}
					}
					ImGui::SameLine();
				}

				if (replayAvailableP2 && !recordingP2) {
					if (!replayingP2) {
						if (ImGui::Button("Playback P2")) {
							if (CheckTheMode() == true)
							{
								replayingP2 = true;
								recordReplayIndexP2 = 0;
							}

						}
					}
					else if (recordReplayIndexP2 > 90) {
						if (ImGui::Button("Stop Playback P2"))
						{
							if (CheckTheMode() == true)
							{
								replayingP2 = false;
							}
						}
					}
				}


				ImGui::Separator();

				//Saves Recordings. Needs to be adjusted.
				if (ImGui::Button("Save P1 Recording"))
				{
					if (CheckTheMode() == true)
					{
						if (!recording && replayAvailableP1)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };

							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
							if (GetSaveFileName(&ofn))
							{
								pRec = fopen(ofn.lpstrFile, "wb");
								fwrite(replayBuffer, 550, recordedLengthP1, pRec);
								fclose(pRec);
							}
						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Save P2 Recording"))
				{
					if (CheckTheMode() == true)
					{
						if (!recording && replayAvailableP2)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };

							char szFile[_MAX_PATH] = "RecP2";
							const char szExt[] = "dat\0"; // extra '\0' for lpstrFilter

							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetSaveFileName(&ofn))
							{
								pRec = fopen(ofn.lpstrFile, "wb");
								fwrite(replayBuffer2, 550, recordedLengthP2, pRec);
								fclose(pRec);

							}
						}
					}
				}

				//Loads Recordings. To Be Continued....
				if (ImGui::Button("Load Recording in P1"))
				{
					if (CheckTheMode() == true)
					{
						if (!recordingP1 && !replayingP1)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };
							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetOpenFileName(&ofn))
							{
								if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
								{
									MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

								}
								else
								{
									//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
									std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
									mRP.unsetf(std::ios::skipws);
									std::ifstream::pos_type pos = mRP.tellg();
									int filesize = pos;

									if (filesize %  550 != 0)
									{
										MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
									}
									else 
									{
										//Gets the file size, sets the needed variables to play the replay when loaded, 
										//and then copies the file into an array in memory.
										int length = pos / 550;
										replayAvailableP1 = true;
										recordedLengthP1 = length;
										recordReplayIndexP1 = 0;

										mRP.seekg(0, std::ios::beg);

										std::vector<BYTE> pChars;
										pChars.reserve(filesize);

										pChars.insert(pChars.begin(),
											std::istream_iterator<BYTE>(mRP),
											std::istream_iterator<BYTE>());

										//For loop for inserting the frames in the proper place. Based on code from recording.
										//Need to fix this.
										for (int i = 0; i < recordedLengthP1; i++)
										{

											memcpy(&replayBuffer[i], &pChars[(i * 550)], ReplayBufferSize);
										}
									}


								}

							}


						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Load Recording in P2"))
				{
					if (CheckTheMode() == true)
					{
						if (!recordingP2 && !replayingP2)
						{
							OPENFILENAME ofn = { sizeof(OPENFILENAME) };
							char szFile[_MAX_PATH] = "RecP1";
							const char szExt[] = ".dat\0"; // extra '\0' for lpstrFilter
							ofn.hwndOwner = GetConsoleWindow();
							ofn.lpstrFile = szFile; // <--------------------- initial file name
							ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
							ofn.lpstrFilter = ofn.lpstrDefExt = szExt;
							ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

							if (GetOpenFileName(&ofn))
							{
								if (ofn.lStructSize > (ReplayBufferSize * ReplayLength))
								{
									MessageBoxA(0, "This file is too big to load.\nThe opened file size is: " + ofn.lStructSize, 0, MB_ICONINFORMATION);

								}
								else
								{
									//MessageBoxA(0,ofn.lpstrFile,0,MB_ICONINFORMATION);
									std::ifstream mRP(ofn.lpstrFile, std::ios::binary | std::ios::ate);
									mRP.unsetf(std::ios::skipws);
									std::ifstream::pos_type pos = mRP.tellg();
									int filesize = pos;

									if (filesize % 550 != 0)
									{
										MessageBoxA(0, "This file is malformed.\nRecording files are assumed to be multiples of 550\nand yet the filesize is : " + ofn.lStructSize, 0, MB_ICONINFORMATION);
									}
									else
									{
										//Gets the file size, sets the needed variables to play the replay when loaded, 
										//and then copies the file into an array in memory.
										int length = pos / 550;
										replayAvailableP2 = true;
										recordedLengthP2 = length;
										recordReplayIndexP2 = 0;

										mRP.seekg(0, std::ios::beg);

										std::vector<BYTE> pChars;
										pChars.reserve(filesize);

										pChars.insert(pChars.begin(),
											std::istream_iterator<BYTE>(mRP),
											std::istream_iterator<BYTE>());

										//For loop for inserting the frames in the proper place. Based on code from recording.
										for (int i = 0; i < recordedLengthP2; i++)
										{

											memcpy(&replayBuffer2[i], &pChars[(i * 550)], ReplayBufferSize);
										}
									}


								}

							}


						}
					}
				}

				break;
			}

			if (RecordingSlot)
			{
				ImGui::Text("Recording Slot");
				if (ImGui::SliderInt("Slot", &RecordingSlot, 1, 5))
				{
					if (CheckTheMode() == true)
					{

					}
				}
			}

			auto sigger = sigscan::get();

			ImGui::Separator();

			ImGui::Text("V0.3 By Eternal Yoshi, thanks to HkHaan96 for help\nand Ermaccer for the original hook");

			ImGui::EndTabItem();

		}

		ImGui::EndTabBar();
	}
	ImGui::End();
	//ImGui::EndChild();

	//Original Code Here.
	/*
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Settings"))
		{
			m_bSubmenuActive[SM_Settings] = true;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::BeginMenu("About"))
			{
				ImGui::MenuItem("Version: " UMVC3HOOK_VERSION);
				ImGui::MenuItem("Date: " __DATE__);
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

	}
	ImGui::EndMenuBar();

	if (ImGui::BeginTabBar("##tabs"))
	{
		if (ImGui::BeginTabItem("Camera"))
		{
			DrawCameraTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Misc."))
		{
			DrawMiscTab();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
	*/

	if (m_bSubmenuActive[SM_Settings])
		DrawSettings();

}

void UMVC3Menu::Process()
{

	Memory::VP::Patch<char>(_addr(0x140532D80), m_bFreeCam ? 0xC3 : 0x48);

	Memory::VP::Patch<char>(_addr(0x140326A70), (m_bDisableHUD || m_bDisableHUDTotal) ? 0xC3 : 0x48);
	Memory::VP::Patch<char>(_addr(0x140324240), (m_bDisableHUD || m_bDisableHUDTotal) ? 0xC3 : 0x48);
	Memory::VP::Patch<char>(_addr(0x1404537B0), m_bDisableHUDTotal ? 0xC3 : 0x48);

	Memory::VP::Patch<char>(_addr(0x14077EA40), m_bDisableEffects ? 0xC3 : 0x48);
	Memory::VP::Patch<char>(_addr(0x14077D330), m_bDisableEffects ? 0xC3 : 0x48);

	UpdateControls();
	UpdateFreecam();
}

void UMVC3Menu::UpdateControls()
{

}

void UMVC3Menu::UpdateFreecam()
{

	UMVC3MainCamera* cam = GetMainCamera();

	if (!cam)
		return;

	Vector fwd = cam->mat.forward;
	Vector up = cam->mat.up;
	Vector right = cam->mat.right;

	static XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	static XMVECTOR defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	static XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	float delta = GUIImplementationDX9::GetDeltaTime();

	if (m_bFreeCam)
	{
		// forward

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyXPlus))
			TheMenu->camMat.pos.X += TheMenu->m_fFreeCameraSpeed * delta * 1;


		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyXMinus))
			TheMenu->camMat.pos.X += TheMenu->m_fFreeCameraSpeed * delta * -1;

		// right

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYPlus))
			TheMenu->camMat.pos.Y += TheMenu->m_fFreeCameraSpeed * delta * 1;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYMinus))
			TheMenu->camMat.pos.Y += TheMenu->m_fFreeCameraSpeed * delta * -1;

		// up

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyZPlus))
			TheMenu->camMat.pos.Z += TheMenu->m_fFreeCameraSpeed * delta * 1;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyZMinus))
			TheMenu->camMat.pos.Z += TheMenu->m_fFreeCameraSpeed * delta * -1;


		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyPitchPlus))
			camRot.X += TheMenu->m_fFreeCameraRotationSpeed * delta;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyPitchMinus))
			camRot.X -= TheMenu->m_fFreeCameraRotationSpeed * delta;

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYawPlus))
			camRot.Y += TheMenu->m_fFreeCameraRotationSpeed * delta;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYawMinus))
			camRot.Y -= TheMenu->m_fFreeCameraRotationSpeed * delta;

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRollPlus))
			camRot.Z += TheMenu->m_fFreeCameraRotationSpeed * delta;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRollMinus))
			camRot.Z -= TheMenu->m_fFreeCameraRotationSpeed * delta;
	}

	if (m_bFreeCam || m_bCustomCameraFOV)
	{
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyFOVMinus))
			camFov -= 2.5f * delta;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyFOVPlus))
			camFov += 2.5f * delta;
	}


	if (m_bFreeCam)
	{
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(camRot.X, camRot.Y, camRot.Z);

		XMVECTOR camLookAt = XMVector3TransformCoord(defaultForward, rotMat);
		XMVECTOR camUp = XMVector3TransformCoord(defaultUp, rotMat);
		XMVECTOR camRight = XMVector3TransformCoord(defaultRight, rotMat);
		XMVector3Normalize(camLookAt);
		XMVector3Normalize(camUp);
		XMVector3Normalize(camRight);
		XMFLOAT3 cL, cU, cR;
		XMStoreFloat3(&cL, camLookAt);
		XMStoreFloat3(&cU, camUp);
		XMStoreFloat3(&cR, camRight);

		camMat.forward = { cL.x, cL.y, cL.z };
		camMat.up = { cU.x, cU.y, cU.z };
		camMat.right = { cR.x, cR.y, cR.z };
	}
	if (m_bFreeCam)
	{
		cam->mat.pos = cam->mat2.pos = camMat.pos;
		cam->mat.right = cam->mat2.right = camMat.right;
		cam->mat.forward = cam->mat2.forward = camMat.forward;
		cam->mat.up = cam->mat2.up = camMat.up;

	}
	else
	{
		camMat.pos = cam->mat.pos;
	}

	if (TheCamera)
	{
		if (m_bCustomCameraFOV)
			TheCamera->mFOV = camFov;
		else
			camFov = TheCamera->mFOV;
	}
}

void UMVC3Menu::DrawCameraTab()
{
	UMVC3MainCamera* cam = GetMainCamera();

	if (!cam)
		return;
#ifdef _DEBUG
	ImGui::Text("%p", cam);


	ImGui::InputFloat4("Pr", &cam->mat.M[0][0]);
	ImGui::InputFloat4("Pu", &cam->mat.M[1][0]);
	ImGui::InputFloat4("Pf", &cam->mat.M[2][0]);
	ImGui::InputFloat4("Pp", &cam->mat.M[3][0]);
	ImGui::Separator();
	ImGui::Separator();
	ImGui::InputFloat4("Sr", &cam->mat2.M[0][0]);
	ImGui::InputFloat4("Su", &cam->mat2.M[1][0]);
	ImGui::InputFloat4("Sf", &cam->mat2.M[2][0]);
	ImGui::InputFloat4("Sp", &cam->mat2.M[3][0]);


	ImGui::Separator();
#endif

	ImGui::InputFloat3("X | Y | Z", &camMat.pos.X);
	ImGui::InputFloat3("Pitch | Yaw | Roll", &camRot.X);


	ImGui::Separator();
	ImGui::Checkbox("Set FOV", &m_bCustomCameraFOV);
	ImGui::SameLine();
	ShowHelpMarker("FOV won't update while free camera is active!");
	ImGui::InputFloat("FOV", &camFov);


	ImGui::Separator();
	ImGui::Checkbox("Enable Free Camera", &m_bFreeCam);
	ImGui::SameLine(); ShowHelpMarker("Allows to move camera with keyboard. Keys can be changed in the Settings menu or in the .ini file. Hotkey can be assigned in Settings menu.");

	if (m_bFreeCam)
	{
		ImGui::InputFloat("Free Camera Speed", &m_fFreeCameraSpeed);
		ImGui::InputFloat("Free Camera Rotation Speed", &m_fFreeCameraRotationSpeed);

	}

}

void UMVC3Menu::DrawMiscTab()
{
	ImGui::Checkbox("Disable Game HUD", &m_bDisableHUD);
	ImGui::SameLine();
	ShowHelpMarker(eKeyboardMan::KeyToString(SettingsMgr->iToggleHUDKey));
	ImGui::Checkbox("Disable All HUD", &m_bDisableHUDTotal);
	ImGui::Checkbox("Disable Effects", &m_bDisableEffects);
}

void UMVC3Menu::DrawSettings()
{
	ImGui::SetNextWindowPos({ ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f }, ImGuiCond_Once, { 0.5f, 0.5f });
	ImGui::SetNextWindowSize({ 35 * ImGui::GetFontSize(), 35 * ImGui::GetFontSize() }, ImGuiCond_Once);
	ImGui::Begin("Settings", &m_bSubmenuActive[SM_Settings]);

	static int settingID = 0;
	static const char* settingNames[] = {
		"Menu",
		"INI",
		"Keys",
	};

	enum eSettings {
		MENU,
		INI,
		KEYS,
	};

	ImGui::BeginChild("##settings", { 12 * ImGui::GetFontSize(), 0 }, true);

	for (int n = 0; n < IM_ARRAYSIZE(settingNames); n++)
	{
		bool is_selected = (settingID == n);
		if (ImGui::Selectable(settingNames[n], is_selected))
			settingID = n;
		if (is_selected)
			ImGui::SetItemDefaultFocus();
	}

	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild("##content", { 0, -ImGui::GetFrameHeightWithSpacing() });

	switch (settingID)
	{
	case MENU:
		ImGui::TextWrapped("All user settings are saved to umvc3hook_user.ini.");
		ImGui::Text("Menu Scale");
		ImGui::PushItemWidth(-FLT_MIN);
		ImGui::InputFloat("##", &SettingsMgr->fMenuScale);
		ImGui::PopItemWidth();
		break;
	case INI:
		ImGui::TextWrapped("These settings control UMVC3Hook.ini options. Any changes require game restart to take effect.");
		ImGui::Checkbox("Debug Console", &SettingsMgr->bEnableConsoleWindow);
		break;
	case KEYS:
		if (m_bPressingKey)
			ImGui::TextColored(ImVec4(0.f, 1.f, 0.3f, 1.f), "Press a key!");

		if (ImGui::Button("Reset Keys", { -FLT_MIN, 0 }))
		{
			SettingsMgr->ResetKeys();
			Notifications->SetNotificationTime(5500);
			Notifications->PushNotification("Keys reset! Remember to save.");

		}
		ImGui::Separator();
		ImGui::LabelText("##", "Core");
		ImGui::Separator();
		KeyBind(&SettingsMgr->iHookMenuOpenKey, "Open/Close Menu", "menu");
		ImGui::Separator();
		ImGui::LabelText("##", "Camera");
		ImGui::Separator();

		KeyBind(&SettingsMgr->iFreeCameraKeyFOVPlus, "FOV+", "fov_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyFOVMinus, "FOV-", "fov_minus");

		KeyBind(&SettingsMgr->iFreeCameraKeyYawPlus, "Yaw+", "ya_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyYawMinus, "Yaw-", "ya_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyPitchPlus, "Pitch+", "pi_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyPitchMinus, "Pitch-", "pi_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRollPlus, "Roll+", "r_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRollMinus, "Roll-", "r_minus");

		KeyBind(&SettingsMgr->iFreeCameraKeyXPlus, "X+", "x_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyXMinus, "X-", "x_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyYPlus, "Y+", "y_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyYMinus, "Y-", "y_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyZPlus, "Z+", "z_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyZMinus, "Z-", "z_minus");


		ImGui::Separator();
		ImGui::LabelText("##", "Misc");
		ImGui::Separator();
		KeyBind(&SettingsMgr->iToggleFreeCameraKey, "Toggle Free Camera", "fcam");
		KeyBind(&SettingsMgr->iToggleHUDKey, "Toggle HUD", "thud");
		ImGui::Separator();

		if (m_bPressingKey)
		{
			eVKKeyCode result = eKeyboardMan::GetLastKey();

			if (result >= VK_BACKSPACE && result < VK_KEY_NONE)
			{
				*m_pCurrentVarToChange = result;
				m_bPressingKey = false;
			}

		}
		break;
	default:
		break;
	}

	if (ImGui::Button("Save", { -FLT_MIN, 0 }))
	{
		Notifications->SetNotificationTime(5500);
		Notifications->PushNotification("Settings saved to UMVC3Hook.ini and umvc3hook_user.ini!");
		GUIImplementationDX9::RequestFontReload();
		SettingsMgr->SaveSettings();
	}

	ImGui::EndChild();

	ImGui::End();
}

void UMVC3Menu::DrawKeyBind(char* name, int* var)
{
	ImGui::SameLine();

	static char butName[256] = {};
	sprintf(butName, "%s##key%s", eKeyboardMan::KeyToString(*var), name);
	if (ImGui::Button(butName))
	{
		m_bPressingKey = true;
		m_pCurrentVarToChange = var;
	}
}

void UMVC3Menu::KeyBind(int* var, char* bindName, char* name)
{
	ImGui::LabelText("##", bindName);
	DrawKeyBind(name, var);
}



//TODO put this in a better spot.

#include <d3d9types.h>
bool UMVC3Menu::startedHitbox() {
	return startedHitboxViewer;
}
int UMVC3Menu::getFighterIndex(uintptr_t ptr) {
	for (size_t i = 0; i < 6; i++)
	{
		if ((uintptr_t)scriptableFighters[i].fighter == ptr) return i;
	}
	return 0;
}
int UMVC3Menu::getHBoxCount(int fighterIndex) {
	if (!pAddrSet || scriptableFighters[fighterIndex].fighter == NULL)
		return 0;
	auto t = *((uintptr_t*)(((uint8_t*)scriptableFighters[fighterIndex].fighter) + 0x4E10));
	auto tt = (uintptr_t*)(*(uintptr_t*)(((uint8_t*)t) + 0x30));
	for (size_t i = 0; i < 100; i++)
	{
		if (*tt == 0) {
			return i;
		}
		tt++;
	}
	return 26;
}
HBoxInfo UMVC3Menu::getHBoxPos(int index, int fighterIndex)
{
	if (!pAddrSet || scriptableFighters[fighterIndex].fighter == NULL)
		return HBoxInfo(0, 0, 0.0f, D3DCOLOR_RGBA(0, 250, 0, 100), 60, 0);
	auto t = *((uintptr_t*)(((uint8_t*)scriptableFighters[fighterIndex].fighter) + 0x4E10));
	auto tt = (uint8_t*)*(uintptr_t*)(((uint8_t*)t) + 0x30);
	tt += index * 8;
	tt = (uint8_t*)(*(uintptr_t*)tt);
	auto valx = tt + 0x30;
	auto posx = *((float*)valx);
	auto valy = tt + 0x34;
	auto posy = *((float*)valy);
	auto r = screenPosition(glm::vec3(posx, posy, 70.0f));
	return HBoxInfo(r.x, r.y, 0.0f, D3DCOLOR_RGBA(0, 250, 0, 70, 60, 0), 30, 1);
}

int UMVC3Menu::getHitboxCount() {
	return hitboxes.size();
	return NULL;
}
HBoxInfo UMVC3Menu::getHitBoxPos(int index)
{
	auto valx = ((uint8_t*)hitboxes[index]) + 0x20;
	auto posx = *((float*)valx);
	auto valy = ((uint8_t*)hitboxes[index]) + 0x24;
	auto posy = *((float*)valy);
	auto valen = ((uint8_t*)hitboxes[index]) + 0x30 + 0x20;
	auto en = *((int*)valen);
	auto valsz = ((uint8_t*)hitboxes[index]) + 0x20 + 0x20;
	float size = *((float*)valsz);
	if(index==0)
		printf("%llx - %f\n",valx,posy);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

	auto r = screenPosition(glm::vec3(posx, posy, 70.0f));
	if (posx != 0) {
		printf("pttrtrtrt:%f - %llx %llx\n", posx, valx, valen);
	}
	return HBoxInfo(r.x, r.y, 0.0f, D3DCOLOR_RGBA(250, 0, 0, 100), size, en);
}



int64 cameraptr = 0;
float posXMultiplier = 1.15;
float posYMultiplier = 1.37;
#include "../glm/vec4.hpp"
#include "../glm/vec3.hpp"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
float UMVC3Menu::getXMultiplier() { return posXMultiplier; }
float* UMVC3Menu::getXMultiplierPtr() { return &posXMultiplier; }
void UMVC3Menu::setXMultiplier(float val) { posXMultiplier = val; }
float UMVC3Menu::getYMultiplier() { return posYMultiplier; }
float* UMVC3Menu::getYMultiplierPtr() { return &posYMultiplier; }
void UMVC3Menu::setYMultiplier(float val) { posYMultiplier = val; }
void UMVC3Menu::setCameraPtr(uintptr_t camptr) {
	cameraptr = camptr;
}
glm::highp_mat4 getProjection() {

	auto res = glm::perspective<float>(*(float*)(cameraptr + 0x4C), 1600.0f / 900.0f, 0.01f, 100.0f);
	return res;
}
glm::vec4 getView() {
	return glm::vec4(0, 0, 1600, 900 * UMVC3Menu::getYMultiplier());
}
glm::vec2 UMVC3Menu::screenPosition(glm::vec3& _coord) {
	float posx = *(float*)(cameraptr + 0x50);
	float posy = *(float*)(cameraptr + 0x54);
	float posz = *(float*)(cameraptr + 0x58);
	float posx2 = *(float*)(cameraptr + 0x70);
	float posy2 = *(float*)(cameraptr + 0x74);
	float posz2 = *(float*)(cameraptr + 0x78);

	_coord.z = posz;
	int multi = 0.5f;
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(posx, posy, posz2), glm::vec3(posx2, posy2, posz), glm::vec3(0, 1, 0));
	return glm::project(_coord, viewMatrix, getProjection(), getView());
}
