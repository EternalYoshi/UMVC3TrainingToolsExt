#include "pch.h"
#include "mem.h"

bool mem::DetourX(BYTE* src, BYTE* dst, const uintptr_t len)
{
	//Ensures Length is greater than 5.
	if (len < 14) return false;

	//Unconditional Jump(32-bit).
	*src = 0XE9;

	//Unconditional Jump Instruction.
	BYTE JumpIn[] =
	{ 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	//Creates the Gateway.
	void* gateway = (BYTE*)VirtualAlloc(0, len + sizeof(JumpIn), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	//Writes the stolen bytes to gateway.
	memcpy_s(gateway, len, src, len);

	//Get the gateway to destination address.
	uintptr_t gatewayRelativeAddr = src - gateway - 14;

	//DWORD curPro = 0;
	//VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curPro);

	return true;

}

BYTE* mem::TrampHookX(BYTE* src, BYTE* dst, const uintptr_t len)
{

	if (len < 5) return 0;

	//Create the Gateway.
	BYTE* gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	//Writes the stolen bytes to gateway.
	memcpy_s(gateway, len, src, len);

	//Get the gateway to destination address.
	uintptr_t gatewayRelativeAddr = src - gateway - 5;

	// add the jmp opcode to the end of the gateway.
	*(gateway + len) = 0xE9;

	//Write the address of the gateway to the gateway to the jmp.
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gatewayRelativeAddr;

	//Perform Detour.
	DetourX(src, dst, len);

	return gateway;

}


//https://guidedhacking.com/threads/how-to-hack-any-game-first-internal-hack-dll-tutorial.12142

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

/*
uintptr_t mem::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{

	uintptr_t addr = ptr;
	for(unsigned int i =0; i < offsets.size(); i++)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;

}
*/