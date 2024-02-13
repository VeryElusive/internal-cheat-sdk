#pragma once
#include "../sdk/valve/interfaces/CGlobalVarsBase.h"
#include "../sdk/valve/interfaces/IGameResourceService.h"
#include "../sdk/valve/interfaces/iengineclient.h"
#include "../sdk/valve/interfaces/ischemasystem.h"
#include "../sdk/valve/interfaces/imemalloc.h"
#include "../sdk/valve/interfaces/ccsgoinput.h"
#include "../sdk/valve/interfaces/ilocalize.h"
#include "../sdk/valve/interfaces/iinputsystem.h"
#include "../sdk/valve/interfaces/csource2client.h"

class IEngineCVar;

namespace Interfaces {
	bool Init( );

	inline IEngineClient* Engine{ };
	inline CSource2Client* Client{ };
	inline CGlobalVarsBase* GlobalVars{ };
	inline IGameResourceService* GameResourceService{ };
	inline ISchemaSystem* SchemaSystem{ };
	inline IMemAlloc* MemAlloc{ };
	inline CCSGOInput* Input{ };
	inline ILocalize* Localize{ };
	inline IEngineCVar* Cvar{ };
	inline IInputSystem* InputSystem{ };
	inline void* Unknown{ };
}