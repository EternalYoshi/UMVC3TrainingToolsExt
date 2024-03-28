//By Eternal Yoshi. Thanks to ermaccer for the new hook & HKHaan and Sheep for collecting the pointers and data needed to make this possible.
#include "../plugin/Menu.h"
#include "../plugin/Settings.h"
#include "..\sigscan.h"
#include "../gui/notifications.h"
#include "../gui/imgui/imgui.h"
#include "../umvc3/Camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <commdlg.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <random>
#include <ctime>
#include <chrono> 
#include <thread> 

#define undefined4 int
static int64 timer = GetTickCount64();

float p1Pos = -125.0f;
float p2Pos = 125.0f;
bool restarted = false;
int restartTimer = 0;

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
bool RNGIncludeSlot1 = false;
bool RNGIncludeSlot2 = false;
bool RNGIncludeSlot3 = false;
bool RNGIncludeSlot4 = false;
bool RNGIncludeSlot5 = false;
FILE* pRec;
int GameMode;
int RandomNum;
bool VCROn = false;
int GameModeInternal = 0;
bool LinkedWithTool = false;
inline uintptr_t TysonTableInternal;
inline uintptr_t Player1TeamTableInternal;
inline uintptr_t Player2TeamTableInternal;
inline uintptr_t StrangeTableInternal;
bool PLinked = &LinkedWithTool;

struct Recording {

	int FrameCount;
	char* InputBinary[3844];

};
//HANDLE rp = 0;
//bool* Linked = &LinkedWithTool;

//std::thread rp;
// 
// 
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

std::string CheckConnection()
{
	return "This is from the dll. If you can read this you got it.";
}

extern "C" __declspec(dllexport) int CheckConnectionTwo()
{
	LinkedWithTool = true;
	return 75;
}

extern "C" __declspec(dllexport) void DeployTheHooks()
{
	if (GameMode == 5)
	{
		Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
		InjectHook(_addr(0x140289c5a), tramp->Jump(FUN_1402b41b0), PATCH_CALL);
	}
}

extern "C" __declspec(dllexport) void StartTheTick()
{
	//std::thread tickThread(TickFunction);
	//tickThread.join();
}

void TheRecordButton()
{
	while (true)
	{
		//system("cls");
		printf("Currently Connected With Training Tool: %s\n", &LinkedWithTool ? "true" : "false");
		StrangeTableInternal = *(uintptr_t*)_addr(0x140d50e58);
		if (StrangeTableInternal != 0)
		{
			GameModeInternal = *((int*)(StrangeTableInternal + 0x34C));
			GameMode = GameModeInternal;
			if (GameModeInternal == 5)
			{
				TysonTableInternal = *(uintptr_t*)_addr(0x140D44A70);
				Player1TeamTableInternal = *((int*)(TysonTableInternal + 0x58));
				Player2TeamTableInternal = *((int*)(TysonTableInternal + 0x328));

				if (Player1TeamTableInternal != 0 && Player2TeamTableInternal != 0)
				{
					//Thread stuff later.
					Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
					InjectHook(_addr(0x140289c5a), tramp->Jump(FUN_1402b41b0), PATCH_CALL);
					printf("recordingP1: %s\n", recordingP1 ? "true" : "false");
					printf("recordingP2: %s\n", recordingP2 ? "true" : "false");
					printf("replayAvailableP1: %s\n", replayAvailableP1 ? "true" : "false");
					printf("replayAvailableP2: %s\n", replayAvailableP2 ? "true" : "false");
					printf("recordReplayIndexP1: %s\n", recordReplayIndexP1 ? "true" : "false");
					printf("recordReplayIndexP2: %s\n", recordReplayIndexP2 ? "true" : "false");
					printf("Recording Slot: %i", RecordingSlot);
					printf("\nCan Record.....\n");
				}
				else
				{
					printf("Not in a match yet.....\n");

				}


			}
			else
			{
				printf("Not Recording anything.....\n");

			}
		}


		//std::this_thread::sleep_for(std::chrono::milliseconds(15));
	}
}


