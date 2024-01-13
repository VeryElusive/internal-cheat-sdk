#include "visuals.h"
#include "../../havoc.h"
#include "../../sdk/valve/interfaces/ischemasystem.h"
#include "../../sdk/fnv1a.h"
#include <string>
#include "../../utils/render/render.h"

void CVisuals::Main( ) {
	if ( !ctx.m_pLocal )
		return;

	Render::Text( { 300, 500 }, ( "fw: " + std::to_string( ctx.m_flForwardmove ) ).c_str( ), Color( 255,255,255 ), 0, 13, Render::Fonts.NameESP );
	Render::Text( { 300, 520 }, ( "sd: " + std::to_string( ctx.m_flSidemove ) ).c_str( ), Color( 255,255,255 ), 0, 13, Render::Fonts.NameESP );
	Render::Text( { 300, 540 }, ( "up: " + std::to_string( ctx.m_flUpmove ) ).c_str( ), Color( 255,255,255 ), 0, 13, Render::Fonts.NameESP );

	// replace with highestentity (in m_pGameEntitySystem) when we do other entity visuals
	for ( int i{ 1 }; i < Interfaces::Engine->GetMaxClients( ); ++i ) {
		C_BaseEntity* ent{ Interfaces::GameResourceService->m_pGameEntitySystem->Get( i ) };
		if ( !ent )
			continue;

		SchemaClassInfoData_t* classInfo{ };
		ent->GetSchemaClassInfo( &classInfo );
		if ( !classInfo )
			continue;

		const auto uHashedName{ FNV1A::Hash( classInfo->szNname ) };

		if ( uHashedName == FNV1A::HashConst( "CCSPlayerController" ) ) {
			auto playerController{ static_cast< CCSPlayerController* >( ent ) };

			C_CSPlayerPawn* pawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( playerController->m_hPawn( ) ) };
			if ( !pawn )
				continue;

			PlayerEntry_t& entry{ ctx.PlayerEntries[ i ] };

			if ( entry.m_pPawn != pawn )
				entry.Reset( pawn );

			entry.m_pName = playerController->m_sSanitizedPlayerName( );

			if ( playerController->m_bPawnIsAlive( ) )
				Features::Visuals.HandlePlayer( entry );
		}

		//if ( );
	}
}