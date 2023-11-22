#include "entity.h"
#include "../../utils/schema.h"

#define CONCAT3(a, b, c) a ## b ## c

//#define INIT_SCHEMA( table, var ) var = Schema::GetOffset( FNV1A::HashConst( "CBasePlayerController::m_hPawn" )
#define INIT_SCHEMA(table, var) var##_OFFSET = Schema::GetOffset( FNV1A::HashConst( CONCAT3(#table, "::", #var) ) )


void CEntityInstance::Initialise( ) {
	INIT_SCHEMA( CBasePlayerController, m_hPawn );
}