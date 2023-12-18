#pragma once
#include "..\utils.h"
#include "..\umvc3\Camera.h"
#include "..\umvc3\Vector.h"

int64 CameraConstrutctor_Hook(int64 camera, int64 a2);
void PluginProcess();

/*
void attachHooks(Trampoline* tramp, bool phys) 
{
	InjectHook(_addr(0x140289c5a), tramp->Jump(FUN_1402b41b0), PATCH_CALL);

}
*/