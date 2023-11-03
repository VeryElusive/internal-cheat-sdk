#include "interfaces.h"

void Interfaces::Init( ) {
	GlobalVars = *reinterpret_cast< CGlobalVarsBase** >( Memory::ResolveRelativeAddress( Memory::FindPattern( CLIENT_DLL, _( "48 89 0D ? ? ? ? 48 89 41" ) ), 0x3, 0x7 ) );
}