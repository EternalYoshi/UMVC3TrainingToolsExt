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
inline bool P1Char1Slow = false;
inline bool P1Char2Slow = false;
inline bool P1Char3Slow = false;
inline bool P2Char1Slow = false;
inline bool P2Char2Slow = false;
inline bool P2Char3Slow = false;
inline bool DarkPhoenix = false;
inline bool Turnabout = false;

inline const char* EvidenceTypes[] = { "Nothing","Photo","Watch", "Vase","Folder","Phone","Knife","Sunglasses","Beer","Bonsai", "Doll","Figurine","Plunger","Chicken" };
inline const char* EvidenceTypesTwo[] = { "Nothing","Photo","Watch", "Vase","Folder","Phone","Knife","Sunglasses","Beer","Bonsai", "Doll","Figurine","Plunger","Chicken" };
inline const char* EvidenceTypesThree[] = { "Nothing","Photo","Watch", "Vase","Folder","Phone","Knife","Sunglasses","Beer","Bonsai", "Doll","Figurine","Plunger","Chicken" };
inline static const char* selected_item = "Nothing";
inline static const char* selected_itemTwo = "Nothing";
inline static const char* selected_itemThree = "Nothing";

bool CheckGame();

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

void ChangeFrankLevel(int FrankLevel);