extern "C" __declspec(dllexport) void RecordBoth(int RecordingSlot)
{

	switch (RecordingSlot)
	{


	case 5:
		recordingP1 = true;
		replayingP1 = false;
		recordingP2 = true;
		replayingP2 = false;
		recordReplayIndex5P1 = 0;
		recordReplayIndex5P2 = 0;
		break;

	case 4:
		recordingP1 = true;
		replayingP1 = false;
		recordingP2 = true;
		replayingP2 = false;
		recordReplayIndex4P1 = 0;
		recordReplayIndex4P2 = 0;
		break;

	case 3:
		recordingP1 = true;
		replayingP1 = false;
		recordingP2 = true;
		replayingP2 = false;
		recordReplayIndex3P1 = 0;
		recordReplayIndex3P2 = 0;
		break;

	case 2:
		recordingP1 = true;
		replayingP1 = false;
		recordingP2 = true;
		replayingP2 = false;
		recordReplayIndex2P1 = 0;
		recordReplayIndex2P2 = 0;
		break;


	case 1:
	default:
		recordingP1 = true;
		replayingP1 = false;
		recordingP2 = true;
		replayingP2 = false;
		recordReplayIndexP1 = 0;
		recordReplayIndexP2 = 0;
		break;
	}
	/*
	recordingP1 = true;
	replayingP1 = false;
	recordingP2 = true;
	replayingP2 = false;
	recordReplayIndexP1 = 0;
	recordReplayIndexP2 = 0;
	*/


}

extern "C" __declspec(dllexport) void RecordP1(int RecordingSlot)
{
	recordingP1 = true;
	replayingP1 = false;
	recordingP2 = false;
	replayingP2 = false;
	printf("%i", RecordingSlot);
	printf("Recording P1 Start!");

	//rp = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(TheRecordButton), 0, NULL, 0);

	//std::thread rp(&MessagesSender::updateMessages, &recordingP1);

	switch (RecordingSlot)
	{

	case 5:
		recordReplayIndex5P1 = 0;
		break;

	case 4:
		recordReplayIndex4P1 = 0;
		break;

	case 3:
		recordReplayIndex3P1 = 0;
		break;

	case 2:
		recordReplayIndex2P1 = 0;
		break;

	case 1:
	default:
		recordReplayIndexP1 = 0;
		break;

	}

	//recordReplayIndexP1 = 0;
}

extern "C" __declspec(dllexport) void RecordP2(int RecordingSlot)
{
	recordingP2 = true;
	replayingP2 = false;
	recordingP1 = false;
	replayingP1 = false;
	printf("%i", RecordingSlot);
	printf("Recording P2 Start!");
	switch (RecordingSlot)
	{

	case 5:
		recordReplayIndex5P2 = 0;
		break;

	case 4:
		recordReplayIndex4P2 = 0;
		break;

	case 3:
		recordReplayIndex3P2 = 0;
		break;

	case 2:
		recordReplayIndex2P2 = 0;
		break;

	case 1:
	default:
		recordReplayIndexP2 = 0;
		break;

	}
}

extern "C" __declspec(dllexport) void StopRecording(int RecordingSlot)
{
	recordingP1 = false;
	recordingP2 = false;

	//rp = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(TheRecordButton), 0, NULL, 0);


	switch (RecordingSlot)
	{

	case 5:
		recordedLength5P1 = recordReplayIndex5P1 - 1;
		recordedLength5P2 = recordReplayIndex5P2 - 1;
		break;

	case 4:
		recordedLength4P1 = recordReplayIndex4P1 - 1;
		recordedLength4P2 = recordReplayIndex4P2 - 1;
		break;

	case 3:
		recordedLength3P1 = recordReplayIndex3P1 - 1;
		recordedLength3P2 = recordReplayIndex3P2 - 1;
		break;

	case 2:
		recordedLength2P1 = recordReplayIndex2P1 - 1;
		recordedLength2P2 = recordReplayIndex2P2 - 1;
		break;

	case 1:
	default:
		recordedLengthP1 = recordReplayIndexP1 - 1;
		recordedLengthP2 = recordReplayIndexP2 - 1;
		break;

	}


}

