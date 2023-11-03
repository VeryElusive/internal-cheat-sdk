#pragma once
#include "memory.h"
#include "interfaces.h"

#define SIG_ADD_RECAST( type, name, mod, addr, addOffset ) inline type* name{ *reinterpret_cast<type**>( Memory::FindPattern( mod, addr ) + addOffset ) };
#define SIG_OFFSET( name, addr, addOffset ) inline std::uintptr_t name{ Memory::FindPattern( addr ) + addOffset };
#define FUNCTION( name, addr, addOffset, callType ) inline callType name{ reinterpret_cast<callType>( Memory::FindPattern( addr ) + addOffset ) };
#define ADDRESS( name, addr ) SIG_OFFSET( name, addr, 0 );

namespace Displacement {
	// define here
}

#undef ADDRESS
#undef FUNCTION
#undef SIG_OFFSET
#undef SIG_ADD_RECAST