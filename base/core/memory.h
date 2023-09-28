#pragma once
#include "../havoc.h"
#include <Psapi.h>
#include <TlHelp32.h>

#define IMAGE_SIZE 1631744

namespace Memory {
	inline std::uintptr_t ACModuleHandle{ };

	std::vector<std::optional<std::uint8_t>> PatternToBytes( std::string pattern );

	std::uintptr_t GetBaseAddress( );

	std::uintptr_t FindPattern( std::string pattern );
}