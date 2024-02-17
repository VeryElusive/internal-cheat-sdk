#include "ragebot.h"
#include "autowall.h"
#include "../../utils/math.h"
#include "../../core/config.h"
#include "../../sdk/valve/entity.h"

#include "../../sdk/fnv1a.h"

void CRageBot::Main( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	/*	if ( !Config::Get<bool>( Vars.RagebotEnable )
		|| ctx.m_pWeapon->m_bReloading( )
		|| ( ctx.m_pWeaponData->nWeaponType >= WEAPONTYPE_C4
			&& ctx.m_pWeapon->m_iItemDefinitionIndex( ) != WEAPON_TASER )
		|| ctx.m_pLocal->m_MoveType( ) == MOVETYPE_LADDER )
		return;*/

	if ( !Configs::m_cConfig.m_bRageBotEnable
		|| !CanFire( local ) )
		return;

	CAimTarget bestTarget{ };
	CAimTarget target{ };

	for ( auto& e : ctx.m_mapPlayerEntries ) {
		auto& entry{ e.second };
		if ( !entry.m_pPawn )
			continue;

		target.m_pEntry = &entry;
		target.m_pRecord = nullptr;
		target.m_iBestDamage = 0;

		target.GetBestLagRecord( entry );

		bestTarget.SelectBetterTarget( target );
	}

	if ( !bestTarget.m_pRecord )
		return;

	bestTarget.Attack( local, cmd );
}

bool CRageBot::CanFire( C_CSPlayerPawn* local ) {
	const auto weaponServices{ local->m_pWeaponServices( ) };
	if ( !weaponServices )
		return false;

	const auto weapon{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSWeaponBase>( weaponServices->m_hActiveWeapon( ) ) };
	if ( !weapon )
		return false;

	if ( local->m_bWaitForNoAttack( ) )
		return false;

	if ( local->m_bIsDefusing( ) )
		return false;

	if ( weapon->m_bInReload( ) )
		return false;

	if ( !local->m_pWeaponServices( ) )
		return false;

	if ( Interfaces::GlobalVars->m_flCurTime < local->m_pWeaponServices( )->m_flNextAttack( ) )
		return false;

	if ( !weapon->m_AttributeManager( ) )
		return false;

	if ( !weapon->m_AttributeManager( )->m_Item( ) )
		return false;

	const auto& idx{ weapon->m_AttributeManager( )->m_Item( )->m_iItemDefinitionIndex( ) };

	if ( idx == WEAPON_C4 )
		return false;

	if ( Interfaces::GlobalVars->m_nTickCount < weapon->m_nNextPrimaryAttackTick( ) )
		return false;

	return true;
}

void CAimTarget::SelectBetterTarget( const CAimTarget& compareTarget ) {
	if ( !compareTarget.m_pRecord )
		return;

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

	std::vector <int> hitgroups{ HITBOX_HEAD, HITBOX_RIGHT_UPPER_ARM, HITBOX_LEFT_UPPER_ARM, HITBOX_RIGHT_FOOT, HITBOX_LEFT_FOOT };//HITBOX_STOMACH

	const auto damage{ QuickScan( oldestRecord, hitgroups ) };
	if ( damage > 100 ) {
		this->m_iBestDamage = damage;
		this->m_pRecord = oldestRecord;
	}

	//return;

	for ( auto it{ entry.Animations.m_vecLagRecords.rbegin( ) }; it != entry.Animations.m_vecLagRecords.rend( ); it = std::next( it ) ) {
		auto& record{ *it };

		if ( &record == oldestRecord )
			break;

		if ( !record.IsRecordValid( ) )
			continue;

		const auto damage{ this->QuickScan( &record, hitgroups ) };
		if ( damage <= 0 ) {
			if ( !this->m_pRecord )
				break;

			continue;
		}

		if ( damage > 100 ) {
			this->m_iBestDamage = damage;
			this->m_pRecord = &record;
		}
	}
}

float CAimTarget::QuickScan( const CLagRecord* record, std::vector<int> hitgroups ) {
	const auto localPawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };
	if ( !localPawn )
		return 0.f;

	const auto gameSceneNode{ this->m_pEntry->m_pPawn->m_pGameSceneNode( ) };
	if ( !gameSceneNode )
		return 0.f;

	const auto skeleton{ gameSceneNode->GetSkeletonInstance( ) };
	if ( !skeleton )
		return 0.f;

	const auto hitboxSet{ this->m_pEntry->m_pPawn->GetHitboxSet( skeleton->m_nHitboxSet( ) ) };

	auto& model{ skeleton->m_modelState( ).m_hModel };
	if ( !model.IsValid( ) )
		return 0.f;

	auto& hitbox{ hitboxSet->m_arrHitboxs[ HITBOX_HEAD ] };
	const auto& bone{ hitbox.GetBoneIndex( model.operator CModel * ( ) ) };

	const auto point{ record->m_arrBones[ bone ].m_vecPosition };

	PenetrationData_t data{ };

	const auto weaponServices{ localPawn->m_pWeaponServices( ) };
	if ( !weaponServices )
		return 0.f;

	const auto weapon{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSWeaponBase>( weaponServices->m_hActiveWeapon( ) ) };
	if ( !weapon )
		return 0.f;

	const auto weaponData{ weapon->m_pWeaponData( ) };
	if ( !weaponData )
		return 0.f;

	CLagBackup backup{ this->m_pEntry->m_pPawn };

	record->Apply( this->m_pEntry->m_pPawn );

	const auto headPos{ ( localPawn->m_vOldOrigin( ) + localPawn->m_vecViewOffset( ) ) };
	const auto success{ Features::Penetration.FireBullet( headPos, point, localPawn, this->m_pEntry->m_pPawn, weaponData, data ) };

	backup.Apply( this->m_pEntry->m_pPawn );

	return data.m_flDamage;
}

