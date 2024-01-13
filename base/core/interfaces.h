#pragma once
#include "../sdk/valve/interfaces/CGlobalVarsBase.h"
#include "../sdk/valve/interfaces/IGameResourceService.h"
#include "../sdk/valve/interfaces/iengineclient.h"
#include "../sdk/valve/interfaces/ischemasystem.h"
#include "../sdk/valve/interfaces/imemalloc.h"
#include "../sdk/valve/interfaces/ccsgoinput.h"
#include "../sdk/valve/interfaces/ilocalize.h"

class ISource2Client;

namespace Interfaces {
	bool Init( );

	inline IEngineClient* Engine{ };
	inline ISource2Client* Client{ };
	inline CGlobalVarsBase* GlobalVars{ };
	inline IGameResourceService* GameResourceService{ };
	inline ISchemaSystem* SchemaSystem{ };
	inline IMemAlloc* MemAlloc{ };
	inline CCSGOInput* Input{ };
	inline ILocalize* Localize{ };
}