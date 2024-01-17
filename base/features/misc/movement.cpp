#include "movement.h"
#include "../../core/config.h"
#include "../../utils/math.h"

void CMovement::Main( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	const auto onGround{ ( local->m_fFlags( ) & FL_ONGROUND ) };
	static int lastOnGround{ };
	static bool wait{ };

	if ( local->m_MoveType( ) != MOVETYPE_WALK )
		return;

	if ( Configs::m_cConfig.m_bBunnyhop ) {
		if ( cmd->m_cButtonStates.m_iHeld & IN_JUMP ) {
			if ( onGround && lastOnGround ) {
				cmd->m_cButtonStates.m_iHeld &= ~IN_JUMP;
				cmd->m_cButtonStates.m_iToggle |= IN_JUMP;
			}
			else if ( lastOnGround && !onGround )
				cmd->m_cButtonStates.m_iToggle |= IN_JUMP;
		}
	}


	if ( Configs::m_cConfig.m_bBugWalk && Configs::m_cConfig.m_kBugWalk.m_bEnabled ) {
		if ( local->m_fFlags( ) & FL_ONGROUND ) {
			cmd->m_cButtonStates.m_iHeld |= IN_JUMP;
			cmd->m_cButtonStates.m_iToggle |= IN_JUMP;
		}
		else {
			cmd->m_cButtonStates.m_iHeld &= ~IN_JUMP;
			cmd->m_cButtonStates.m_iToggle &= ~IN_JUMP;
		}
	}

	if ( Configs::m_cConfig.m_bAutoStrafer )
		AutoStrafer( local, cmd );
	//ctx.m_flUpmove = cmd->cmd.pBase->flUpMove;

	lastOnGround = onGround;
}

void CMovement::AutoStrafer( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	if ( local->m_fFlags( ) & FL_ONGROUND )
		return;

	if ( !cmd->cmd.pBase
		|| !cmd->cmd.pBase->pViewangles )
		return;

	const auto velocity{ local->m_vecAbsVelocity( ) };
	const auto speed2D{ velocity.Length2D( ) };
	if ( speed2D < 2.f
		&& !cmd->cmd.pBase->flForwardMove && !cmd->cmd.pBase->flSideMove )
		return;

	const auto idealRot{ std::min( Math::RadiansToDegree( std::asinf( 15.f / speed2D ) ), 90.f ) };
	const auto sign{ cmd->cmd.pBase->nCommandNumber % 2 ? 1.f : -1.f };

	bool move_forward = cmd->m_cButtonStates.m_iHeld & IN_FORWARD, move_backward = cmd->m_cButtonStates.m_iHeld & IN_BACK;
	bool move_left = cmd->m_cButtonStates.m_iHeld & IN_MOVELEFT, move_right = cmd->m_cButtonStates.m_iHeld & IN_MOVERIGHT;

	cmd->cmd.pBase->flForwardMove = speed2D > 0.1f ? 0.f : 1.f;

	auto movementAngle{ cmd->cmd.pBase->pViewangles->angValue };

	if ( move_forward )
		movementAngle.y += move_left ? 45.f : move_right ? -45.f : 0.f;
	else if ( move_backward )
		movementAngle.y += move_left ? 135.f : move_right ? -135.f : 180.f;
	else if ( move_left || move_right )
		movementAngle.y += move_left ? 90.f : -90.f;

	static auto old_yaw = 0.f;
	auto yaw_delta = std::remainder( movementAngle.y - old_yaw, 360.f ), abs_yaw_delta = std::abs( yaw_delta );
	old_yaw = movementAngle.y;

	if ( yaw_delta > 0.f ) cmd->cmd.pBase->flSideMove = -1.f;
	else if ( yaw_delta < 0.f ) cmd->cmd.pBase->flSideMove = 1.f;

	if ( abs_yaw_delta <= idealRot || abs_yaw_delta >= 30.f ) {
		const auto vel_ang = Math::RadiansToDegree( std::atan2( velocity.y, velocity.x ) );
		const auto vel_delta = std::remainder( movementAngle.y - vel_ang, 360.f );

		auto retrack_speed = idealRot * ( ( Configs::m_cConfig.m_iAutoStraferSpeed / 100.f ) * 3 );

		if ( vel_delta <= retrack_speed || speed2D <= 15.f ) {
			if ( -retrack_speed <= vel_delta || speed2D <= 15.f ) {
				movementAngle.y += idealRot * sign;
				cmd->cmd.pBase->flSideMove = sign;
			}
			else {
				movementAngle.y = vel_ang - retrack_speed;
				cmd->cmd.pBase->flSideMove = 1.f;
			}
		}
		else {
			movementAngle.y = vel_ang + retrack_speed;
			cmd->cmd.pBase->flSideMove = -1.f;
		}
	}

	///MoveMINTFix( cmd, movementAngle.y );

	// my good friend philip gave me this code.
	// THANK YOU PHILIP!!!!
	const float rot{ Math::DegreeToRadians( cmd->cmd.pBase->pViewangles->angValue.y - movementAngle.y ) };

	const float newForward{ std::cos( rot ) * cmd->cmd.pBase->flForwardMove - std::sin( rot ) * cmd->cmd.pBase->flSideMove };
	const float newSide{ std::sin( rot ) * cmd->cmd.pBase->flForwardMove + std::cos( rot ) * cmd->cmd.pBase->flSideMove };

	// TODO: properly do toggle.
	cmd->m_cButtonStates.m_iHeld &= ~( IN_BACK | IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT );
	cmd->m_cButtonStates.m_iToggle &= ~( IN_BACK | IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT );

	cmd->cmd.pBase->flForwardMove = std::clamp( newForward, -1.f, 1.f );
	cmd->cmd.pBase->flSideMove = std::clamp( newSide * -1.f, -1.f, 1.f );
}

void CMovement::MoveMINTFix( C_CSPlayerPawn* local, CUserCmd* cmd, Vector wishAngle ) {
	if ( !cmd->cmd.pBase->flForwardMove && !cmd->cmd.pBase->flSideMove )
		return;

	cmd->cmd.pBase->pViewangles->angValue.NormalizeAngle( ).ClampAngle( );

	if ( cmd == nullptr )
		return;

	Vector o_fwd, o_right, n_fwd, n_right;
	Math::AngleVectors( wishAngle, &o_fwd, &o_right, nullptr );
	Math::AngleVectors( cmd->cmd.pBase->pViewangles->angValue, &n_fwd, &n_right, nullptr );
	o_fwd.z = o_right.z = n_fwd.z = n_right.z = 0.f;
	o_fwd.NormalizeInPlace( ), o_right.NormalizeInPlace( ), n_fwd.NormalizeInPlace( ), n_right.NormalizeInPlace( );

	auto o_wish_coords = o_fwd * cmd->cmd.pBase->flForwardMove + o_right * cmd->cmd.pBase->flSideMove;
	cmd->cmd.pBase->flForwardMove = ( n_right.y * o_wish_coords.x - n_right.x * o_wish_coords.y ) / ( n_right.y * n_fwd.x - n_right.x * n_fwd.y );
	cmd->cmd.pBase->flSideMove = ( n_fwd.x * o_wish_coords.y - n_fwd.y * o_wish_coords.x ) / ( n_right.y * n_fwd.x - n_right.x * n_fwd.y );
}