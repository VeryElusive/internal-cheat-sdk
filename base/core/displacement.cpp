#include "displacement.h"
#include "memory.h"

void Displacement::Init( ) {
	// xref: "(missing),", "(missing)", "Ent %3d: %s class %s name %s\n" | or find "cl_showents" cvar -> look for callback
	GetBaseEntity = reinterpret_cast< fnGetBaseEntity >( Memory::FindPattern( CLIENT_DLL, _( "81 FA ? ? ? ? 77 ? 8B C2 C1 F8 ? 83 F8 ? 77 ? 48 98 48 8B 4C C1 ? 48 85 C9 74 ? 8B C2 25 ? ? ? ? 48 6B C0 ? 48 03 C8 74 ? 8B 41 ? 25 ? ? ? ? 3B C2 75 ? 48 8B 01" ) ) );
	SetViewAngles = reinterpret_cast< fnSetViewAngles >( Memory::FindPattern( CLIENT_DLL, _( "85 D2 75 3F 48 63 81 ? ? ? ? F2 41 0F 10 00 85" ) ) );
	GetBoneParent = reinterpret_cast< fnGetBoneParent >( Memory::FindPattern( CLIENT_DLL, _( "85 D2 78 17 3B 91 70" ) ) );
	//ViewMatrix = reinterpret_cast< ViewMatrix_t* >( Memory::FindPattern( CLIENT_DLL, _( "48 8D 0D ? ? ? ? 48 C1 E0 06 48 03 C1" ) ) );

	const void* SDL3{ Memory::GetModuleBaseHandle( _( L"SDL3.dll" ) ) };
	if ( SDL3 )
		fnSetRelativeMouseMode = reinterpret_cast< decltype( fnSetRelativeMouseMode ) >( Memory::GetExportAddress( SDL3, "SDL_SetRelativeMouseMode" ) );
}

#undef ADDRESS
#undef FUNCTION
#undef SIG_OFFSET
#undef SIG_ADD_RECAST