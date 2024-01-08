#include "visuals.h"
#include "../../havoc.h"
#include "../../sdk/valve/interfaces/ischemasystem.h"
#include "../../sdk/fnv1a.h"

void CVisuals::Main( ) {
	if ( !ctx.m_pLocal )
		return;

	// replace with highestentity (in m_pGameEntitySystem) when we do other entity visuals
	for ( int i{ 1 }; i < Interfaces::Engine->GetMaxClients( ); ++i ) {
		C_BaseEntity* ent{ Interfaces::GameResourceService->m_pGameEntitySystem->Get( i ) };
		if ( !ent )
			return;

		SchemaClassInfoData_t* classInfo{ };
		ent->GetSchemaClassInfo( &classInfo );
		if ( !classInfo )
			continue;

		PlayerEntry_t& entry{ ctx.PlayerEntries[ i ] };

		const auto uHashedName{ FNV1A::Hash( classInfo->szNname ) };

		if ( uHashedName == FNV1A::HashConst( "CCSPlayerController" ) ) {
			auto playerController{ static_cast< CCSPlayerController* >( ent ) };

			C_CSPlayerPawn* pawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( playerController->m_hPawn( ) ) };
			if ( !pawn )
				continue;

			if ( entry.m_pPawn != pawn )
				entry.Reset( pawn );

			entry.m_pName = playerController->m_sSanitizedPlayerName( );

			if ( playerController->m_bPawnIsAlive( ) )
				Features::Visuals.HandlePlayer( entry );
		}

		//if ( );
	}
}