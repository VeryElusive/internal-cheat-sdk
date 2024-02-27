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

	bool m_bValid{ };
};

struct PostCMoveData_t {
	Vector m_vecPoint{ };

	CLagRecord* m_pRecord{ };
	PlayerEntry_t* m_pEntry{ };

	bool m_bValid{ };
};

class CAimTarget {
public:
	CLagRecord* m_pRecord{ };
	AimPoint_t m_cPoint{ };
	PlayerEntry_t* m_pEntry{ };

	int m_iBestDamage{ };

	void GetBestLagRecord( PlayerEntry_t& entry );
	void SelectBetterTarget( const CAimTarget& compareTarget );
	void Attack( C_CSPlayerPawn* local, CUserCmd* cmd );
	void ScanTarget( C_CSPlayerPawn* local );

private:
	void GenerateMultiPoints( const CBoneData bone, const CHitbox& hitbox, const int hitboxIndex, const float scale, std::vector< int > hitboxes );
	void ScanPoint( const Vector point, const CHitbox& hitbox, std::vector< int > hitboxes );
	float QuickScan( const CLagRecord* record, std::vector <int> hitgroups );
};

class CRageBot {
public:
	void Main( C_CSPlayerPawn* local, CUserCmd* cmd );
	void PostCMove( C_CSPlayerPawn* local, CUserCmd* cmd );

	PostCMoveData_t m_cData{ };
private:
	bool CanFire( C_CSPlayerPawn* local );
};

namespace Features { inline CRageBot RageBot; };