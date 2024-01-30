#include "ragebot.h"

#include "../../sdk/fnv1a.h"

void CRageBot::Main( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	/*	if ( !Config::Get<bool>( Vars.RagebotEnable )
		|| ctx.m_pWeapon->m_bReloading( )
		|| ( ctx.m_pWeaponData->nWeaponType >= WEAPONTYPE_C4
			&& ctx.m_pWeapon->m_iItemDefinitionIndex( ) != WEAPON_TASER )
		|| ctx.m_pLocal->m_MoveType( ) == MOVETYPE_LADDER )
		return;*/

	CAimTarget bestTarget{ };
	CAimTarget target{ };

	// replace with highestentity (in m_pGameEntitySystem) when we do other entity visuals
	for ( int i{ 1 }; i < Interfaces::Engine->GetMaxClients( ); ++i ) {
		C_BaseEntity* ent{ Interfaces::GameResourceService->m_pGameEntitySystem->Get( i ) };
		if ( !ent )
			continue;

		SchemaClassInfoData_t* classInfo{ };
		ent->GetSchemaClassInfo( &classInfo );
		if ( !classInfo )
			continue;

		if ( FNV1A::Hash( classInfo->szNname ) != FNV1A::HashConst( "CCSPlayerController" ) )
			continue;

		auto playerController{ static_cast< CCSPlayerController* >( ent ) };
		if ( !playerController->m_bPawnIsAlive( ) )
			continue;

		C_CSPlayerPawn* pawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( playerController->m_hPawn( ) ) };
		if ( !pawn )
			continue;

		PlayerEntry_t& entry{ ctx.m_mapPlayerEntries[ pawn->GetRefEHandle( ).m_nIndex ] };

		target.m_pEntry = &entry;
		target.m_pRecord = nullptr;
		target.m_iBestDamage = 0;

		target.GetBestLagRecord( entry );

		bestTarget.SelectBetterTarget( target );
	}

}

void CAimTarget::SelectBetterTarget( const CAimTarget& compareTarget ) {
	if ( compareTarget.m_iBestDamage > this->m_iBestDamage )
		*this = compareTarget;
}

void CAimTarget::GetBestLagRecord( PlayerEntry_t& entry ) {
	CLagRecord* oldestRecord{ };

	for ( auto& record : entry.Animations.m_vecLagRecords ) {
		if ( record.IsRecordValid( ) ) {
			oldestRecord = &record;
			break;
		}
	}

	if ( !oldestRecord )
		return;

}