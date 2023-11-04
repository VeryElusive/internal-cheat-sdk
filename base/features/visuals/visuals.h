#pragma once
#include "../../core/interfaces.h"
#include "../../sdk/valve/entity.h"

class CVisuals {
public:
	void Main( );

private:
	void HandlePlayer( CCSPlayerController* player );
};

namespace Features { inline CVisuals Visuals; };