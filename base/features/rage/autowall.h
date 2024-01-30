#pragma once
#include "../../sdk/valve/entity.h"
#include "../../sdk/valve/trace.h"

struct PenetrationData_t {
	float m_flDamage{ };

	uint8_t m_iPenetrationCount{ };
};

class CPenetration {
public:
	bool FireBullet( Vector start, Vector end, 
		C_CSPlayerPawn* shooter, C_CSPlayerPawn* target, 
		CCSWeaponBaseVData* weaponData, PenetrationData_t& penData );

	void ScaleDamage( const int hitgroup, const C_CSPlayerPawn* entity, CCSWeaponBaseVData* weaponData, float& damage );
};

namespace Features { inline CPenetration Penetration; };