void CAimTarget::Attack( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	// rebuild of LagCompensation (called from createmove)

	const auto gameSceneNode{ this->m_pEntry->m_pPawn->m_pGameSceneNode( ) };
	if ( !gameSceneNode )
		return;

	const auto skeleton{ gameSceneNode->GetSkeletonInstance( ) };
	if ( !skeleton )
		return;

	const auto hitboxSet{ this->m_pEntry->m_pPawn->GetHitboxSet( skeleton->m_nHitboxSet( ) ) };

	auto& model{ skeleton->m_modelState( ).m_hModel };
	if ( !model.IsValid( ) )
		return;

	auto& hitbox{ hitboxSet->m_arrHitboxs[ HITBOX_HEAD ] };
	const auto& bone{ hitbox.GetBoneIndex( skeleton->GetModel( ) ) };

	const auto point{ this->m_pRecord->m_arrBones[ bone ].m_vecPosition };

	std::memcpy( ctx.DEBUGBacktrackBones, this->m_pRecord->m_arrBones, sizeof( this->m_pRecord->m_arrBones ) );

	// TODO: this->m_pPoint->m_vecPoint
	Features::RageBot.m_cData.m_vecPoint = point;// this->m_pPoint->m_vecPoint;
	Features::RageBot.m_cData.m_pRecord = this->m_pRecord;
	Features::RageBot.m_cData.m_pEntry = this->m_pEntry;
	Features::RageBot.m_cData.m_bValid = true;

	Interfaces::Input->AddButton( IN_ATTACK );

	//cmd->m_cButtonStates.m_iHeld |= IN_ATTACK;
}

int cl_interp_SrcTick = -1, sv_interp0_SrcTick = -1, sv_interp1_SrcTick = -1;

void CRageBot::PostCMove( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	if ( !m_cData.m_bValid )
		return;

	m_cData.m_bValid = false;

	const auto headPos{ ( local->m_vOldOrigin( ) + local->m_vecViewOffset( ) ) };

	Vector angle{ };
	Math::VectorAngles( m_cData.m_vecPoint - headPos, angle );

	auto& record{ m_cData.m_pRecord };

	cmd->pBase->pViewangles->angValue = angle;
	//cmd->pBase->nTickCount = record->m_nPlayerTickCount;

	static auto loc{ Memory::FindPattern( CLIENT_DLL, "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC 70 48 8B F9 48 8D 44 24" ) };

	using fnFunc = bool( __fastcall* ) ( void*, void*, void*, void*, void* );
	auto func = ( fnFunc )( loc );

	int v35 = record->m_nPlayerTickCount;

	cl_interp_SrcTick = -1, sv_interp0_SrcTick = -1, sv_interp1_SrcTick = -1;

	if ( record->m_flPlayerTickFraction > 0.0099999998f )
		v35 += 1;
	func( m_cData.m_pEntry->m_pPawn->m_pGameSceneNode( ), &cl_interp_SrcTick, &sv_interp0_SrcTick, &sv_interp1_SrcTick, &v35 );


	for ( int i{ }; i < cmd->cmd.m_iSize; ++i ) {
		const auto subTick{ cmd->cmd.GetInputHistoryEntry( i ) };


		// magic bullet/rapidfire occured when i set cmd->cmd.nAttackStartHistoryIndex to the wrong index

		//const auto subTickAttack{ cmd->cmd.inputHistoryField.pRep->tElements[ cmd->cmd.nAttackStartHistoryIndex ] };

		//cmd->m_cButtonStates.m_iHeld |= IN_ATTACK;
		//cmd->m_cButtonStates.m_iToggle |= IN_ATTACK;

		if ( subTick->pTargetViewCmd )
			subTick->pTargetViewCmd->angValue = angle;
		if ( subTick->pViewCmd )
			subTick->pViewCmd->angValue = angle;


		// this is used to calculate if you can shoot. it gets clamped between 2 ticks of the server tickcount.
		//subTick->nPlayerTickCount = ctx.m_iRenderTick + 1;
		//subTick->flPlayerTickFraction = ctx.m_flRenderTickFraction + 0.0012f;

		subTick->nRenderTickCount = record->nRenderTickCount;
		subTick->flRenderTickFraction = record->flRenderTickFraction;

		if ( subTick->cl_interp ) {
			subTick->cl_interp->nSrcTick = cl_interp_SrcTick;
			subTick->cl_interp->nDstTick = cl_interp_SrcTick + 1;
			subTick->cl_interp->flFraction = record->flRenderTickFraction;
		}

		if ( subTick->sv_interp0 ) {
			subTick->sv_interp0->nSrcTick = sv_interp0_SrcTick;
			subTick->sv_interp0->nDstTick = sv_interp0_SrcTick + 1;
			subTick->sv_interp0->flFraction = record->flRenderTickFraction;
		}
		if ( subTick->sv_interp1 ) {
			subTick->sv_interp1->nSrcTick = sv_interp1_SrcTick;
			subTick->sv_interp1->nDstTick = sv_interp1_SrcTick + 1;
			subTick->sv_interp1->flFraction = record->flRenderTickFraction;
		}
	}
}