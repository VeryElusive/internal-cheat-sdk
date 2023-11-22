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

		SchemaClassInfoData_t* pClassInfo = nullptr;
		ent->GetSchemaClassInfo( &pClassInfo );
		if ( pClassInfo == nullptr )
			continue;

		const auto uHashedName{ FNV1A::Hash( pClassInfo->szNname ) };

		if ( uHashedName == FNV1A::HashConst( "CCSPlayerController" ) ) {
			Features::Visuals.HandlePlayer( static_cast< const CCSPlayerController*>( ent ) );
		}

		//if ( );
	}
}