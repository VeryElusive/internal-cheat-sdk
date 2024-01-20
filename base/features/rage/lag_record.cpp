#include "lag_record.h"

#include "../../sdk/fnv1a.h"
#include "../../sdk/valve/entity.h"

void CLagCompensation::Main( ) {
	for ( int i{ 1 }; i < Interfaces::Engine->GetMaxClients( ); ++i ) {
		const auto ent{ Interfaces::GameResourceService->m_pGameEntitySystem->Get( i ) };
		if ( !ent )
			continue;

		SchemaClassInfoData_t* classInfo{ };
		ent->GetSchemaClassInfo( &classInfo );
		if ( !classInfo )
			continue;

		const auto hashedName{ FNV1A::Hash( classInfo->szNname ) };

		if ( hashedName == FNV1A::HashConst( "CCSPlayerController" ) ) {
			auto playerController{ static_cast< CCSPlayerController* >( ent ) };

			auto pawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( playerController->m_hPawn( ) ) };
			if ( !pawn )
				continue;

			PlayerEntry_t& entry{ ctx.PlayerEntries[ i ] };

			if ( entry.m_pPawn != pawn )
				entry.Reset( pawn );

			UpdateRecord( entry );
		}
	}
}

void CLagCompensation::UpdateRecord( PlayerEntry_t& entry ) {

}