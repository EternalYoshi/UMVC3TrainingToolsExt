#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

inline bool ValidVer;
inline int GameMode;
inline uintptr_t StrangeTable;
inline uintptr_t mysterytable;
inline uintptr_t block2;
inline uintptr_t ptable;
inline DWORD errorMessageID;
inline int P1Character1ID;
inline int P1Character2ID;
inline int P1Character3ID;
inline int P2Character1ID;
inline int P2Character2ID;
inline int P2Character3ID;
inline uintptr_t P1Character1Data;
inline uintptr_t P1Character2Data;
inline uintptr_t P1Character3Data;
inline uintptr_t P2Character1Data;
inline uintptr_t P2Character2Data;
inline uintptr_t P2Character3Data;
inline uintptr_t team1ptr;
inline uintptr_t team2ptr;
inline int TempA;
inline int TempB;
inline unsigned char val;

//inline static int64 timer = GetTickCount64();

inline float p1Pos = -125.0f;
inline float p2Pos = 125.0f;
inline bool restarted = false;
inline int restartTimer = 0;

inline int MODOKLOU = 1;
inline int FrankLevel = 1;
inline float PrestigePoints = 0.0;
inline int WrightEvidenceA = -1;
inline int WrightEvidenceB = -1;
inline int WrightEvidenceC = -1;
inline int SaveStateSlot = 0;
inline int DormRed = 0;
inline int DormBlue = 0;
inline bool DormSpellSet = false;
inline bool EndlessInstalls = false;
inline int DeadpoolTeleportCount = 0;
inline bool FreezeDeadpoolTPCounter = false;
inline bool P1Character1Dead = false;
inline bool P1Character2Dead = false;
inline bool P1Character3Dead = false;
inline bool P2Character1Dead = false;
inline bool P2Character2Dead = false;
inline bool P2Character3Dead = false;
inline bool EndlessXFactor = false;
inline bool EndlessXFactorP1 = false;
inline bool EndlessXFactorP2 = false;
inline bool SpecialFeature = false;
inline bool HitboxDisplay = false;
inline bool SetIndividualHP = false;
inline bool AlsoSetRedHealth = false;
inline float P1Char1Health = 1.0;
inline float P1Char1RedHealth = 1.0;
inline float P1Char2Health = 1.0;
inline float P1Char2RedHealth = 1.0;
inline float P1Char3Health = 1.0;
inline float P1Char3RedHealth = 1.0;
inline float P2Char1Health = 1.0;
inline float P2Char1RedHealth = 1.0;
inline float P2Char2Health = 1.0;
inline float P2Char2RedHealth = 1.0;
inline float P2Char3Health = 1.0;
inline float P2Char3RedHealth = 1.0;
inline float P1Char1HealthNew = 1.0;
inline float P1Char2HealthNew = 1.0;
inline float P1Char3HealthNew = 1.0;
inline float P2Char1HealthNew = 1.0;
inline float P2Char2HealthNew = 1.0;
inline float P2Char3HealthNew = 1.0;
inline bool P1Char1Slow = false;
inline bool P1Char2Slow = false;
inline bool P1Char3Slow = false;
inline bool P2Char1Slow = false;
inline bool P2Char2Slow = false;
inline bool P2Char3Slow = false;
inline bool DarkPhoenix = false;
inline bool Turnabout = false;
inline bool LockMODOKLOU = false;
inline int P1C1MaxHP = 0;
inline int P1C2MaxHP = 0;
inline int P1C3MaxHP = 0;
inline int P2C1MaxHP = 0;
inline int P2C2MaxHP = 0;
inline int P2C3MaxHP = 0;


inline const char* EvidenceTypes[] = { "Nothing","Photo","Watch", "Vase","Folder","Phone","Knife","Sunglasses","Beer","Bonsai", "Doll","Figurine","Plunger","Chicken" };
inline const char* EvidenceTypesTwo[] = { "Nothing","Photo","Watch", "Vase","Folder","Phone","Knife","Sunglasses","Beer","Bonsai", "Doll","Figurine","Plunger","Chicken" };
inline const char* EvidenceTypesThree[] = { "Nothing","Photo","Watch", "Vase","Folder","Phone","Knife","Sunglasses","Beer","Bonsai", "Doll","Figurine","Plunger","Chicken" };
inline static const char* selected_item = "Nothing";
inline static const char* selected_itemTwo = "Nothing";
inline static const char* selected_itemThree = "Nothing";

bool CheckGame();

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

inline FighterInstall EmptyInstall =
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

inline FighterInstall DarkPhoenixInstall =
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

inline FighterInstall TurnaboutMode =
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

DWORD SingleWrite(DWORD64 AddressToWrite, int length, HANDLE hProcess, BYTE ByteToWrite, DWORD errorMessageID, LPSTR messageBuffer);

DWORD MultiWrite(DWORD64 AddressToWrite, int length, HANDLE hProcess, BYTE ByteToWrite[], DWORD errorMessageID, LPSTR messageBuffer);

DWORD ProtectedSingleWrite(DWORD64 AddressToWrite, int length, HANDLE hProcess, BYTE ByteToWrite, DWORD errorMessageID, LPSTR messageBuffer, DWORD oldPageProtection);

DWORD ProtectedMultiWrite(DWORD64 AddressToWrite, int length, HANDLE hProcess, BYTE ByteToWrite[], DWORD errorMessageID, LPSTR messageBuffer, DWORD oldPageProtection);

bool CheckTheMode();

void GetMainPointers();

void GetCharacterIDs();

void GetPlayerData();

void EndlessXFactorUpdate();

void RestartWithChanges();

void TickUpdates();

void SetIndividualCharacterHealth();

void ChangeFrankLevel(int FrankLevel);

void ChangeWrightEvidence();

int EvidenceNameToID(const char* name);

void Objection();

void PopTheBird();

void ChangeMODOKUnderstanding(int MODOKLOU);

//Sets Dormammu Spell Charges.
void SetDormSpellLevels();

//Sets Infnite Install Time for Several Characters.
void EndlessInstallBoolUpdate();

void SetDeadpoolTeleport();