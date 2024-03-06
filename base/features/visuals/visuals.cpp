#include "visuals.h"
#include "../../havoc.h"
#include "../../sdk/valve/interfaces/ischemasystem.h"
#include "../../sdk/fnv1a.h"
#include <string>
#include "../../utils/render/render.h"

void CVisuals::Main( ) {
	if ( !ctx.GetLocalPawn( ) )
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

		const auto uHashedName{ FNV1A::Hash( classInfo->szName ) };

		if ( uHashedName == FNV1A::HashConst( "CCSPlayerController" ) ) {
			auto playerController{ static_cast< CCSPlayerController* >( ent ) };

			C_CSPlayerPawn* pawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( playerController->m_hPawn( ) ) };
			if ( !pawn )
				continue;

			PlayerEntry_t& entry{ ctx.m_mapPlayerEntries[ pawn->GetRefEHandle( ).m_nIndex ] };

			if ( entry.m_pPawn != pawn
				|| !playerController->m_bPawnIsAlive( ) )
				entry.Reset( pawn );

			entry.m_pName = playerController->m_sSanitizedPlayerName( );

			if ( playerController->m_bPawnIsAlive( ) && entry.Visuals.m_cBBox.Valid( ) )
				Features::Visuals.HandlePlayer( entry );
		}
	}

	if ( ctx.DEBUGBactrackEntry ) {
		const auto gameSceneNode{ ctx.DEBUGBactrackEntry->m_pPawn->m_pGameSceneNode( ) };
		if ( !gameSceneNode )
			return;

		const auto skeleton{ gameSceneNode->GetSkeletonInstance( ) };
		if ( !skeleton )
			return;

		const auto modelState{ skeleton->m_modelState( ) };

		const auto model{ modelState.m_hModel };
		if ( !model.IsValid( ) )
			return;

		for ( std::int32_t i = 0; i < 128; ++i ) {
			const auto boneFlag{ model->GetBoneFlags( i ) };
			if ( !( boneFlag & FLAG_HITBOX ) )
				continue;

			const auto boneParentIndex{ model->GetBoneParent( i ) };
			if ( boneParentIndex == -1 )
				continue;

			Vector2D screen{ };
			Vector2D parentScreen{ };

			if ( Render::WorldToScreen( ctx.DEBUGBacktrackBones[ i ].m_vecPosition, screen )
				&& Render::WorldToScreen( ctx.DEBUGBacktrackBones[ boneParentIndex ].m_vecPosition, parentScreen ) )
				Render::Line( screen, parentScreen, Color( 255, 255, 255 ) );
		}
	}

	for ( std::int32_t i = 0; i < 32; ++i ) {
		if ( ctx.DEBUGPointPrintout[ i ] == Vector( 0, 0, 0 ) )
			continue;

		Vector2D screen{ };

		if ( Render::WorldToScreen( ctx.DEBUGPointPrintout[ i ], screen ) )
			Render::RectFilled( screen, { 4, 4 }, Color( 255, 255, 255 ) );
	}

	const auto weaponServices{ ctx.GetLocalPawn( )->m_pWeaponServices( ) };
	if ( !weaponServices )
		return;

	const auto weapon{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSWeaponBase>( weaponServices->m_hActiveWeapon( ) ) };
	if ( !weapon )
		return;

	Render::Text( { 300, 600 }, ( "spread: " + std::to_string( ctx.DEBUGSpread ) ).c_str( ), Color( 255, 255, 255 ), 0, 13, Render::Fonts.NameESP );
	Render::Text( { 300, 610 }, ( "inaccuracy: " + std::to_string( ctx.DEBUGInac ) ).c_str( ), Color( 255, 255, 255 ), 0, 13, Render::Fonts.NameESP );
}