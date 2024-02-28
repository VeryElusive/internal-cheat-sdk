#pragma once
#include "../../sdk/valve/entity.h"
#include "../../sdk/valve/trace.h"

struct PenetrationData_t {
	float m_flDamage{ };
	//uint8_t m_iHitbox{ };
	uint8_t m_iHitGroup{ };

	bool m_bPenetrated{ true };

	//uint8_t m_iPenetrationCount{ 4 };
};

class CPenetration {
public:
	bool FireBullet( Vector start, Vector end, 
		const C_CSPlayerPawn* shooter, const C_CSPlayerPawn* target,
		const CCSWeaponBaseVData* weaponData, PenetrationData_t& penData,
		const float minDmgCap = 0 );

	void ScaleDamage( const int hitgroup, const C_CSPlayerPawn* entity, const CCSWeaponBaseVData* weaponData, float& damage );
};

namespace Features { inline CPenetration Penetration; };