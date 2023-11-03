#pragma once
#include "memory.h"
#include "../sdk/valve/interfaces/CGlobalVarsBase.h"
#include "../sdk/valve/interfaces/IGameResourceService.h"
#include "../sdk/valve/interfaces/iengineclient.h"


namespace Interfaces {
	void Init( );

	inline IEngineClient* Engine{ };
	inline CGlobalVarsBase* GlobalVars{ };
	inline IGameResourceService* GameResourceService{ };
}