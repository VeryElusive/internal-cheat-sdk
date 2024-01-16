#pragma once
#include "../../havoc.h"
#include "../../core/interfaces.h"
#include "../../sdk/valve/entity.h"


class CMovement {
public:
	void Main( C_CSPlayerPawn* local, CUserCmd* cmd );
	void MoveMINTFix( CUserCmd* cmd, float wishYaw );
private:
	void AutoStrafer( C_CSPlayerPawn* local, CUserCmd* cmd );
};

namespace Features { inline CMovement Movement; };