extern "C" __declspec(dllexport) void SaveP1Recording(int RecordingSlot)
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

		switch (RecordingSlot)
		{

		case 5:
			if (GetSaveFileName(&ofn))
			{
				pRec = fopen(ofn.lpstrFile, "wb");
				fwrite(replayBuffer9, 550, recordedLengthP1, pRec);
				fclose(pRec);
			}
			break;

		case 4:
			if (GetSaveFileName(&ofn))
			{
				pRec = fopen(ofn.lpstrFile, "wb");
				fwrite(replayBuffer7, 550, recordedLengthP1, pRec);
				fclose(pRec);
			}
			break;

		case 3:
			if (GetSaveFileName(&ofn))
			{
				pRec = fopen(ofn.lpstrFile, "wb");
				fwrite(replayBuffer5, 550, recordedLengthP1, pRec);
				fclose(pRec);
			}
			break;

		case 2:
			if (GetSaveFileName(&ofn))
			{
				pRec = fopen(ofn.lpstrFile, "wb");
				fwrite(replayBuffer3, 550, recordedLengthP1, pRec);
				fclose(pRec);
			}
			break;

		case 1:
		default:
			if (GetSaveFileName(&ofn))
			{
				pRec = fopen(ofn.lpstrFile, "wb");
				fwrite(replayBuffer, 550, recordedLengthP1, pRec);
				fclose(pRec);
			}
			break;

		}

		/*
		if (GetSaveFileName(&ofn))
		{
			pRec = fopen(ofn.lpstrFile, "wb");
			fwrite(replayBuffer, 550, recordedLengthP1, pRec);
			fclose(pRec);
		}
		*/
	}

}

extern "C" __declspec(dllexport) void SaveP2Recording(int RecordingSlot)
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

		switch (RecordingSlot)
		{

		case 5:
			if (GetSaveFileName(&ofn))
			{
				pRec = fopen(ofn.lpstrFile, "wb");
				fwrite(replayBuffer10, 550, recordedLengthP1, pRec);
				fclose(pRec);
			}
			break;

		case 4:
			if (GetSaveFileName(&ofn))
			{
				pRec = fopen(ofn.lpstrFile, "wb");
				fwrite(replayBuffer8, 550, recordedLengthP1, pRec);
				fclose(pRec);
			}
			break;

		case 3:
			if (GetSaveFileName(&ofn))
			{
				pRec = fopen(ofn.lpstrFile, "wb");
				fwrite(replayBuffer6, 550, recordedLengthP1, pRec);
				fclose(pRec);
			}
			break;

		case 2:
			if (GetSaveFileName(&ofn))
			{
				pRec = fopen(ofn.lpstrFile, "wb");
				fwrite(replayBuffer4, 550, recordedLengthP1, pRec);
				fclose(pRec);
			}
			break;

		case 1:
		default:
			if (GetSaveFileName(&ofn))
			{
				pRec = fopen(ofn.lpstrFile, "wb");
				fwrite(replayBuffer2, 550, recordedLengthP1, pRec);
				fclose(pRec);
			}
			break;

		}


		/*
		if (GetSaveFileName(&ofn))
		{
			pRec = fopen(ofn.lpstrFile, "wb");
			fwrite(replayBuffer2, 550, recordedLengthP2, pRec);
			fclose(pRec);

		}
		*/
	}

}

extern "C" __declspec(dllexport) void LoadRecordingP1(int RecordingSlot)
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

		switch (RecordingSlot)
		{

		case 5:

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

			break;

		case 4:

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

			break;

		case 3:

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

			break;

		case 2:

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

			break;

		case 1:
		default:

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

			break;

		}
		/*
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
		*/

	}

}

