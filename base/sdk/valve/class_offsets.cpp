#include "entity.h"
#include "class_offsets.h"
#include "../../utils/schema.h"
#include <string>

//e.g expansion: #define INIT_SCHEMA( table, var ) var = Schema::GetOffset( FNV1A::HashConst( "CBasePlayerController::m_hPawn" )

#ifdef _DEBUG
#define INIT_SCHEMA(table, var) var##_OFFSET = Schema::GetOffset( FNV1A::HashConst( CONCAT3(#table, "::", #var) ) ); printf("%s::%s -> 0x%X\n", #table, #var, var##_OFFSET);
#else
#define INIT_SCHEMA(table, var) var##_OFFSET = Schema::GetOffset( FNV1A::HashConst( CONCAT3(#table, "::", #var) ) )
#endif


void ClassOffsets::Initialise( ) {
	ENTER_REGION( "Schema" );

	CBasePlayerController::Initialise( );
	CGameSceneNode::Initialise( );
	CCollisionProperty::Initialise( );
	C_BaseEntity::Initialise( );
	CCSPlayerController::Initialise( );
	C_CSPlayerPawnBase::Initialise( );
	CCSWeaponBaseVData::Initialise( );

	// other


}


void CBasePlayerController::Initialise( ) {
	INIT_SCHEMA( CBasePlayerController, m_hPawn );
}

void CCSPlayerController::Initialise( ) {
	INIT_SCHEMA( CCSPlayerController, m_bPawnIsAlive );
	INIT_SCHEMA( CCSPlayerController, m_sSanitizedPlayerName );
}

void CGameSceneNode::Initialise( ) {
	INIT_SCHEMA( CGameSceneNode, m_vecAbsOrigin );
	INIT_SCHEMA( CGameSceneNode, m_nodeToWorld );
	INIT_SCHEMA( CGameSceneNode, m_bDormant );
}

void CCollisionProperty::Initialise( ) {
	INIT_SCHEMA( CCollisionProperty, m_vecMins );
	INIT_SCHEMA( CCollisionProperty, m_vecMaxs );
}

void C_BaseEntity::Initialise( ) {
	INIT_SCHEMA( C_BaseEntity, m_pGameSceneNode );
	INIT_SCHEMA( C_BaseEntity, m_pCollision );
	INIT_SCHEMA( C_BaseEntity, m_iTeamNum );
	INIT_SCHEMA( C_BaseEntity, m_iHealth );
}

void C_CSPlayerPawnBase::Initialise( ) {
	INIT_SCHEMA( C_CSPlayerPawnBase, m_pClippingWeapon );
}

void CCSWeaponBaseVData::Initialise( ) {
	INIT_SCHEMA( CCSWeaponBaseVData, m_szName );
}