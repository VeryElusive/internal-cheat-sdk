#pragma once
#include "../../sdk/valve/entity.h"

class CLagRecord {
public:
	CLagRecord( C_CSPlayerPawn* pawn, int nRenderTickCount,
		float flRenderTickFraction,
		int tickCount ) :

		m_flSimulationTime( pawn->m_flSimulationTime( ) ),
		m_vecOrigin( pawn->m_pGameSceneNode( )->m_vecOrigin( ) ),
		nRenderTickCount( nRenderTickCount ),
		flRenderTickFraction( flRenderTickFraction ),
		m_nAddedTickCount( tickCount )
	{ };

	bool IsRecordValid( ) const;

	CBoneData m_arrBones[ 128u ]{ };

	Vector m_vecOrigin{ };

	float m_flSimulationTime{ };

	int nRenderTickCount; // 0x60
	float flRenderTickFraction; // 0x64
	int m_nAddedTickCount{ };

};


struct PlayerEntry_t;

class CLagCompensation {
public:
	void Main( );
private:
	void AddRecord( PlayerEntry_t& entry );
};

namespace Features { inline CLagCompensation LagCompensation; };