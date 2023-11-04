#pragma once
#include "core/interfaces.h"

#include "sdk/vector.h"
#include "sdk/color.h"

class CCSPlayerController;

struct PlayerEntry_t {
	struct Vis {

	}; Vis Visuals;

	struct Anim {

	}; Anim Animations;

	CCSPlayerController* m_pPlayer{ };
};

struct Context_t {
	float m_flFrameTime{ };

	CCSPlayerController* m_pLocal{ };

	PlayerEntry_t PlayerEntries[ 64 ]{ };

	void GetLocal( ) {
		const int nIndex = Interfaces::Engine->GetLocalPlayer( );
		m_pLocal = Interfaces::GameResourceService->m_pGameEntitySystem->Get<CCSPlayerController>( nIndex );
	}
};

inline Context_t ctx;