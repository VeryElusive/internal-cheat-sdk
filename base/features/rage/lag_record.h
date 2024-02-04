#pragma once
#include "../../sdk/valve/entity.h"

class CLagRecord {
public:
	CLagRecord( C_CSPlayerPawn* pawn, int nRenderTickCount,
		float flRenderTickFraction,
		int tickCount ) :

		m_flSimulationTime( pawn->m_flSimulationTime( ) ),
		m_vecOrigin( pawn->GetAbsOrigin( ) ),
		nRenderTickCount( nRenderTickCount ),
		flRenderTickFraction( flRenderTickFraction ),
		m_nAddedTickCount( tickCount )
	{ };

	bool IsRecordValid( ) const;
	void Apply( C_CSPlayerPawn* pawn ) const;

	CBoneData m_arrBones[ 128u ]{ };

	Vector m_vecOrigin{ };

	float m_flSimulationTime{ };

	int nRenderTickCount; // 0x60
	float flRenderTickFraction; // 0x64
	int m_nAddedTickCount{ };

};

class CLagBackup {
public:
	CLagBackup( C_CSPlayerPawn* pawn );
	void Apply( C_CSPlayerPawn* pawn ) const;

private:

	CBoneData m_arrBones[ 128u ]{ };
	Vector m_vecOrigin{ };
};

struct PlayerEntry_t;

class CLagCompensation {
public:
	void Main( );
private:
	void AddRecord( PlayerEntry_t& entry );
};

namespace Features { inline CLagCompensation LagCompensation; };