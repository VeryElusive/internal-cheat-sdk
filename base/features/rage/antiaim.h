#pragma once
#include "../../havoc.h"
#include "../../core/interfaces.h"
#include "../../sdk/valve/entity.h"
#include "../misc/movement.h"

class CAntiAim {
public:
	void Main( C_CSPlayerPawn* local, CUserCmd* cmd );

private:
};

namespace Features { inline CAntiAim AntiAim; };