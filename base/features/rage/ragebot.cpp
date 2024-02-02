#include "ragebot.h"
#include "autowall.h"
#include "../../utils/math.h"
#include "../../core/config.h"

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
		target.m_pEntry = &entry;
		target.m_pRecord = nullptr;
		target.m_iBestDamage = 0;

		target.GetBestLagRecord( entry );

		bestTarget.SelectBetterTarget( target );
	}

	if ( !bestTarget.m_pRecord )
		return;

	const auto gameSceneNode{ bestTarget.m_pEntry->m_pPawn->m_pGameSceneNode( ) };
	if ( !gameSceneNode )
		return;

	const auto skeleton{ gameSceneNode->GetSkeletonInstance( ) };
	if ( !skeleton )
		return;

	const auto hitboxSet{ bestTarget.m_pEntry->m_pPawn->GetHitboxSet( skeleton->m_nHitboxSet( ) ) };

	auto& model{ skeleton->m_modelState( ).m_hModel };
	if ( !model.IsValid( ) )
		return;

	auto& hitbox{ hitboxSet->m_arrHitboxs[ HITBOX_HEAD ] };
	const auto& bone{ hitbox.GetBoneIndex( skeleton->GetModel( ) ) };

	const auto point{ bestTarget.m_pRecord->m_arrBones[ bone ].m_vecPosition };

	const auto headPos{ ( local->m_vOldOrigin( ) + local->m_vecViewOffset( ) ) };

	Vector angle{ };
	Math::VectorAngles( point - headPos, angle );

	cmd->cmd.pBase->pViewangles->angValue = angle;

	cmd->cmd.nAttackStartHistoryIndex = 0;
	cmd->m_cButtonStates.m_iHeld |= IN_ATTACK;
	cmd->m_cButtonStates.m_iToggle |= IN_ATTACK;

	const auto sub_tick = cmd->cmd.inputHistoryField.pRep->tElements[ cmd->cmd.nAttackStartHistoryIndex ];

	if ( sub_tick->pTargetViewCmd )
		sub_tick->pTargetViewCmd->angValue = angle;
	if ( sub_tick->pViewCmd )
		sub_tick->pViewCmd->angValue = angle;

	auto& record{ bestTarget.m_pRecord };

	std::memcpy( ctx.DEBUGBacktrackBones, record->m_arrBones, sizeof( record->m_arrBones ) );

	cmd->cmd.pBase->nTickCount = record->m_nAddedTickCount;

	sub_tick->m_nSomeFlag |= 0x200u;
	sub_tick->m_nSomeFlag |= 0x400u;
	sub_tick->m_nSomeFlag |= 0x1u;
	sub_tick->nPlayerTickCount = record->nRenderTickCount + 1;
	sub_tick->flPlayerTickFraction = record->flRenderTickFraction + 0.0012f;

	sub_tick->nRenderTickCount = record->nRenderTickCount;
	sub_tick->flRenderTickFraction = record->flRenderTickFraction;

	auto v58 = ( void** ) ( sub_tick->m_nUnknown0 & 0xFFFFFFFFFFFFFFFCu );
	if ( ( sub_tick->m_nUnknown0 & 1 ) != 0 )
		v58 = ( void** ) *v58;

	sub_tick->m_nSomeFlag |= 0x100u;

	if ( !sub_tick->player_interp )
		sub_tick->player_interp = ( CCSGOInterpolationInfo* ) Displacement::InitCSGOInterpolationInfoPB( ( __int64 ) v58 );

	sub_tick->player_interp->m_nSomeFlag |= 2u;
	sub_tick->player_interp->m_nSomeFlag |= 4u;
	sub_tick->player_interp->m_nSomeFlag |= 1u;

	if ( !sub_tick->cl_interp )
		sub_tick->cl_interp = ( CCSGOInterpolationInfo* ) Displacement::InitCSGOInterpolationInfoPB( ( __int64 ) v58 );

	sub_tick->cl_interp->m_nSomeFlag |= 2u;
	sub_tick->cl_interp->m_nSomeFlag |= 4u;
	sub_tick->cl_interp->m_nSomeFlag |= 1u;

	sub_tick->m_nSomeFlag |= 0x40u;

	if ( !sub_tick->sv_interp0 )
		sub_tick->sv_interp0 = ( CCSGOInterpolationInfo* ) Displacement::InitCSGOInterpolationInfoPB( ( __int64 ) v58 );

	sub_tick->sv_interp0->m_nSomeFlag |= 2u;
	sub_tick->sv_interp0->m_nSomeFlag |= 4u;
	sub_tick->sv_interp0->m_nSomeFlag |= 1u;

	sub_tick->m_nSomeFlag |= 0x80u;

	if ( !sub_tick->sv_interp1 )
		sub_tick->sv_interp1 = ( CCSGOInterpolationInfo* ) Displacement::InitCSGOInterpolationInfoPB( ( __int64 ) v58 );

	sub_tick->sv_interp1->m_nSomeFlag |= 2u;
	sub_tick->sv_interp1->m_nSomeFlag |= 4u;
	sub_tick->sv_interp1->m_nSomeFlag |= 1u;

	//sub_tick->m_nSomeFlag |= 0x4000u;
	//sub_tick->nTargetEntIndex = bestTarget.m_pEntry->m_iIndex;// bestTarget.m_pEntry->m_pPawn->m_pEntity( )->Index( );

	//sub_tick->m_nSomeFlag |= 0x2000u;
	//sub_tick->nFrameNumber = ctx.m_nFrameNumber;


	if ( sub_tick->cl_interp && sub_tick->sv_interp0 && sub_tick->sv_interp1 && sub_tick->player_interp ) {
		sub_tick->cl_interp->nSrcTick = record->m_nAddedTickCount;
		sub_tick->cl_interp->nDstTick = record->m_nAddedTickCount + 1;
		sub_tick->cl_interp->flFraction = 0.f;

		sub_tick->player_interp->nSrcTick = record->m_nAddedTickCount + 1;
		sub_tick->player_interp->nDstTick = record->m_nAddedTickCount + 2;
		sub_tick->player_interp->flFraction = 0.f;

		sub_tick->sv_interp0->nSrcTick = record->m_nAddedTickCount - 1;
		sub_tick->sv_interp0->nDstTick = record->m_nAddedTickCount;
		sub_tick->sv_interp0->flFraction = 0.f;

		sub_tick->sv_interp1->nSrcTick = record->m_nAddedTickCount;
		sub_tick->sv_interp1->nDstTick = record->m_nAddedTickCount + 1;
		sub_tick->sv_interp1->flFraction = 0.f;
	}
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
	if ( damage > 50 ) {
		this->m_iBestDamage = damage;
		this->m_pRecord = oldestRecord;
	}

	return;

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

	auto& hitbox{ hitboxSet->m_arrHitboxs[ HITBOX_LEFT_THIGH ] };
	const auto& bone{ hitbox.GetBoneIndex( skeleton->GetModel( ) ) };

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

	const auto headPos{ ( localPawn->m_vOldOrigin( ) + localPawn->m_vecViewOffset( ) ) };
	const auto success{ Features::Penetration.FireBullet( headPos, point, localPawn, this->m_pEntry->m_pPawn, weaponData, data ) };

	//printf( "%Ii\n", hitboxSet->m_arrHitboxs[ 1 ].GetBoneIndex( skeleton->GetModel( ) ) );
	//printf( "\n" );

	//auto bone{ model->GetBoneFromHitbox( hitbox ) };

	//ctx.DEBUGBacktrackBones[ 6 ].m_vecPosition = skeleton->m_modelState( ).m_pBones[ bone ].m_vecPosition;

	//hitboxSet->m_arrHitbox[ 0 ].m_nBoneNameHash;

	return data.m_flDamage;
}

void CRageBot::SetupAttack( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	// rebuild of LagCompensation (called from createmove)
}