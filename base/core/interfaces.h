#pragma once
#include "../sdk/valve/interfaces/CGlobalVarsBase.h"
#include "../sdk/valve/interfaces/IGameResourceService.h"
#include "../sdk/valve/interfaces/iengineclient.h"
#include "../sdk/valve/interfaces/ischemasystem.h"
#include "../sdk/valve/interfaces/imemalloc.h"
#include "../sdk/valve/interfaces/iinput.h"


namespace Interfaces {
	void Init( );

	inline IEngineClient* Engine{ };
	inline CGlobalVarsBase* GlobalVars{ };
	inline IGameResourceService* GameResourceService{ };
	inline ISchemaSystem* SchemaSystem{ };
	inline IMemAlloc* MemAlloc{ };
	inline IInput* Input{ };
}