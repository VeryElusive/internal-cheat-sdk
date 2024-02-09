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

	INIT_HOOK( MouseInputEnabled, Memory::GetVFunc( Interfaces::Input, 13 ) );

	INIT_HOOK( CreateMove, Memory::GetVFunc( Interfaces::Input, 15 ) );

	INIT_HOOK( FrameStageNotify, Memory::GetVFunc( Interfaces::Client, 33 ) );

	INIT_HOOK( GetMatrixForView, Memory::FindPattern( CLIENT_DLL, _( "40 53 48 81 EC ? ? ? ? 49 8B C1" ) ) );

	// STRING XREF: "mapname", "transition", "game_newmap"
	INIT_HOOK( LevelInit, Memory::FindPattern( CLIENT_DLL, _( "48 89 5C 24 ? 56 48 83 EC ? 48 8B 0D ? ? ? ? 48 8B F2" ) ) );

	// STRING XREF: "map_shutdown"
	INIT_HOOK( LevelShutdown, Memory::FindPattern( CLIENT_DLL, _( "48 83 EC ? 48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 45 33 C9 45 33 C0 48 8B 01 FF 50 ? 48 85 C0 74 ? 48 8B 0D ? ? ? ? 48 8B D0 4C 8B 01 48 83 C4 ? 49 FF 60 ? 48 83 C4 ? C3 CC CC CC 48 83 EC ? 4C 8B D9" ) ) );
	
	INIT_HOOK( OverrideView, Memory::FindPattern( CLIENT_DLL, _( "48 89 5C 24 ? 55 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 01 48 8B DA" ) ) );

	INIT_HOOK( Unknown02, Memory::FindPattern( CLIENT_DLL, _( "85 D2 0F 85 ? ? ? ? 57 48 83 EC 20 48 89 5C 24" ) ) );

	INIT_HOOK( CalcWorldSpaceBones, Memory::FindPattern( CLIENT_DLL, _( "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 8B F1" ) ) );

	INIT_HOOK( Lagcompensation, Memory::FindPattern( CLIENT_DLL, _( "48 8B C4 4C 89 48 20 55 56 41 56 48 8D 68 B1 48 81 EC" ) ) );


	//INIT_HOOK( ReadFrameInput, Memory::FindPattern( CLIENT_DLL, _( "40 55 53 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 8B" ) ) );


	//INIT_HOOK( CalcAnimationState, Memory::FindPattern( CLIENT_DLL, _( "40 55 53 56 57 41 54 41 55 41 56 41 57 B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8D 6C 24 ? 48 8B F9 44 8B" ) ) );

	//INIT_HOOK( SetTargetTime, Memory::FindPattern( SERVER_DLL, _( "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 40 F3 0F 10 05" ) ) );
	//INIT_HOOK( ModifyBonePosition, Memory::FindPattern( ANIMATION_SYSTEM, _( "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 20 4C 89 40" ) ) );

	//INIT_HOOK( SetViewAngles, Memory::FindPattern( CLIENT_DLL, _( "85 D2 75 3F 48 63 81 ? ? ? ? F2 41 0F 10 00 85" ) ) );

	//INIT_HOOK( SendMove, Memory::FindPattern( ENGINE2_DLL, _( "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 50 33 C0 C6 81" ) ) )
}