extern "C" __declspec(dllexport) void LoadRecordingP2(int RecordingSlot)
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

		switch (RecordingSlot)
		{

		case 5:

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

			break;

		case 4:

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

			break;

		case 3:

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

			break;

		case 2:

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

			break;

		case 1:
		default:

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

			break;

		}

		/*
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
		*/

	}

}

extern "C" __declspec(dllexport) void PlaybackP1(int RecordingSlot)
{

	replayingP1 = true;

	switch (RecordingSlot)
	{

	case 5:
		recordReplayIndex5P1 = 0;
		break;

	case 4:
		recordReplayIndex4P1 = 0;
		break;

	case 3:
		recordReplayIndex3P1 = 0;
		break;

	case 2:
		recordReplayIndex2P1 = 0;
		break;

	case 1:
	default:
		recordReplayIndexP1 = 0;
		break;

	}

	//recordReplayIndexP1 = 0;

}

extern "C" __declspec(dllexport) void StopPlaybackP1(int RecordingSlot)
{
	replayingP1 = false;
}

extern "C" __declspec(dllexport) void PlaybackP2(int RecordingSlot)
{
	replayingP2 = true;

	switch (RecordingSlot)
	{

	case 5:
		recordReplayIndex5P2 = 0;
		break;

	case 4:
		recordReplayIndex4P2 = 0;
		break;

	case 3:
		recordReplayIndex3P2 = 0;
		break;

	case 2:
		recordReplayIndex2P2 = 0;
		break;

	case 1:
	default:
		recordReplayIndexP2 = 0;
		break;

	}

	//recordReplayIndexP2 = 0;
}

extern "C" __declspec(dllexport) void StopPlaybackP2(int RecordingSlot)
{
	replayingP2 = false;
}

extern "C" __declspec(dllexport) void PlaybackBoth(int RecordingSlot)
{
	replayingP1 = true;
	replayingP2 = true;

	switch (RecordingSlot)
	{

	case 5:
		recordReplayIndex5 = 0;
		recordReplayIndex5P1 = 0;
		recordReplayIndex5P2 = 0;
		break;

	case 4:
		recordReplayIndex4 = 0;
		recordReplayIndex4P1 = 0;
		recordReplayIndex4P2 = 0;
		break;

	case 3:
		recordReplayIndex3 = 0;
		recordReplayIndex3P1 = 0;
		recordReplayIndex3P2 = 0;
		break;

	case 2:
		recordReplayIndex2 = 0;
		recordReplayIndex2P1 = 0;
		recordReplayIndex2P2 = 0;
		break;

	case 1:
	default:
		recordReplayIndex = 0;
		recordReplayIndexP1 = 0;
		recordReplayIndexP2 = 0;
		break;

	}


	/*
	recordReplayIndex = 0;
	recordReplayIndexP1 = 0;
	recordReplayIndexP2 = 0;
	*/
}

extern "C" __declspec(dllexport) void StopPlaybackBoth(int RecordingSlot)
{
	replayingP1 = false;
	replayingP2 = false;
}

extern "C" __declspec(dllexport) void PlaybackRandomSlotBoth()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(1, 5);

	RandomNum = distr(gen);

	replayingP1 = true;
	replayingP2 = true;

	switch (RandomNum)
	{

	case 5:
		recordReplayIndex5 = 0;
		recordReplayIndex5P1 = 0;
		recordReplayIndex5P2 = 0;
		break;

	case 4:
		recordReplayIndex4 = 0;
		recordReplayIndex4P1 = 0;
		recordReplayIndex4P2 = 0;
		break;

	case 3:
		recordReplayIndex3 = 0;
		recordReplayIndex3P1 = 0;
		recordReplayIndex3P2 = 0;
		break;

	case 2:
		recordReplayIndex2 = 0;
		recordReplayIndex2P1 = 0;
		recordReplayIndex2P2 = 0;
		break;

	case 1:
	default:
		recordReplayIndex = 0;
		recordReplayIndexP1 = 0;
		recordReplayIndexP2 = 0;
		break;

	}

}

