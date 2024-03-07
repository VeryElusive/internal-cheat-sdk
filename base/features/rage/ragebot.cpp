#include "ragebot.h"
#include "autowall.h"
#include "../../utils/math.h"
#include "../../utils/ray_tracer.h"
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
	m_bShouldStop = false;

	if ( !Configs::m_cConfig.m_bRageBotEnable
		|| !CanFire( local ) )
		return;

	CAimTarget bestTarget{ };
	CAimTarget target{ };

	for ( auto& e : ctx.m_mapPlayerEntries ) {
		auto& entry{ e.second };
		if ( !entry.m_pPawn )
			continue;

		if ( entry.m_pPawn->m_iTeamNum( ) == local->m_iTeamNum( ) )
			continue;

		target.m_pEntry = &entry;
		target.m_pRecord = nullptr;
		target.m_iBestDamage = 0;

		if ( !entry.Animations.m_vecLagRecords.size( ) )
			continue;

		target.GetBestLagRecord( entry );

		bestTarget.SelectBetterTarget( target );
	}

	if ( !bestTarget.m_pRecord )
		return;
	bestTarget.ScanTarget( local );

	AutoStop( local, cmd );

	bestTarget.Attack( local, cmd );
}

void CRageBot::AutoStop( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	if ( !Configs::m_cConfig.m_bRageBotAutostop )
		return;

	if ( !m_bShouldStop )
		return;

	if ( !( local->m_fFlags( ) & FL_ONGROUND ) )
		return;

	const auto weaponData{ ctx.GetWeaponData( ) };
	if ( !weaponData )
		return;

	//if ( Features::Ragebot.MenuVars.RagebotAutostopInAir &&
	//	( /*ctx.m_pLocal->m_vecVelocity( ).z > 100.f ||*/ ctx.m_pLocal->m_vecVelocity( ).z < -5.f ) )
	//	return;
	//else if ( !Features::Ragebot.MenuVars.RagebotAutostopInAir && !( ctx.m_pLocal->m_fFlags( ) & FL_ONGROUND ) )
	//	return;

	if ( Configs::m_cConfig.m_bRageBotAutostopMoveBetweenShots && !CanFire( local ) )
		return;

	const auto maxWeaponSpeed{ weaponData->m_flMaxSpeed( ) };
	auto optSpeed{ maxWeaponSpeed * 0.25f };

	if ( !( local->m_fFlags( ) & FL_ONGROUND ) )
		optSpeed = 0.f;

	Features::Movement.LimitSpeed( local, cmd, optSpeed );
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

void SetupHitboxes( std::vector< int >& hitboxes ) {
	int hitboxCount{ };
	for ( int i{ }; i < 8; ++i ) {
		if ( Configs::m_cConfig.m_bRageBotHitboxes[ i ] ) {
			if ( i == 5 )
				hitboxCount += 4;
			else if ( i == 6 || i == 7 )
				hitboxCount += 2;
			else
				++hitboxCount;
		}
	}

	hitboxes.resize( hitboxCount );

	size_t index { };
	if ( Configs::m_cConfig.m_bRageBotHitboxes[ 1 ] )
		hitboxes[ index++ ] = HITBOX_UPPER_CHEST;
	if ( Configs::m_cConfig.m_bRageBotHitboxes[ 2 ] )
		hitboxes[ index++ ] = HITBOX_CHEST;
	if ( Configs::m_cConfig.m_bRageBotHitboxes[ 3 ] )
		hitboxes[ index++ ] = HITBOX_LOWER_CHEST;
	if ( Configs::m_cConfig.m_bRageBotHitboxes[ 4 ] )
		hitboxes[ index++ ] = HITBOX_STOMACH;
	if ( Configs::m_cConfig.m_bRageBotHitboxes[ 5 ] ) {
		hitboxes[ index++ ] = HITBOX_RIGHT_UPPER_ARM;
		hitboxes[ index++ ] = HITBOX_RIGHT_FOREARM;
		hitboxes[ index++ ] = HITBOX_LEFT_UPPER_ARM;
		hitboxes[ index++ ] = HITBOX_LEFT_FOREARM;
	}
	if ( Configs::m_cConfig.m_bRageBotHitboxes[ 6 ] ) {
		hitboxes[ index++ ] = HITBOX_RIGHT_CALF;
		hitboxes[ index++ ] = HITBOX_LEFT_CALF;
	}
	if ( Configs::m_cConfig.m_bRageBotHitboxes[ 7 ] ) {
		hitboxes[ index++ ] = HITBOX_RIGHT_FOOT;
		hitboxes[ index++ ] = HITBOX_LEFT_FOOT;
	}

	if ( Configs::m_cConfig.m_bRageBotHitboxes[ 0 ] )
		hitboxes[ index++ ] = HITBOX_HEAD;

}

static int pointz{ };

void CAimTarget::ScanTarget( C_CSPlayerPawn* local ) {
	std::vector< int > hitboxes{ };
	SetupHitboxes( hitboxes );

	const auto gameSceneNode{ this->m_pEntry->m_pPawn->m_pGameSceneNode( ) };
	if ( !gameSceneNode )
		return;

	const auto skeleton{ gameSceneNode->GetSkeletonInstance( ) };
	if ( !skeleton )
		return;

	const auto hitboxSet{ this->m_pEntry->m_pPawn->GetHitboxSet( skeleton->m_nHitboxSet( ) ) };
	if ( !hitboxSet
		|| !hitboxSet->m_arrHitboxs )
		return;

	auto& model{ skeleton->m_modelState( ).m_hModel };
	if ( !model.IsValid( ) )
		return;

	const auto weaponData{ ctx.GetWeaponData( ) };
	if ( !weaponData )
		return;

	const auto mindmg{ ScaleMinDamage( static_cast< float >( std::max( Configs::m_cConfig.m_iRageBotMinimumPenetrationDamage, Configs::m_cConfig.m_iRageBotMinimumDamage ) ) ) };

	std::vector< Vector > points{ };

	pointz = 0;
	for ( int i{ }; i < 32; ++i ) {
		ctx.DEBUGPointPrintout[ i ] = { 0,0,0 };
	}

	for ( const auto& hb : hitboxes ) {
		auto& hitbox{ hitboxSet->m_arrHitboxs[ hb ] };
		const auto boneIndex{ hitbox.GetBoneIndex( skeleton->GetModel( ) ) };

		// TODO: prefer baim conditions
		//if ( hb == HITBOX_HEAD && this->m_cPoint.m_bValid ) {

		//}

		auto damage{ static_cast< float >( weaponData->m_nDamage( ) ) };
		Features::Penetration.ScaleDamage( hitbox.m_nGroupId, local, weaponData, damage );
		if ( damage < mindmg )
			continue;

		int scale{ ( hb != HITBOX_HEAD ? Configs::m_cConfig.m_iRageBotBodyPointScale : Configs::m_cConfig.m_iRageBotHeadPointScale ) };
		if ( !Configs::m_cConfig.m_bRageBotStaticPoints ) {
			// TODO:
			scale = 100;
		}

		if ( scale < 1 )
			scale = 1;

		const auto scaleFloat{ static_cast< float >( scale ) / 100.f };
		const auto bone{ this->m_pRecord->m_arrBones[ boneIndex ] };

		ScanHitbox( local, weaponData, bone, hitbox, hb, scaleFloat, hitboxes );
	}
}

Vector rotateAroundCenter( const Vector& center, float radius, float angleDegrees, Vector4D rotation ) {
	// Convert angle from degrees to radians
	float angleRadians = angleDegrees * M_PI / 180.0;

	// Calculate new position
	float newX = center.x + radius * cos( angleRadians + rotation.y * M_PI / 180.0 );
	float newY = center.y + radius * sin( angleRadians + rotation.y * M_PI / 180.0 );
	float newZ = center.z + rotation.z; // Add rotation around Z-axis

	return { newX, newY, newZ };
}

// TODO:
void CAimTarget::ScanHitbox( 
	const C_CSPlayerPawn* local, const CCSWeaponBaseVData* weaponData, 
	const CBoneData bone, const CHitbox& hitbox, const int hitboxIndex, 
	const float scale, std::vector< int > hitboxes
) {
	CLagBackup backup{ this->m_pEntry->m_pPawn };
	this->m_pRecord->Apply( this->m_pEntry->m_pPawn );

	const auto radius{ hitbox.m_flShapeRadius * scale };

	const auto center{ bone.m_vecPosition + ( ( hitbox.m_vecMins + hitbox.m_vecMaxs ) / 2.f ) };

	Vector angleForward{ };
	Math::VectorAngles( center - local->GetAbsOrigin( ), angleForward );

	Vector angleTmp{ };

	ScanPoint( local, weaponData, bone.m_vecPosition, hitbox, hitboxes );


	switch ( hitboxIndex ) {
	case HITBOX_HEAD:

		// top.
		/*ScanPoint(
			{ bone.m_vecPosition.x + hitbox.m_vecMaxs.x,
			  bone.m_vecPosition.y + hitbox.m_vecMaxs.y,
			  bone.m_vecPosition.z + hitbox.m_vecMaxs.z + r
			},
			hitbox, hitboxes
		);

		// left.
		angleTmp = angleForward;
		angleTmp.y += 90.f;

		ScanPoint( local, weaponData,
			rotateAroundCenter( center, radius, angleTmp.y, bone.m_vecRotation ),
			hitbox, hitboxes );

		// right.
		angleTmp = angleForward;
		angleTmp.y -= 90.f;

		ScanPoint( local, weaponData,
			rotateAroundCenter( center, radius, angleTmp.y, bone.m_vecRotation ),
			hitbox, hitboxes ); */

		/*ScanPoint( {
				bone.m_vecPosition.x + hitbox.m_vecMaxs.x,
				bone.m_vecPosition.y + hitbox.m_vecMaxs.y,
				bone.m_vecPosition.z + hitbox.m_vecMaxs.z - r
			},
			hitbox, hitboxes
		);*/

		// left.
		/*ScanPoint(
			{ bone.m_vecPosition.x,
			  bone.m_vecPosition.y + ( hitbox.m_flShapeRadius * scale ),
			  bone.m_vecPosition.z
			},
			hitbox, hitboxes
		);

		// right.
		ScanPoint(
			{ bone.m_vecPosition.x,
			  bone.m_vecPosition.y - ( hitbox.m_flShapeRadius * scale ),
			  bone.m_vecPosition.z
			},
			hitbox, hitboxes
		);*/

		// bottom.
		/*ScanPoint(
			{ bone.m_vecPosition.x + hitbox.m_vecMaxs.x,
			  bone.m_vecPosition.y + hitbox.m_vecMaxs.y,
			  bone.m_vecPosition.z + hitbox.m_vecMaxs.z - r
			},
			hitbox, hitboxes
		);*/

		// left.
		/*ScanPoint(
			{ bone.m_vecPosition.x + hitbox.m_vecMaxs.x,
			  bone.m_vecPosition.y + hitbox.m_vecMaxs.y,
			  bone.m_vecPosition.z + hitbox.m_vecMaxs.z - r
			},
			hitbox, hitboxes
		);

		// back.
		ScanPoint(
			{ bone.m_vecPosition.x + hitbox.m_vecMaxs.x,
			  bone.m_vecPosition.y + hitbox.m_vecMaxs.y - r,
			  bone.m_vecPosition.z + hitbox.m_vecMaxs.z
			},
			hitbox, hitboxes
		);*/

		// top/back 45 deg.
		// this is the best spot to shoot at.
		/*ScanPoint(
			{ bone.m_vecPosition.x + hitbox.m_vecMaxs.x + ( rotation * r ),
			  bone.m_vecPosition.y + hitbox.m_vecMaxs.y + ( -rotation * r ),
			  bone.m_vecPosition.z + hitbox.m_vecMaxs.z
			},
			hitbox, hitboxes
		);*/
		break;
	//case HITGROUP_CHEST:
	//	ScanPoint( bone.m_vecPosition, hitbox, hitboxes );
	default: break;
	}

	backup.Apply( this->m_pEntry->m_pPawn );
}

float CAimTarget::ScaleMinDamage( float damage ) {
	if ( Configs::m_cConfig.m_bRageBotOverrideDamage
		&& Configs::m_cConfig.m_kRageBotOverrideDamage.m_bEnabled )
		damage = Configs::m_cConfig.m_iRageBotMinimumOverrideDamage;
	else if ( Configs::m_cConfig.m_bRageBotScaleDamage )
		damage *= this->m_pEntry->m_pPawn->m_iHealth( ) / 100.f;

	return damage;
}

void CAimTarget::ScanPoint( 
	const C_CSPlayerPawn* local, const CCSWeaponBaseVData* weaponData, 
	const Vector point, const CHitbox& hitbox, std::vector<int> hitboxes ) {
	if ( pointz < 32 )
		ctx.DEBUGPointPrintout[ pointz ] = point;
	++pointz;

	const auto headPos{ ( local->GetAbsOrigin( ) + local->m_vecViewOffset( ) ) };

	PenetrationData_t data{ };

	float minDmgCap{ ScaleMinDamage( static_cast< float >( Configs::m_cConfig.m_iRageBotMinimumPenetrationDamage ) ) };
	if ( Configs::m_cConfig.m_bRageBotAutowall )
		minDmgCap = FLT_MAX;

	const auto success{
			Features::Penetration.FireBullet( headPos, point, local, this->m_pEntry->m_pPawn, weaponData, data, minDmgCap )
	};
	if ( !success )
		return;

	const auto mindmg{ ScaleMinDamage( ( data.m_bPenetrated ? Configs::m_cConfig.m_iRageBotMinimumPenetrationDamage : Configs::m_cConfig.m_iRageBotMinimumDamage ) ) };

	if ( data.m_flDamage < mindmg )
		return;

	Features::RageBot.m_bShouldStop = true;

	if ( !this->m_cPoint.m_bValid ) {
		this->m_cPoint.m_bValid = true;
		//goto UPDATE_TARGET_POINT;
	}
	
	if ( data.m_flDamage <= this->m_cPoint.m_flDamage )
		return;

UPDATE_TARGET_POINT:
	this->m_cPoint.m_flDamage = data.m_flDamage;
	this->m_cPoint.m_iHitgroup = data.m_iHitGroup;// TODO: MAKE ENTIRE RAGEBOT RUN OFF HITBOX
	this->m_cPoint.m_vecPoint = point;
}

void CAimTarget::GetBestLagRecord( PlayerEntry_t& entry ) {

	//this->m_pRecord = &entry.Animations.m_vecLagRecords.back( ); this->m_iBestDamage = 100;
	//return;

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
	if ( damage > 0 ) {
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

		if ( damage > this->m_iBestDamage ) {
			this->m_iBestDamage = damage;
			this->m_pRecord = &record;
		}
	}
}

float CAimTarget::QuickScan( const CLagRecord* record, std::vector<int> hitgroups ) {
	const auto localPawn{ ctx.GetLocalPawn( ) };
	if ( !localPawn )
		return 0.f;

	const auto gameSceneNode{ this->m_pEntry->m_pPawn->m_pGameSceneNode( ) };
	if ( !gameSceneNode )
		return 0.f;

	const auto skeleton{ gameSceneNode->GetSkeletonInstance( ) };
	if ( !skeleton )
		return 0.f;

	const auto hitboxSet{ this->m_pEntry->m_pPawn->GetHitboxSet( skeleton->m_nHitboxSet( ) ) };
	if ( !hitboxSet )
		return 0.f;

	auto& model{ skeleton->m_modelState( ).m_hModel };
	if ( !model.IsValid( ) )
		return 0.f;

	const auto weaponData{ ctx.GetWeaponData( ) };
	if ( !weaponData )
		return 0.f;

	int dmg{ };

	const auto headPos{ ( localPawn->GetAbsOrigin( ) + localPawn->m_vecViewOffset( ) ) };

	CLagBackup backup{ this->m_pEntry->m_pPawn };
	record->Apply( this->m_pEntry->m_pPawn );

	for ( const auto& hb : hitgroups ) {
		auto& hitbox{ hitboxSet->m_arrHitboxs[ hb ] };
		const auto bone{ hitbox.GetBoneIndex( skeleton->GetModel( ) ) };
		if ( bone > 128 || bone < 0 )
			continue;

		const auto point{ record->m_arrBones[ bone ].m_vecPosition };

		PenetrationData_t data{ };


		const auto success{
			Features::Penetration.FireBullet( headPos, point, localPawn, this->m_pEntry->m_pPawn, weaponData, data )
		};
		if ( !success )
			continue;

		if ( data.m_flDamage > dmg )
			dmg = data.m_flDamage;

		if ( data.m_iHitGroup == HITGROUP_HEAD )
			break;
	}

	backup.Apply( this->m_pEntry->m_pPawn );

	return dmg;
}

void CAimTarget::Attack( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	// rebuild of LagCompensation (called from createmove)
	if ( !this->m_cPoint.m_bValid )
		return;

	const auto weaponData{ ctx.GetWeaponData( ) };
	if ( !weaponData )
		return;

	const auto headPos{ ( local->GetAbsOrigin( ) + local->m_vecViewOffset( ) ) };

	Vector angle{ };
	Math::VectorAngles( this->m_cPoint.m_vecPoint - headPos, angle );

	Vector fwd{ }, right{ }, up{ };
	Math::AngleVectors( angle, &fwd, &right, &up );

	int totalHits{ };

	const auto weaponServices{ local->m_pWeaponServices( ) };
	if ( !weaponServices )
		return;

	const auto weapon{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSWeaponBase>( weaponServices->m_hActiveWeapon( ) ) };
	if ( !weapon )
		return;

	const auto spread{ weapon->GetSpread( ) };
	const auto inaccuracy{ weapon->GetInaccuracy( ) };

	CLagBackup backup{ this->m_pEntry->m_pPawn };
	this->m_pRecord->Apply( this->m_pEntry->m_pPawn );

	for ( int i{ }; i < 256; ++i ) {
		Math::RandomSeed( i );

		auto v1 = Math::RandomFloat( 0.f, 1.f );
		auto v2 = Math::RandomFloat( 0.f, M_2PI );

		auto v3 = Math::RandomFloat( 0.f, 1.f );
		auto v4 = Math::RandomFloat( 0.f, M_2PI );

		/*if ( recoil_index < 3.f && item_index == WEAPON_NEGEV ) {
			for ( auto i = 3; i > recoil_index; --i ) {
				v1 *= v1;
				v3 *= v3;
			}

			v1 = 1.f - v1;
			v3 = 1.f - v3;
		}

		if ( item_index == WEAPON_REVOLVER ) {
			v1 = 1.f - v1 * v1;
			v3 = 1.f - v3 * v3;
		}*/

		const auto inac = v1 * inaccuracy;
		const auto spre = v3 * spread;

		const auto dir{ ( fwd + 
			right * ( std::cos( v2 ) * inac + std::cos( v4 ) * spre ) + 
			up * ( std::sin( v2 ) * inac + std::sin( v4 ) * spre ) 
			).Normalized( ) };

		PenetrationData_t data{ };
		const auto success{
			Features::Penetration.FireBullet( headPos, headPos + dir * weaponData->m_flRange( ), local, this->m_pEntry->m_pPawn, weaponData, data )
		};
		if ( !success )
			continue;

		totalHits++;
	}

	backup.Apply( this->m_pEntry->m_pPawn );

	if ( static_cast< int >( totalHits / 256.f * 100.f ) < Configs::m_cConfig.m_iRageBotHitchance )
		return;

	std::memcpy( ctx.DEBUGBacktrackBones, this->m_pRecord->m_arrBones, sizeof( this->m_pRecord->m_arrBones ) );
	ctx.DEBUGBactrackEntry = this->m_pEntry;

	// TODO: this->m_pPoint->m_vecPoint
	Features::RageBot.m_cData.m_vecPoint = this->m_cPoint.m_vecPoint;// this->m_pPoint->m_vecPoint;
	Features::RageBot.m_cData.m_pRecord = this->m_pRecord;
	Features::RageBot.m_cData.m_pEntry = this->m_pEntry;
	Features::RageBot.m_cData.m_bValid = true;

	// TODO: CHECK IF THE LAST FRAME THE BUTTON WAS DOWN
	Interfaces::Input->AddButton( IN_ATTACK );

	cmd->m_cButtonStates.m_iHeld |= IN_ATTACK;
}

struct LockAngle_t {
	int slot;
	Vector angle;
};

LockAngle_t lockAngle;

void CRageBot::PostCMove( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	if ( !m_cData.m_bValid ) {
		if ( lockAngle.slot == Interfaces::Input->m_nSequenceNumber )
			cmd->pBase->pViewangles->angValue = lockAngle.angle;

		return;
	}

	m_cData.m_bValid = false;

	const auto headPos{ ( local->GetAbsOrigin( ) + local->m_vecViewOffset( ) ) };

	Vector angle{ };
	Math::VectorAngles( m_cData.m_vecPoint - headPos, angle );

	auto& record{ m_cData.m_pRecord };

	cmd->pBase->pViewangles->angValue = angle;
	cmd->pBase->nTickCount = record->m_nPlayerTickCount;

	lockAngle.angle = angle;
	lockAngle.slot = Interfaces::Input->m_nSequenceNumber;

	if ( !Configs::m_cConfig.m_bRageBotSilentAim )
		Interfaces::Input->SetViewAngles( angle );

	const auto backupTick{ Interfaces::Input->m_pSubTickData->m_iPlayerTick };
	const auto backupFrac{ Interfaces::Input->m_pSubTickData->m_flPlayerTickFraction };

	Interfaces::Input->m_pSubTickData->m_iPlayerTick = record->m_nPlayerTickCount;
	Interfaces::Input->m_pSubTickData->m_flPlayerTickFraction = 0.f;// record->m_flPlayerTickFraction;

	//if ( record->m_flPlayerTickFraction > 0.01f )
	//	++Interfaces::Input->m_pSubTickData->m_iPlayerTick;

	struct InterpData_t {
		int nSrcTick = -1;
		int nDstTick = -1;
		float flFraction = 0.f;
	};

	InterpData_t cl_interp, sv_interp0, sv_interp1;
	Displacement::SetupInterpolationFractions( m_cData.m_pEntry->m_pPawn->m_pGameSceneNode( ), &cl_interp, &sv_interp0, &sv_interp1, &Interfaces::Input->m_pSubTickData->m_iPlayerTick );

	Interfaces::Input->m_pSubTickData->m_iPlayerTick = backupTick;
	Interfaces::Input->m_pSubTickData->m_flPlayerTickFraction = backupFrac;

	//cmd->m_cButtonStates.m_iHeld |= IN_ATTACK;
	//cmd->m_cButtonStates.m_iToggle |= IN_ATTACK;

	const auto subTick{ cmd->inputHistoryField.pRep->tElements[ cmd->m_nStartAttackHistoryIndex ] };

	//const auto subTickAttack{ cmd->cmd.inputHistoryField.pRep->tElements[ cmd->cmd.nAttackStartHistoryIndex ] };

	if ( subTick->pTargetViewCmd )
		subTick->pTargetViewCmd->angValue = angle;
	if ( subTick->pViewCmd )
		subTick->pViewCmd->angValue = angle;

	subTick->nRenderTickCount = record->nRenderTickCount;
	subTick->flRenderTickFraction = record->flRenderTickFraction;

	if ( subTick->cl_interp ) {
		subTick->cl_interp->nSrcTick = cl_interp.nSrcTick;
		//subTick->cl_interp->nDstTick = cl_interp.nDstTick;
		subTick->cl_interp->flFraction = cl_interp.flFraction;
	}

	if ( subTick->sv_interp0 ) {
		subTick->sv_interp0->nSrcTick = sv_interp0.nSrcTick;
		//subTick->sv_interp0->nDstTick = sv_interp0.nDstTick;
		subTick->sv_interp0->flFraction = sv_interp0.flFraction;
	}
	if ( subTick->sv_interp1 ) {
		subTick->sv_interp1->nSrcTick = sv_interp1.nSrcTick;
		//subTick->sv_interp1->nDstTick = sv_interp1.nDstTick;
		subTick->sv_interp1->flFraction = sv_interp1.flFraction;
	}
}