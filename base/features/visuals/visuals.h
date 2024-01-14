#pragma once
#include "../../havoc.h"
#include "../../core/interfaces.h"
#include "../../sdk/valve/entity.h"

enum ESP_TYPE {
	ENEMY,
	TEAM,
	LOCAL
};

class CVisuals {
public:
	void Main( );

	bool GetBBox( const C_CSPlayerPawn* pawn, BBox_t& bbox );

private:
	void HandlePlayer( PlayerEntry_t& entry );


	void DrawBox( const PlayerEntry_t& entry, uint8_t type );
	void DrawName( const PlayerEntry_t& entry, uint8_t type );
	void DrawHealth( const PlayerEntry_t& entry, uint8_t type );
	bool DrawAmmo( const PlayerEntry_t& entry, uint8_t type );
	void DrawWeapon( const PlayerEntry_t& entry, uint8_t type, const bool didAmmoBar );
};

namespace Features { inline CVisuals Visuals; };