extern "C" __declspec(dllexport) void PlaybackRandomP1()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(1, 5);

	RandomNum = distr(gen);

	replayingP1 = true;
	replayingP2 = true;

	switch (RandomNum)
	{

	case 5:
		recordReplayIndex5P1 = 0;
		break;

	case 4:
		recordReplayIndex4P1 = 0;
		break;

	case 3:
		recordReplayIndex3P1 = 0;
		break;

	case 2:
		recordReplayIndex2P1 = 0;
		break;

	case 1:
	default:
		recordReplayIndexP1 = 0;
		break;

	}
}

extern "C" __declspec(dllexport) void PlaybackRandomP2()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(1, 5);

	RandomNum = distr(gen);

	replayingP1 = true;
	replayingP2 = true;

	switch (RandomNum)
	{

	case 5:
		recordReplayIndex5P2 = 0;
		break;

	case 4:
		recordReplayIndex4P2 = 0;
		break;

	case 3:
		recordReplayIndex3P2 = 0;
		break;

	case 2:
		recordReplayIndex2P2 = 0;
		break;

	case 1:
	default:
		recordReplayIndexP2 = 0;
		break;

	}
}

extern "C" __declspec(dllexport) bool CheckIfRecordingP1()
{
	return recordingP1;
}

extern "C" __declspec(dllexport) bool CheckIfRecordingP2()
{
	return recordingP2;
}

extern "C" __declspec(dllexport) bool CheckIfRecordingBoth()
{
	return recording;
}

extern "C" __declspec(dllexport) bool CheckIfPlayback1P(int RecordingSlot)
{
	switch (RecordingSlot)
	{

	case 5:
		if (recordReplayIndex5P1 > 90)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case 4:
		if (recordReplayIndex4P1 > 90)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case 3:
		if (recordReplayIndex3P1 > 90)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case 2:
		if (recordReplayIndex2P1 > 90)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case 1:
	default:
		if (recordReplayIndexP1 > 90)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	}
}

extern "C" __declspec(dllexport) bool CheckIfPlayback2P(int RecordingSlot)
{
	switch (RecordingSlot)
	{

	case 5:
		if (recordReplayIndex5P2 > 90)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case 4:
		if (recordReplayIndex4P2 > 90)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case 3:
		if (recordReplayIndex3P2 > 90)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case 2:
		if (recordReplayIndex2P2 > 90)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case 1:
	default:
		if (recordReplayIndexP2 > 90)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	}
}

extern "C" __declspec(dllexport) bool CheckReplayAvailableP1(int RecordingSlot)
{
	switch (RecordingSlot)
	{

	case 5:
		return replayAvailable5P1;
		break;

	case 4:
		return replayAvailable4P1;
		break;

	case 3:
		return replayAvailable3P1;
		break;

	case 2:
		return replayAvailable2P1;
		break;

	case 1:
	default:
		return replayAvailableP1;
		break;

	}
}

extern "C" __declspec(dllexport) bool CheckReplayAvailableP2(int RecordingSlot)
{
	switch (RecordingSlot)
	{

	case 5:
		return replayAvailable5P2;
		break;

	case 4:
		return replayAvailable4P2;
		break;

	case 3:
		return replayAvailable3P2;
		break;

	case 2:
		return replayAvailable2P2;
		break;

	case 1:
	default:
		return replayAvailableP2;
		break;

	}
}

extern "C" __declspec(dllexport) bool CheckReplayAvailableBoth(int RecordingSlot)
{
	switch (RecordingSlot)
	{

	case 5:
		return replayAvailable5;
		break;

	case 4:
		return replayAvailable4;
		break;

	case 3:
		return replayAvailable3;
		break;

	case 2:
		return replayAvailable2;
		break;

	case 1:
	default:
		return replayAvailable;
		break;

	}
}

extern "C" __declspec(dllexport) void ChangeRecordingSlot(int RecSlot)
{
	RecordingSlot = RecSlot;
}


void TheProccess()
{
	while (true)
	{
		TheRecordButton();
		Sleep(1);
	}

}
