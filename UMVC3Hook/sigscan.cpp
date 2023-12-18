#include "sigscan.h"
#include <stdexcept>
#include <fstream>
#include <ios>
#include <iomanip>
#include <Windows.h>
#include <Psapi.h>

bool failedScan = false;

const sigscan& sigscan::get()
{
	static sigscan instance("umvc3.exe");
	return instance;
}

sigscan::sigscan(const char* name)
{
	const auto module = GetModuleHandleA(nullptr);
	if (module == nullptr)
		throw std::runtime_error("Module not found");

	MODULEINFO info;
	GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(info));
	start =  (uintptr_t)(info.lpBaseOfDll);
	end = start + info.SizeOfImage;
}

uintptr_t sigscan::scan(const char* sig, const char* mask) const
{
	const auto last_scan = end - strlen(mask) + 1;
	for (auto addr = start; addr < last_scan; addr++) {
		for (size_t i = 0;; i++) {
			if (mask[i] == '\0')
				return addr;
			if (mask[i] != '?' && sig[i] != *(char*)(addr + i))
				break;
		}
	}

	printf("couldn't find\n");

	//throw std::runtime_error("Sigscan failed");
	failedScan = true;
	return 0;
}
void sigscan::scan2(unsigned char* sig, const char* mask) const
{
	const auto last_scan =start- end - strlen(mask) + 1;
	for (auto addres = 0; addres < last_scan; addres++) {
		auto addr = addres + start;
		for (size_t i = 0;; i++) {
			if (mask[i] == '\0')
				printf("scan2:%x - %x\n", addres + 0x140000000,addr);
			if (sig[i] != *(unsigned char*)(addr + i))
				break;
		}
	}

	printf("couldn't find\n");

	//throw std::runtime_error("Sigscan failed");
	failedScan = true;
}

uintptr_t sigscan::findAll(const char* sig, const char* mask) const
{
	auto len = 4;
	const auto last_scan = end - len+1;
	for (auto addr = start; addr < last_scan; addr++) {
		if (sig[0] == *(char*)(addr + 0) && sig[1] == *(char*)(addr + 1) && sig[2] == *(char*)(addr + 2)) {
			printf("%02x-",addr);
			printf("\n");
		}
		if (sig[0] == *(char*)(addr + 2) && sig[1] == *(char*)(addr + 1) && sig[2] == *(char*)(addr + 0)) {
			printf("%02x-",addr);
			printf("\n");
		}
	}

	printf("couldn't find\n");

	//throw std::runtime_error("Sigscan failed");
	failedScan = true;
	return 0;
}