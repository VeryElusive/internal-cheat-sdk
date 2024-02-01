#include "ragebot.h"
#include "autowall.h"
#include "../../utils/math.h"
#include "../../core/config.h"

#include "../../sdk/fnv1a.h"

void SetSubTickAngles( CUserCmd* cmd, const Vector& Angles )
{
	auto SubTicks = cmd->cmd.inputHistoryField;
	for ( int i = 0; i < SubTicks.nCurrentSize; i++ )
	{
		auto* Tick = SubTicks.pRep->tElements[ i ];
		if ( Tick && Tick->pViewCmd )
		{
			Tick->pViewCmd->angValue = Angles;
		}
	}
}

void CRageBot::Main( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	/*	if ( !Config::Get<bool>( Vars.RagebotEnable )
		|| ctx.m_pWeapon->m_bReloading( )
		|| ( ctx.m_pWeaponData->nWeaponType >= WEAPONTYPE_C4
			&& ctx.m_pWeapon->m_iItemDefinitionIndex( ) != WEAPON_TASER )
		|| ctx.m_pLocal->m_MoveType( ) == MOVETYPE_LADDER )
		return;*/

	if ( !Configs::m_cConfig.m_bRageBotEnable )
		return;

	Configs::m_cConfig.m_bRageBotEnable = false;

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

	Vector angle{ };
	Math::VectorAngles( point - ( local->GetAbsOrigin( ) + local->m_vecViewOffset( ) ), angle );

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

	sub_tick->nPlayerTickCount = record->nRenderTickCount + 1;
	sub_tick->flPlayerTickFraction = record->flRenderTickFraction + 0.0012f;

	sub_tick->nRenderTickCount = record->nRenderTickCount;
	sub_tick->flRenderTickFraction = record->flRenderTickFraction;

	if ( sub_tick->cl_interp && sub_tick->sv_interp0 && sub_tick->sv_interp1 ) {
		sub_tick->cl_interp->nSrcTick = record->m_nAddedTickCount;
		sub_tick->cl_interp->nDstTick = record->m_nAddedTickCount + 1;
		sub_tick->cl_interp->flFraction = 0.f;

		sub_tick->sv_interp0->nSrcTick = record->m_nAddedTickCount - 1;
		sub_tick->sv_interp0->nDstTick = record->m_nAddedTickCount;
		sub_tick->sv_interp0->flFraction = 0.f;

		sub_tick->sv_interp1->nSrcTick = record->m_nAddedTickCount;
		sub_tick->sv_interp1->nDstTick = record->m_nAddedTickCount + 1;
		sub_tick->sv_interp1->flFraction = 0.f;
	}
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

	QuickScan( oldestRecord, hitgroups );

	this->m_pRecord = oldestRecord;
	this->m_iBestDamage = 1.f;
}

// rdi = skeleton
// rax = hitbox( hitboxSet + 0x18 )
// edx = rdi+rax+34h

float CAimTarget::QuickScan( CLagRecord* record, std::vector<int> hitgroups ) {
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
	const auto& bone{ hitbox.GetBoneIndex( skeleton->GetModel( ) ) };

	const auto point{ record->m_arrBones[ bone ].m_vecPosition };

	PenetrationData_t data{ };

	//const auto success{ Features::Penetration.FireBullet( localPawn->GetAbsOrigin( ), point, localPawn, this->m_pEntry->m_pPawn, ctx.m_pWeaponData,
	//	data ) };

	//printf( "%Ii\n", hitboxSet->m_arrHitboxs[ 1 ].GetBoneIndex( skeleton->GetModel( ) ) );
	//printf( "\n" );

	//auto bone{ model->GetBoneFromHitbox( hitbox ) };

	//ctx.DEBUGBacktrackBones[ 6 ].m_vecPosition = skeleton->m_modelState( ).m_pBones[ bone ].m_vecPosition;

	//hitboxSet->m_arrHitbox[ 0 ].m_nBoneNameHash;

	return 0.0f;
}