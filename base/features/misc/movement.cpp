#include "movement.h"
#include "../rage/ragebot.h"
#include "../../core/config.h"
#include "../../utils/math.h"
#include "../../sdk/valve/interfaces/ienginecvar.h"

void CMovement::Main( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	if ( local->m_MoveType( ) != MOVETYPE_WALK )
		return;

	if ( Configs::m_cConfig.m_bBunnyhop ) {
		if ( local->m_fFlags( ) & FL_ONGROUND )
			cmd->m_cButtonStates.m_iHeld &= ~IN_JUMP;
	}

	if ( Configs::m_cConfig.m_bAutoStrafer )
		AutoStrafer( local, cmd );

	if ( Configs::m_cConfig.m_bSlowWalk && Configs::m_cConfig.m_kSlowWalk.m_bEnabled ) {
		const auto weaponData{ ctx.GetWeaponData( ) };
		if ( !weaponData )
			return;

		LimitSpeed( local, cmd, weaponData->m_flMaxSpeed( ) / 3.f );
	}
	//ctx.m_flUpmove = cmd->pBase->flUpMove;
}

void CMovement::AutoStrafer( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	if ( local->m_fFlags( ) & FL_ONGROUND )
		return;

	if ( !cmd->pBase
		|| !cmd->pBase->pViewangles )
		return;

	const auto velocity{ local->m_vecAbsVelocity( ) };
	const auto speed2D{ velocity.Length2D( ) };
	if ( speed2D < 2.f
		&& !cmd->pBase->flForwardMove && !cmd->pBase->flSideMove )
		return;

	const auto idealRot{ std::min( Math::RadiansToDegree( std::asinf( 15.f / speed2D ) ), 90.f ) };
	const auto sign{ cmd->pBase->nCommandNumber % 2 ? 1.f : -1.f };

	bool move_forward = cmd->m_cButtonStates.m_iHeld & IN_FORWARD, move_backward = cmd->m_cButtonStates.m_iHeld & IN_BACK;
	bool move_left = cmd->m_cButtonStates.m_iHeld & IN_MOVELEFT, move_right = cmd->m_cButtonStates.m_iHeld & IN_MOVERIGHT;

	cmd->pBase->flForwardMove = speed2D > 0.1f ? 0.f : 1.f;

	auto movementAngle{ cmd->pBase->pViewangles->angValue };

	if ( move_forward )
		movementAngle.y += move_left ? 45.f : move_right ? -45.f : 0.f;
	else if ( move_backward )
		movementAngle.y += move_left ? 135.f : move_right ? -135.f : 180.f;
	else if ( move_left || move_right )
		movementAngle.y += move_left ? 90.f : -90.f;

	static auto old_yaw = 0.f;
	auto yaw_delta = std::remainder( movementAngle.y - old_yaw, 360.f ), abs_yaw_delta = std::abs( yaw_delta );
	old_yaw = movementAngle.y;

	if ( yaw_delta > 0.f ) cmd->pBase->flSideMove = -1.f;
	else if ( yaw_delta < 0.f ) cmd->pBase->flSideMove = 1.f;

	if ( abs_yaw_delta <= idealRot || abs_yaw_delta >= 30.f ) {
		const auto vel_ang = Math::RadiansToDegree( std::atan2( velocity.y, velocity.x ) );
		const auto vel_delta = std::remainder( movementAngle.y - vel_ang, 360.f );

		auto retrack_speed = idealRot * ( ( Configs::m_cConfig.m_iAutoStraferSpeed / 100.f ) * 3 );

		if ( vel_delta <= retrack_speed || speed2D <= 15.f ) {
			if ( -retrack_speed <= vel_delta || speed2D <= 15.f ) {
				movementAngle.y += idealRot * sign;
				cmd->pBase->flSideMove = sign;
			}
			else {
				movementAngle.y = vel_ang - retrack_speed;
				cmd->pBase->flSideMove = 1.f;
			}
		}
		else {
			movementAngle.y = vel_ang + retrack_speed;
			cmd->pBase->flSideMove = -1.f;
		}
	}

	//MoveMINTFix( local, cmd, movementAngle.y );

	// my good friend philip gave me this code.
	// THANK YOU PHILIP!!!!
	const float rot{ Math::DegreeToRadians( cmd->pBase->pViewangles->angValue.y - movementAngle.y ) };

	const float newForward{ std::cos( rot ) * cmd->pBase->flForwardMove - std::sin( rot ) * cmd->pBase->flSideMove };
	const float newSide{ std::sin( rot ) * cmd->pBase->flForwardMove + std::cos( rot ) * cmd->pBase->flSideMove };

	// TODO: properly do toggle.
	//cmd->m_cButtonStates.m_iHeld &= ~( IN_BACK | IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT );
	//cmd->m_cButtonStates.m_iToggle &= ~( IN_BACK | IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT );

	cmd->pBase->flForwardMove = std::clamp( newForward, -1.f, 1.f );
	cmd->pBase->flSideMove = std::clamp( newSide * -1.f, -1.f, 1.f );
}

