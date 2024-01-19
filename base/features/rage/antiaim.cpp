#include "antiaim.h"
#include "../misc/movement.h"
#include "../../core/config.h"
#include <random>

bool CAntiAim::Condition( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	if ( local->m_MoveType( ) != MOVETYPE_WALK )
		return false;

	if ( cmd->m_cButtonStates.m_iHeld & IN_ATTACK )
		return false;

	if ( cmd->m_cButtonStates.m_iHeld & IN_USE )
		return false;

	return true;
}

// TODO: detected! look at nConsumedServerAngleChanges (pretty sure it is a checksum but havent reversed it)

// TODO: set the sub tick angles to this aswell.

void CAntiAim::Yaw( C_CSPlayerPawn* local, float& yaw, bool lastPass ) {
	const int& yawRange{ Configs::m_cConfig.m_iAntiAimYawRange };

	static bool invert{ };
	static int rotatedYaw{ };

	switch ( Configs::m_cConfig.m_iAntiAimYaw ) {
	case 0: yaw += 0.f; break;// forward
	case 1: yaw += 180.f; break;// backward
	case 2: yaw += 90.f; break;// left
	case 3: yaw -= 90.f; break;// right
	default: break;
	}

	switch ( Configs::m_cConfig.m_iAntiAimYawAdd ) {
	case 1:// jitter 
		yaw += yawRange * ( m_bJitter ? 0.5f : -0.5f );
		break;
	case 2: {// rotate
		if ( lastPass ) {
			rotatedYaw -= invert ? Configs::m_cConfig.m_iAntiAimYawSpeed : -Configs::m_cConfig.m_iAntiAimYawSpeed;

			if ( rotatedYaw < yawRange * -0.5f )
				invert = false;
			else if ( rotatedYaw > yawRange * 0.5f )
				invert = true;

			rotatedYaw = std::clamp<int>( rotatedYaw, yawRange * -0.5f, yawRange * 0.5f );
		}

		yaw += rotatedYaw;
	}break;
	case 3: {// spin
		if ( lastPass ) {
			rotatedYaw += Configs::m_cConfig.m_iAntiAimYawSpeed;
			rotatedYaw = std::remainderf( rotatedYaw, 360.f );
		}
		yaw = rotatedYaw;
		break;
	}
	/*case 4: {// random
		yaw += Math::RandomFloat( -yawRange / 2.f, yawRange / 2.f );
		break;
	}*/
	default: break;
	}
}

void CAntiAim::Main( C_CSPlayerPawn* local, CUserCmd* cmd, bool lastPass ) {
	if ( !Configs::m_cConfig.m_bAntiAimEnable
		|| !Condition( local, cmd ) )
		return;

	if ( lastPass )
		m_bJitter = !m_bJitter;

	switch ( Configs::m_cConfig.m_iAntiAimPitch ) {
	case 0:
		break;
	case 1: 
		cmd->cmd.pBase->pViewangles->angValue.x -89.f; // up
		break;
	case 2: 
		cmd->cmd.pBase->pViewangles->angValue.x = 89.f; // down
		break;
	case 3: 
		cmd->cmd.pBase->pViewangles->angValue.x = 0.f;  // zero
		break;
	//case 4: return rand( );  break;// random
	default: 
		break;
	}

	const auto old{ cmd->cmd.pBase->pViewangles->angValue };

	Yaw( local, cmd->cmd.pBase->pViewangles->angValue.y, lastPass );

	Features::Movement.MoveMINTFix( local, cmd, old.y );
}