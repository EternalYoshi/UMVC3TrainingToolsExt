#pragma once
#include "utils/MemoryMgr.h"
#include "utils/Trampoline.h"
#include "utils/Patterns.h"
#include <vector>
using namespace Memory::VP;

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
bool replaying = false;
bool replayAvailable = false;
int recordReplayIndex = 0;
int recordedLength = 0;
int RecordingSlot = 1;
#define ReplayLength (60*120)
#define ReplayBufferSize 550
uint8_t replayBuffer[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer2[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer3[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer4[ReplayLength][ReplayBufferSize];
uint8_t replayBuffer5[ReplayLength][ReplayBufferSize];

void FUN_1402b41b0(longlong param_1)
{
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
