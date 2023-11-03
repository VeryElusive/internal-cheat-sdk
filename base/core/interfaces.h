#pragma once
#include "memory.h"
#include "../sdk/valve/interfaces/CGlobalVarsBase.h"


namespace Interfaces {
	void Init( );

	inline CGlobalVarsBase* GlobalVars{ };
}