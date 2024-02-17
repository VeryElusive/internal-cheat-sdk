#include "../core/hooks.h"
#include "../core/interfaces.h"
#include "../core/config.h"
#include "../sdk/valve/interfaces/ienginecvar.h"
#include "../utils/math.h"

__int64* __fastcall Hooks::hkLevelInit( void* rcx, const char* newMap ) {
	const auto og{ LevelInit.Original<decltype( &hkLevelInit )>( ) };

	if ( Interfaces::GlobalVars == nullptr )
		Interfaces::GlobalVars = *reinterpret_cast< CGlobalVarsBase** >( Memory::ResolveRelativeAddress( Memory::FindPattern( CLIENT_DLL, _( "48 89 0D ? ? ? ? 48 89 41" ) ), 0x3, 0x7 ) );

	ctx.m_mapPlayerEntries.clear( );

	return og( rcx, newMap );
}

__int64 __fastcall Hooks::hkLevelShutdown( void* rcx ) {
	const auto og{ LevelShutdown.Original<decltype( &hkLevelShutdown )>( ) };

	Interfaces::GlobalVars = nullptr;

	ctx.m_mapPlayerEntries.clear( );

	return og( rcx );
}

void __fastcall Hooks::hkOverrideView( void* rcx, CViewSetup* setup ) {
	const auto og{ OverrideView.Original<decltype( &hkOverrideView )>( ) };

	static bool done{ };

	if ( !done ) {
		Interfaces::Cvar->UnlockHiddenCVars( );
		done = true;
	}

	//const auto b{ setup->m_angView 
	// TODO: avoid cvar usage lol
	CVAR( cam_idealdist );
	CVAR( cam_collision );
	CVAR( cam_snapto );
	CVAR( c_thirdpersonshoulder );
	CVAR( c_thirdpersonshoulderaimdist );
	CVAR( c_thirdpersonshoulderdist );
	CVAR( c_thirdpersonshoulderheight );
	CVAR( c_thirdpersonshoulderoffset );

	if ( Configs::m_cConfig.m_bThirdperson && Configs::m_cConfig.m_kThirdperson.m_bEnabled ) {
		if ( !Interfaces::Input->m_bCameraInThirdPerson )
			Interfaces::Input->m_bCameraInThirdPerson = true;

		cam_idealdist->value.fl = Math::Interpolate( cam_idealdist->value.fl, Configs::m_cConfig.m_iThirdpersonDistance, ctx.m_flFrameTime * 10.f );
		cam_collision->value.i1 = true;
		cam_snapto->value.i1 = true;
		c_thirdpersonshoulder->value.i1 = true;
		c_thirdpersonshoulderaimdist->value.fl = 0.f;
		c_thirdpersonshoulderdist->value.fl = 0.f;
		c_thirdpersonshoulderheight->value.fl = 0.f;
		c_thirdpersonshoulderoffset->value.fl = 0.f;
	}
	else {
		if ( Interfaces::Input->m_bCameraInThirdPerson )
			Interfaces::Input->m_bCameraInThirdPerson = false;

		cam_idealdist->value.fl = 30;
	}


	/*ctx.GetLocal( );
	if ( ctx.m_pLocal && ctx.m_pLocal->m_bPawnIsAlive( ) ) {
		const auto localPawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };
		if ( localPawn ) {
			const auto estViewOffset{ setup->m_vecOrigin - localPawn->m_pGameSceneNode( )->m_vecAbsOrigin( ) };

			setup->m_vecOrigin.x -= estViewOffset.x;
			setup->m_vecOrigin.y -= estViewOffset.y;

			static auto old{ estViewOffset.z };
			static int lastTickCount{ };

			constexpr int smoothingAmount{ 5 };

			static float prevOffsets[ smoothingAmount ]{ };
			static int currentIndex = 0;

			if ( lastTickCount != Interfaces::GlobalVars->m_nTickCount ) {
				prevOffsets[ currentIndex ] = estViewOffset.z;
				currentIndex = ( currentIndex + 1 ) % smoothingAmount;

				lastTickCount = Interfaces::GlobalVars->m_nTickCount;
			}

			float sum{ };
			for ( int i = 0; i < smoothingAmount; ++i )
				sum += prevOffsets[ i ];

			const auto average{ sum / smoothingAmount };

			if ( std::abs( estViewOffset.z - average ) )
				setup->m_vecOrigin.z += ( average - estViewOffset.z );
		}
	}*/

	return og( rcx, setup );
}