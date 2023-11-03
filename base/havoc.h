#pragma once
#include "pch.h"

#include "utils/math.h"

#include "core/interfaces.h"

#include "core/config.h"
#include "core/memory.h"

#include "sdk/vector.h"
#include "sdk/color.h"

class CCSPlayerController;

struct Context_t {
	float m_flFrameTime{ };

	CCSPlayerController* m_pLocal{ };

	void GetLocal( ) {
		const int nIndex = Interfaces::Engine->GetLocalPlayer( );
		m_pLocal = Interfaces::GameResourceService->m_pGameEntitySystem->Get<CCSPlayerController>( nIndex );
	}
};

inline Context_t ctx;