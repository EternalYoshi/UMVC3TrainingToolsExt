#pragma once

#include <cstdint>

class sigscan
{

public:
	uintptr_t start, end;
	sigscan(const char* name);

	// Needs a singleton so it can be used in global initializers
	static const sigscan& get();

	uintptr_t scan(const char* sig, const char* mask) const;
	void scan2(unsigned char* sig, const char* mask) const;
	uintptr_t findAll(const char* sig, const char* mask) const;
};

inline void* get_rip_relative(uintptr_t offset)
{
	return (void*)(offset + 4 + *(int32_t*)offset);
}

extern bool failedScan;