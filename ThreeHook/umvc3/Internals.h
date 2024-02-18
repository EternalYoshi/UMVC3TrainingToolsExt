#pragma once

//public void FUN_1402b41b0(longlong param_1);
//#define longlong  long long
//void FUN_1402b41b0(longlong);

extern "C" {
	__declspec(dllexport) std::string CheckConnection();
	__declspec(dllexport) int CheckConnectionTwo();
	__declspec(dllexport) void DeployTheHooks(int);
	__declspec(dllexport) void RecordBoth(int);
	__declspec(dllexport) void RecordP1(int);
	__declspec(dllexport) void RecordP2(int);
	__declspec(dllexport) void StopRecording();
	__declspec(dllexport) void SaveP1Recording(int);
	__declspec(dllexport) void SaveP2Recording(int);
	__declspec(dllexport) void PlaybackP1(int);
	__declspec(dllexport) void PlaybackP2(int);
	__declspec(dllexport) void PlaybackBoth(int);
	__declspec(dllexport) void StopPlaybackP1(int);
	__declspec(dllexport) void StopPlaybackP2(int);
	__declspec(dllexport) void StopPlaybackBoth(int);
	__declspec(dllexport) void PlaybackRandomSlotP1();
	__declspec(dllexport) void PlaybackRandomSlotP2();
	__declspec(dllexport) void PlaybackRandomSlotBoth();
	__declspec(dllexport) bool CheckIfRecordingP1();
	__declspec(dllexport) bool CheckIfRecordingP2();
	__declspec(dllexport) bool CheckIfRecordingBoth();
	__declspec(dllexport) bool CheckIfPlayback1P(int);
	__declspec(dllexport) bool CheckIfPlayback2P(int);
	__declspec(dllexport) bool CheckReplayAvailableP1(int);
	__declspec(dllexport) bool CheckReplayAvailableP2(int);
	__declspec(dllexport) bool CheckReplayAvailableBoth(int);
	__declspec(dllexport) bool ChangeRecordingSlot(int);
	__declspec(dllexport) void StartTheTick();


}