void CMovement::MoveMINTFix( C_CSPlayerPawn* local, CUserCmd* cmd, float wishAngle ) {
	cmd->pBase->pViewangles->angValue.NormalizeAngle( );

	if ( !cmd->pBase->flForwardMove && !cmd->pBase->flSideMove )
		return;

	if ( cmd->pBase->pViewangles->angValue.y == wishAngle )
		return;

	const auto oldMovementLength{ cmd->pBase->flForwardMove * cmd->pBase->flForwardMove + cmd->pBase->flSideMove * cmd->pBase->flSideMove };

	wishAngle = std::remainderf( wishAngle, 360.f );

	auto& curAngles{ cmd->pBase->pViewangles->angValue.y };
	curAngles = std::remainderf( curAngles, 360.f );

	float rot{ std::fabs( curAngles - wishAngle ) };
	if ( wishAngle <= curAngles )
		rot = 360.f - std::fabs( wishAngle - curAngles );

	const auto forwardMove{ cmd->pBase->flForwardMove };
	const auto sideMove{ cmd->pBase->flSideMove };

	// we do a little bit of l3d451r7ing...

	auto v57 = Math::DegreeToRadians( 360.f - rot );
	auto v58 = Math::DegreeToRadians( ( 360.f - rot ) + 90.f );

	auto v59 = std::cos( v57 ) * forwardMove;
	cmd->pBase->flForwardMove = v59 - ( std::cos( v58 ) * sideMove );

	auto v60 = std::sin( v58 ) * sideMove;
	cmd->pBase->flSideMove = v60 - ( std::sin( v57 ) * forwardMove );

	ClampMovement( cmd, oldMovementLength );
}

void CMovement::ClampMovement( CUserCmd* cmd, const float oldMovementLength ) {
	const auto curLength{ ( cmd->pBase->flForwardMove * cmd->pBase->flForwardMove + cmd->pBase->flSideMove * cmd->pBase->flSideMove ) };
	if ( curLength ) {
		float scale = oldMovementLength / curLength;
		if ( scale != 1.f ) {
			cmd->pBase->flForwardMove *= scale;
			cmd->pBase->flSideMove *= scale;
		}
	}
}

void CMovement::LimitSpeed( C_CSPlayerPawn* player, CUserCmd* cmd, float maxSpeed ) {
	CVAR( sv_accelerate )

	const auto cmdSpeed{ std::sqrt( ( cmd->pBase->flSideMove * cmd->pBase->flSideMove ) + ( cmd->pBase->flForwardMove * cmd->pBase->flForwardMove ) ) };
	const auto velocity{ player->m_vecAbsVelocity( ) };
	const auto speed{ velocity.Length2D( ) };

	if ( cmdSpeed <= maxSpeed + 1.f
		&& velocity.Length2D( ) <= maxSpeed + 1.f )
		return;

	Vector forward{ }, right{ };
	Math::AngleVectors( cmd->pBase->pViewangles->angValue, &forward, &right, nullptr );

	const auto diff{ speed - maxSpeed };
	auto wishSpeed{ maxSpeed };

	Vector velDir{ forward.x * cmd->pBase->flForwardMove + right.x * cmd->pBase->flSideMove,
		forward.y * cmd->pBase->flForwardMove + right.y * cmd->pBase->flSideMove, 0.f };

	const auto accel{ sv_accelerate->value.fl };
	const auto maxAccelSpeed{ accel * 0.015625f * std::max( 250.f, player->m_pMovementServices( )->m_flMaxSpeed( ) ) * player->m_pMovementServices( )->m_flSurfaceFriction( ) };

	if ( diff - maxAccelSpeed <= 0.f
		|| speed - maxAccelSpeed - 3.f <= 0.f )
		wishSpeed = maxSpeed;
	else {
		velDir = velocity;
		wishSpeed = -1.f;
	}

	cmd->pBase->flForwardMove = wishSpeed;
	cmd->pBase->flSideMove = 0;

	auto moveDir{ cmd->pBase->pViewangles->angValue.y };

	const auto direction{ std::atan2( velDir.y, velDir.x ) };
	moveDir = std::remainderf( Math::RadiansToDegree( direction ), 360.f );
	MoveMINTFix( player, cmd, moveDir );
}