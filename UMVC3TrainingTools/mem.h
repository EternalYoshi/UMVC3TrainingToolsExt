#pragma once
//#include "stdafx.h"
#include<Windows.h>
#include <vector>

//guidedhacking.com

namespace mem
{
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void Nop(BYTE* dst, unsigned int size);
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);

	bool DetourX(BYTE* src, BYTE* dst, const uintptr_t len);
	BYTE* TrampHookX(BYTE* sec, BYTE* dst, const uintptr_t len);

}
