#include "displacement.h"
#include "memory.h"

void Displacement::Init( ) {
	// xref: "(missing),", "(missing)", "Ent %3d: %s class %s name %s\n" | or find "cl_showents" cvar -> look for callback
	GetBaseEntity = reinterpret_cast< fnGetBaseEntity >( Memory::FindPattern( CLIENT_DLL, _( "81 FA ? ? ? ? 77 ? 8B C2 C1 F8 ? 83 F8 ? 77 ? 48 98 48 8B 4C C1 ? 48 85 C9 74 ? 8B C2 25 ? ? ? ? 48 6B C0 ? 48 03 C8 74 ? 8B 41 ? 25 ? ? ? ? 3B C2 75 ? 48 8B 01" ) ) );
	SetViewAngles = reinterpret_cast< fnSetViewAngles >( Memory::FindPattern( CLIENT_DLL, _( "85 D2 75 3F 48 63 81 ? ? ? ? F2 41 0F 10 00 85" ) ) );
	GetBoneParent = reinterpret_cast< fnGetBoneParent >( Memory::FindPattern( CLIENT_DLL, _( "85 D2 78 17 3B 91 70" ) ) );
	GetBoneFlags = reinterpret_cast< fnGetBoneParent >( Memory::FindPattern( CLIENT_DLL, _( "85 D2 78 16 3B 91" ) ) );
	EventGetInt = reinterpret_cast< fnEventGetInt >( Memory::FindPattern( CLIENT_DLL, _( "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 41 56 48 83 EC 30 48 8B 01 41 8B F0 4C 8B F1 41 B0 01 48 8D 4C 24 20 48 8B DA 48 8B 78" ) ) );
	GetHitboxSet = reinterpret_cast< fnGetHitboxSet >( Memory::FindPattern( CLIENT_DLL, _( "48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 8B DA 48 8B F9 E8 ? ? ? ? 48 8B F0 48 85 C0 0F 84" ) ) );
	TraceFilterInit = reinterpret_cast< fnTraceFilterInit >( Memory::FindPattern( CLIENT_DLL, _( "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 0F B6 41 37 33" ) ) );
	CreateTrace = reinterpret_cast< fnCreateTrace >( Memory::FindPattern( CLIENT_DLL, _( "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 40 F2" ) ) );
	InitTraceInfo = reinterpret_cast< fnInitTraceInfo >( Memory::FindPattern( CLIENT_DLL, _( "48 89 5C 24 08 57 48 83 EC 20 48 8B D9 33 FF 48 8B 0D" ) ) );
	GetTraceInfo = reinterpret_cast< fnGetTraceInfo >( Memory::FindPattern( CLIENT_DLL, _( "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 60 48 8B E9 0F" ) ) );
	HandleBulletPenetration = reinterpret_cast< fnHandleBulletPenetration >( Memory::FindPattern( CLIENT_DLL, _( "48 8B C4 44 89 48 20 55 57 41 55" ) ) );

	FindVarMaterial = reinterpret_cast< fnFindVarMaterial >( Memory::FindPattern( PARTICLES_DLL, _( "48 89 5C 24 08 57 48 81 EC C0 00 00 00 33 C0 8B" ) ) );
	//ViewMatrix = reinterpret_cast< ViewMatrix_t* >( Memory::FindPattern( CLIENT_DLL, _( "48 8D 0D ? ? ? ? 48 C1 E0 06 48 03 C1" ) ) );

	const void* SDL3{ Memory::GetModuleBaseHandle( _( L"SDL3.dll" ) ) };
	if ( SDL3 )
		fnSetRelativeMouseMode = reinterpret_cast< decltype( fnSetRelativeMouseMode ) >( Memory::GetExportAddress( SDL3, "SDL_SetRelativeMouseMode" ) );
}

#undef ADDRESS
#undef FUNCTION
#undef SIG_OFFSET
#undef SIG_ADD_RECAST