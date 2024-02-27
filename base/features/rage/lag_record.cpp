#include "lag_record.h"
#include "../../havoc.h"

#include "../../core/hooks.h"
#include "../../sdk/fnv1a.h"
#include "../../sdk/valve/entity.h"
#include "../../sdk/valve/interfaces/ienginecvar.h"


void CLagCompensation::Main( ) {
	// TODO: make this calc the server curtime.
	// on server it is: TIME_TO_TICKS( Interfaces::GlobalVars->m_flCurTime - sv_maxunlag->value.fl )
	CVAR( sv_maxunlag );
	const auto deadTicks{ Interfaces::GlobalVars->m_nTickCount - TIME_TO_TICKS( sv_maxunlag->value.fl ) };

	const auto local{ ctx.GetLocal( ) };

	for ( int i{ 1 }; i < Interfaces::Engine->GetMaxClients( ); ++i ) {
		const auto ent{ Interfaces::GameResourceService->m_pGameEntitySystem->Get( i ) };
		if ( !ent
			|| ent == local )
			continue;

		SchemaClassInfoData_t* classInfo{ };
		ent->GetSchemaClassInfo( &classInfo );
		if ( !classInfo )
			continue;

		const auto hashedName{ FNV1A::Hash( classInfo->szName ) };

		if ( hashedName == FNV1A::HashConst( "CCSPlayerController" ) ) {
			auto playerController{ static_cast< CCSPlayerController* >( ent ) };

			auto pawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( playerController->m_hPawn( ) ) };
			if ( !pawn )
				continue;

			PlayerEntry_t& entry{ ctx.m_mapPlayerEntries[ pawn->GetRefEHandle( ).m_nIndex ] };

			if ( entry.m_pPawn != pawn )
				entry.Reset( pawn );

			for ( auto it{ entry.Animations.m_vecLagRecords.begin( ) }; it != entry.Animations.m_vecLagRecords.end( ); ) {
				const auto ticks{ it->m_nPlayerTickCount };
				if ( ticks >= deadTicks )
					break;

				it = entry.Animations.m_vecLagRecords.erase( it );
			}

			if ( entry.Animations.m_vecLagRecords.empty( )
				|| entry.m_pPawn->m_flSimulationTime( ) != entry.Animations.m_vecLagRecords.back( ).m_flSimulationTime )
				AddRecord( entry );
		}
	}
}

// TODO: is this correct ??@!?!??!?!
// we need to see what gets called first- cmove or fsn net update
void CLagCompensation::AddRecord( PlayerEntry_t& entry ) {
	auto cmd{ Interfaces::Input->GetUserCmd( ) };

	if ( ctx.m_flRenderTickFraction > 0.01f )
		++ctx.m_iRenderTick;

	auto& record{ entry.Animations.m_vecLagRecords.emplace_back( entry.m_pPawn, ctx.m_iRenderTick, ctx.m_flRenderTickFraction, Interfaces::GlobalVars->m_nTickCount, ctx.m_flPlayerTickFraction ) };

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

	//const auto backupAbsOrigin{ entry.m_pPawn->GetAbsOrigin( ) };
	//skeleton->m_vecAbsOrigin( ) = entry.m_pPawn->GetAbsOrigin( ) = entry.m_pPawn->m_pGameSceneNode( )->m_vecOrigin( );

	entry.Animations.m_bShouldUpdateBones = true;

	const auto CalcWorldSpaceBones{ Hooks::CalcWorldSpaceBones.Original<decltype( &Hooks::hkCalcWorldSpaceBones )>( ) };
	if ( !CalcWorldSpaceBones )
		return;

	CalcWorldSpaceBones( skeleton, FLAG_HITBOX );

	std::memcpy( record.m_arrBones, &modelState.m_pBones[ 0 ], 128 * sizeof( CBoneData ) );

	if ( entry.Animations.m_vecLastBoneOrigin != gameSceneNode->m_vecOrigin( ) ) {
		const auto delta{ gameSceneNode->m_vecOrigin( ) - entry.Animations.m_vecLastBoneOrigin };

		for ( std::int32_t i = 0; i < 128; ++i )
			record.m_arrBones[ i ].m_vecPosition += delta;
	}

	//skeleton->m_vecAbsOrigin( ) = entry.m_pPawn->GetAbsOrigin( ) = backupAbsOrigin;
}

bool CLagRecord::IsRecordValid( ) const {
	CVAR( sv_maxunlag );

	// this will return 0 unless the server is retarded and modifies sv_maxunlag
	const auto mod{ fmodf( sv_maxunlag->value.fl, 0.2f ) };

	const auto maxDelta{ TIME_TO_TICKS( sv_maxunlag->value.fl - mod ) };

	const auto overlap{ 64.f * mod };
	auto lastValid{ TIME_TO_TICKS( Interfaces::GlobalVars->m_flCurTime ) - maxDelta };
	if ( overlap < 1.f - 0.01f ) {
		if ( overlap <= 0.01f )
			lastValid++;
	}
	//else
	//	lastValid--;

	lastValid--;

	return lastValid < TIME_TO_TICKS( this->m_flSimulationTime );
}

void CLagRecord::Apply( C_CSPlayerPawn* pawn ) const {
	const auto gameSceneNode{ pawn->m_pGameSceneNode( ) };
	if ( !gameSceneNode )
		return;

	const auto skeleton{ gameSceneNode->GetSkeletonInstance( ) };
	if ( !skeleton )
		return;

	const auto modelState{ skeleton->m_modelState( ) };

	const auto model{ modelState.m_hModel };
	if ( !model.IsValid( ) )
		return;

	pawn->GetAbsOrigin( ) = this->m_vecOrigin;

	pawn->m_flSimulationTime( ) = this->m_flSimulationTime;

	std::memcpy( &modelState.m_pBones[ 0 ], this->m_arrBones, 128 * sizeof( CBoneData ) );
}


CLagBackup::CLagBackup( C_CSPlayerPawn* pawn ) {
	const auto gameSceneNode{ pawn->m_pGameSceneNode( ) };
	if ( !gameSceneNode )
		return;

	const auto skeleton{ gameSceneNode->GetSkeletonInstance( ) };
	if ( !skeleton )
		return;

	const auto modelState{ skeleton->m_modelState( ) };

	const auto model{ modelState.m_hModel };
	if ( !model.IsValid( ) )
		return;

	this->m_vecOrigin = pawn->GetAbsOrigin( );

	this->m_flSimulationTime = pawn->m_flSimulationTime( );

	std::memcpy( this->m_arrBones, &modelState.m_pBones[ 0 ], 128 * sizeof( CBoneData ) );
}

void CLagBackup::Apply( C_CSPlayerPawn* pawn ) const {
	const auto gameSceneNode{ pawn->m_pGameSceneNode( ) };
	if ( !gameSceneNode )
		return;

	const auto skeleton{ gameSceneNode->GetSkeletonInstance( ) };
	if ( !skeleton )
		return;

	const auto modelState{ skeleton->m_modelState( ) };

	const auto model{ modelState.m_hModel };
	if ( !model.IsValid( ) )
		return;

	pawn->GetAbsOrigin( ) = this->m_vecOrigin;

	pawn->m_flSimulationTime( ) = this->m_flSimulationTime;

	std::memcpy( &modelState.m_pBones[ 0 ], this->m_arrBones, 128 * sizeof( CBoneData ) );
}