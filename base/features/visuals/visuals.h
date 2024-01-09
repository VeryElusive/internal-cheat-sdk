#pragma once
#include "../../havoc.h"
#include "../../core/interfaces.h"
#include "../../sdk/valve/entity.h"

enum ESP_TYPE {
	ENEMY,
	TEAM,
	LOCAL
};

inline const Color DormantCol{ Color( 200, 200, 200 ) };

struct BBox_t {
	float x{ }, y{ }, w{}, h{ };

	const Vector2D Position( ) const { return { std::floor( x ), std::floor( y ) }; }
	const Vector2D Size( ) const { return { std::floor( w ), std::floor( h ) }; }
};

class CVisuals {
public:
	void Main( );

private:
	void HandlePlayer( PlayerEntry_t& entry );

	bool GetBBox( const C_CSPlayerPawn* pawn, BBox_t& bbox );

	void DrawBox( const PlayerEntry_t& entry, uint8_t type, const BBox_t& bbox );
	void DrawName( const PlayerEntry_t& entry, uint8_t type, const BBox_t& bbox );
	void DrawHealth( const PlayerEntry_t& entry, uint8_t type, const BBox_t& bbox );
	void DrawWeapon( const PlayerEntry_t& entry, uint8_t type, const BBox_t& bbox );
};

namespace Features { inline CVisuals Visuals; };