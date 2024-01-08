#include "../core/hooks.h"
#include "../core/interfaces.h"

__int64* __fastcall Hooks::hkLevelInit( void* clientModeShared, const char* newMap ) {
	const auto og{ LevelInit.Original<decltype( &hkLevelInit )>( ) };

	if ( Interfaces::GlobalVars == nullptr )
		Interfaces::GlobalVars = *reinterpret_cast< CGlobalVarsBase** >( Memory::ResolveRelativeAddress( Memory::FindPattern( CLIENT_DLL, _( "48 89 0D ? ? ? ? 48 89 41" ) ), 0x3, 0x7 ) );

	return og( clientModeShared, newMap );
}

__int64 __fastcall Hooks::hkLevelShutdown( void* clientModeShared ) {
	const auto og{ LevelShutdown.Original<decltype( &hkLevelShutdown )>( ) };

	Interfaces::GlobalVars = nullptr;

	return og( clientModeShared );
}