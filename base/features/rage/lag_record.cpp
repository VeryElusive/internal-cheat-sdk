#include "lag_record.h"

#include "../../core/hooks.h"
#include "../../sdk/fnv1a.h"
#include "../../sdk/valve/entity.h"

void CLagCompensation::Main( ) {
	for ( int i{ 1 }; i < Interfaces::Engine->GetMaxClients( ); ++i ) {
		const auto ent{ Interfaces::GameResourceService->m_pGameEntitySystem->Get( i ) };
		if ( !ent
			|| ent == ctx.m_pLocal )
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

			PlayerEntry_t& entry{ ctx.m_mapPlayerEntries[ pawn->GetRefEHandle( ).m_nIndex ] };

			if ( entry.m_pPawn != pawn )
				entry.Reset( pawn );

			if ( entry.m_pPawn->m_flSimulationTime( ) != entry.Animations.m_flSimulationTime )
				UpdateRecord( entry );
		}
	}
}

void CLagCompensation::UpdateRecord( PlayerEntry_t& entry ) {
	entry.Animations.m_flSimulationTime = entry.m_pPawn->m_flSimulationTime( );

	const auto gameSceneNode{ entry.m_pPawn->m_pGameSceneNode( ) };
	if ( !gameSceneNode )
		return;

	const auto skeleton{ gameSceneNode->GetSkeletonInstance( ) };
	if ( !skeleton )
		return;

	const auto modelState{ skeleton->m_modelState( ) };

	const auto model{ modelState.m_hModel };
	if ( !model.IsValid( ) )
		return;

	const auto backupAbsOrigin{ entry.m_pPawn->GetAbsOrigin( ) };
	skeleton->m_vecAbsOrigin( ) = entry.m_pPawn->GetAbsOrigin( ) = entry.m_pPawn->m_pGameSceneNode( )->m_vecOrigin( );

	entry.Animations.m_bShouldUpdateBones = true;

	const auto CalcWorldSpaceBones{ Hooks::CalcWorldSpaceBones.Original<decltype( &Hooks::hkCalcWorldSpaceBones )>( ) };
	CalcWorldSpaceBones( skeleton, FLAG_HITBOX );

	std::memcpy( entry.Animations.m_arrBones, &modelState.m_pBones[ 0 ], model->m_iBoneCount * sizeof( CBoneData ) );

	skeleton->m_vecAbsOrigin( ) = entry.m_pPawn->GetAbsOrigin( ) = backupAbsOrigin;
}