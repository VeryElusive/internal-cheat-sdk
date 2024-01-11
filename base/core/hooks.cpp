#include "hooks.h"
#include "../utils/render/render.h"
#include <d3d9.h>
#include <thread>

void Hooks::Init( ) {
	if ( MH_Initialize( ) != MH_OK )
		return;

	if ( !Render::PrepareMethodTable( ) )
		return;

	INIT_HOOK( Present, Render::MethodsTable[ 8 ] );
	//INIT_HOOK( ResizeBuffers, Render::MethodsTable[ 13 ] );

	// STRING XREF: "mapname", "transition", "game_newmap"
	INIT_HOOK( LevelInit, Memory::FindPattern( CLIENT_DLL, _( "48 89 5C 24 ? 56 48 83 EC ? 48 8B 0D ? ? ? ? 48 8B F2" ) ) );

	// STRING XREF: "map_shutdown"
	INIT_HOOK( LevelShutdown, Memory::FindPattern( CLIENT_DLL, _( "48 83 EC ? 48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 45 33 C9 45 33 C0 48 8B 01 FF 50 ? 48 85 C0 74 ? 48 8B 0D ? ? ? ? 48 8B D0 4C 8B 01 48 83 C4 ? 49 FF 60 ? 48 83 C4 ? C3 CC CC CC 48 83 EC ? 4C 8B D9" ) ) );

	// this is a really gay way to do it, but anyway. it works.
	// called from CViewRender::OnRenderStart
	INIT_HOOK( GetMatrixForView, Memory::FindPattern( CLIENT_DLL, _( "40 53 48 81 EC ? ? ? ? 49 8B C1" ) ) );

	INIT_HOOK( MouseInputEnabled, Memory::GetVFunc( Interfaces::Input, 12 ) );

	INIT_HOOK( FrameStageNotify, Memory::GetVFunc( Interfaces::Client, 33 ) );
}