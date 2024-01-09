#pragma once
#include "entity_handle.h"
#include "interfaces/ischemasystem.h"
#include "../transform.h"

#define VAR_AT_OFFSET( type, name, offset ) __forceinline type& name( ) const { \
		return *reinterpret_cast< type* >( ( reinterpret_cast< std::uintptr_t >( this ) + name##_OFFSET ) ); \
	} \

#define SCHEMA( type, name ) private: inline static uint32_t name##_OFFSET; \
		public: VAR_AT_OFFSET( type, name, name##_OFFSET )

class CCollisionProperty;

// wow... what the fuck...
class CEntityInstance
{
public:
	void GetSchemaClassInfo( SchemaClassInfoData_t** pReturn ) {
		return Memory::CallVFunc<void, 34U>( this, pReturn );
	}
};

class CGameSceneNode {
public:
	SCHEMA( Vector, m_vecAbsOrigin );
	SCHEMA( CTransform, m_nodeToWorld );
	SCHEMA( bool, m_bDormant );

public:
	static void Initialise( );
};

class CCollisionProperty
{
public:
	SCHEMA( Vector, m_vecMins );
	SCHEMA( Vector, m_vecMaxs );

public:
	static void Initialise( );
};

class C_BaseEntity : public CEntityInstance
{
public:
	SCHEMA( CGameSceneNode*, m_pGameSceneNode );
	SCHEMA( CCollisionProperty*, m_pCollision );
	SCHEMA( std::uint8_t, m_iTeamNum );
	SCHEMA( int, m_iHealth );

	__forceinline bool Dormant( ) {
		if ( this->m_pGameSceneNode( ) )
			return m_pGameSceneNode( )->m_bDormant( );

		return true;
	}

	__forceinline Vector& GetAbsOrigin( ) const {
		if ( this->m_pGameSceneNode( ) )
			return m_pGameSceneNode( )->m_vecAbsOrigin( );

		return VecEmpty;
	}

public:
	static void Initialise( );
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
	// todo: make this its own class
	SCHEMA( void*, m_pClippingWeapon );

public:
	static void Initialise( );
};

class C_CSPlayerPawn : public C_CSPlayerPawnBase
{
public:

};


class CBasePlayerController : public C_BaseEntity
{
public:
	SCHEMA( CBaseHandle, m_hPawn );

public:
	static void Initialise( );
};

class CCSPlayerController : public CBasePlayerController
{
public:
	SCHEMA( bool, m_bPawnIsAlive );
	SCHEMA( const char*, m_sSanitizedPlayerName );

public:
	static void Initialise( );
};

class CCSWeaponBaseVData
{
public:
	SCHEMA( const char*, m_szName );

public:
	static void Initialise( );
};
