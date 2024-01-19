#pragma once
#include "../../havoc.h"
#include "../../core/interfaces.h"
#include "../../sdk/valve/entity.h"
#include "../misc/movement.h"

class CAntiAim {
public:
	void Main( C_CSPlayerPawn* local, CUserCmd* cmd, bool lastPass );

private:
	void Yaw( C_CSPlayerPawn* local, float& yaw, bool lastPass );
	bool Condition( C_CSPlayerPawn* local, CUserCmd* cmd );

	bool m_bJitter{ };
};

namespace Features { inline CAntiAim AntiAim; };