#pragma once
#include "memory.h"
#include "../sdk/player.h"

#define SIG_ADD_RECAST( name, addr, addOffset, type ) inline type* name{ *reinterpret_cast<type**>( Memory::FindPattern( addr ) + addOffset ) };
#define SIG_OFFSET( name, addr, addOffset ) inline std::uintptr_t name{ Memory::FindPattern( addr ) + addOffset };
#define FUNCTION( name, addr, addOffset, callType ) inline callType name{ reinterpret_cast<callType>( Memory::FindPattern( addr ) + addOffset ) };
#define ADDRESS( name, addr ) SIG_OFFSET( name, addr, 0 );

typedef void( __fastcall* inputgrab_t )( bool, uint8_t );
typedef void( __vectorcall* RayDamage_t )( Vector& from, Vector& to, Player_t* d );

namespace Displacement {
	// define here
}

#undef ADDRESS
#undef FUNCTION
#undef SIG_OFFSET
#undef SIG_ADD_RECAST