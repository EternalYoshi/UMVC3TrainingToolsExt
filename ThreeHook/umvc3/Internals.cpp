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
#include <ctime>


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

int GameMode;

struct Recording {

	int FrameCount;
	char* InputBinary[3844];

};

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

char* CheckConnection()
{
	return "This is from the dll. If you can read this you got it.";
}
