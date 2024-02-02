#pragma once
#include "../../havoc.h"
#include "../../core/interfaces.h"
#include "../../sdk/valve/entity.h"
#include "../misc/movement.h"

struct AimPoint_t {
	Vector m_vecPoint{ };

	int m_iHitgroup{ };

	float m_flDamage{ };

	bool m_bPenetrated{ };
};

class CAimTarget {
public:
	CLagRecord* m_pRecord{ };
	AimPoint_t* m_pPoint{ };
	PlayerEntry_t* m_pEntry{ };

	int m_iBestDamage{ };

	void GetBestLagRecord( PlayerEntry_t& entry );
	void SelectBetterTarget( const CAimTarget& compareTarget );
	float QuickScan( const CLagRecord* record, std::vector <int> hitgroups );
};

class CRageBot {
public:
	void Main( C_CSPlayerPawn* local, CUserCmd* cmd );

private:
	bool CanFire( C_CSPlayerPawn* local );
	void SetupAttack( C_CSPlayerPawn* local, CUserCmd* cmd );
};

namespace Features { inline CRageBot RageBot; };