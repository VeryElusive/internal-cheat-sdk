#pragma once
#include "../../sdk/valve/entity.h"

class CLagRecord {
public:
	CLagRecord( C_CSPlayerPawn* pawn, int nRenderTickCount,
		float flRenderTickFraction,
		int tickCount, float tickFraction ) :

		m_flSimulationTime( pawn->m_flSimulationTime( ) ),
		m_vecOrigin( pawn->GetAbsOrigin( ) ),
		nRenderTickCount( nRenderTickCount ),
		flRenderTickFraction( flRenderTickFraction ),
		m_nPlayerTickCount( tickCount ),
		m_flPlayerTickFraction( tickFraction )
	{ };

	bool IsRecordValid( ) const;
	void Apply( C_CSPlayerPawn* pawn ) const;

	CBoneData m_arrBones[ 128u ]{ };

	Vector m_vecOrigin{ };

	float m_flSimulationTime{ };

	int nRenderTickCount; //
	float flRenderTickFraction; 
	int m_nPlayerTickCount{ };
	float m_flPlayerTickFraction{ };

};

class CLagBackup {
public:
	CLagBackup( C_CSPlayerPawn* pawn );
	void Apply( C_CSPlayerPawn* pawn ) const;

private:

	CBoneData m_arrBones[ 128u ]{ };
	Vector m_vecOrigin{ };
	float m_flSimulationTime{ };
};

struct PlayerEntry_t;

class CLagCompensation {
public:
	void Main( );
private:
	void AddRecord( PlayerEntry_t& entry );
};

namespace Features { inline CLagCompensation LagCompensation; };