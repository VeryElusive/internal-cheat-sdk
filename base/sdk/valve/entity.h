#pragma once
#include "entity_handle.h"
#include "interfaces/ischemasystem.h"

#define VAR_AT_OFFSET( type, name, offset ) __forceinline CBaseHandle& m_hPawn( ) { \
		return *reinterpret_cast< CBaseHandle* >( ( reinterpret_cast< std::uintptr_t >( this ) + m_hPawn_OFFSET ) ); \
	} \

#define SCHEMA( type, name ) VAR_AT_OFFSET( type, name, name##_OFFSET )

#define DEF_OFFSET( name ) static uint32_t name##_OFFSET;

// wow... what the fuck...
class CEntityInstance
{
public:
	void GetSchemaClassInfo( SchemaClassInfoData_t** pReturn ) {
		return Memory::CallVFunc<void, 34U>( this, pReturn );
	}

	SCHEMA( CBaseHandle, m_hPawn )

private:
	DEF_OFFSET( m_hPawn )

public:
	static void Initialise( );
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

