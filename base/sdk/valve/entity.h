#pragma once
#include "entity_handle.h"
#include "interfaces/ischemasystem.h"

// wow... what the fuck...
class CEntityInstance
{
public:
	void GetSchemaClassInfo( SchemaClassInfoData_t** pReturn ) const {
		return Memory::CallVFunc<void, 34U>( this, pReturn );
	}
};

class C_BaseEntity : public CEntityInstance
{
public:

};

class C_BaseModelEntity : public C_BaseEntity
{
public:

};

//C_CSPlayerPawn : C_CSPlayerPawnBase, C_BasePlayerPawn, [ C_BaseCombatCharacter, C_BaseFlex, CBaseAnimGraph ], C_BaseModelEntity, C_BaseEntity, CEntityInstance
class C_BasePlayerPawn : public C_BaseModelEntity
{
public:

};

class C_CSPlayerPawnBase : public C_BasePlayerPawn
{
public:

};

class C_CSPlayerPawn : public C_CSPlayerPawnBase
{
public:

};


class CBasePlayerController : public C_BaseEntity
{
public:

};

class CCSPlayerController : public CBasePlayerController
{
public:

};

