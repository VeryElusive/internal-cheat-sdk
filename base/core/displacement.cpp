#include "displacement.h"
#include "memory.h"

#define SIG_ADD_RECAST( type, name, mod, addr, addOffset ) inline type* name{ *reinterpret_cast<type**>( Memory::FindPattern( mod, addr ) + addOffset ) };
#define SIG_OFFSET( name, addr, addOffset ) inline std::uintptr_t name{ Memory::FindPattern( addr ) + addOffset };
#define FUNCTION( name, mod, addr, addOffset, callType ) inline callType name{ reinterpret_cast<callType>( Memory::FindPattern( mod, addr ) + addOffset ) };
#define ADDRESS( name, addr ) SIG_OFFSET( name, addr, 0 );

void Displacement::Init( ) {
	GetBaseEntity = reinterpret_cast< fnGetBaseEntity >( Memory::FindPattern( CLIENT_DLL, _( "81 FA ? ? ? ? 77 ? 8B C2 C1 F8 ? 83 F8 ? 77 ? 48 98 48 8B 4C C1 ? 48 85 C9 74 ? 8B C2 25 ? ? ? ? 48 6B C0 ? 48 03 C8 74 ? 8B 41 ? 25 ? ? ? ? 3B C2 75 ? 48 8B 01" ) ) );
}

#undef ADDRESS
#undef FUNCTION
#undef SIG_OFFSET
#undef SIG_ADD_RECAST