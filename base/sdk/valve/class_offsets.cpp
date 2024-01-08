#include "entity.h"
#include "class_offsets.h"
#include "../../utils/schema.h"

#define CONCAT3(a, b, c) a ## b ## c

//e.g expansion: #define INIT_SCHEMA( table, var ) var = Schema::GetOffset( FNV1A::HashConst( "CBasePlayerController::m_hPawn" )

#define INIT_SCHEMA(table, var) var##_OFFSET = Schema::GetOffset( FNV1A::HashConst( CONCAT3(#table, "::", #var) ) )

void ClassOffsets::Initialise( ) {
	CBasePlayerController::Initialise( );
	CGameSceneNode::Initialise( );
	CCollisionProperty::Initialise( );
	C_BaseEntity::Initialise( );
	CCSPlayerController::Initialise( );
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