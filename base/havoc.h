#pragma once
#include "core/interfaces.h"

#include "sdk/matrix.h"
#include "sdk/color.h"

#define CHECK_AND_FAIL( check ) if ( !check ) { printf( #check " failed!\n"); return false; }

class C_CSPlayerPawn;
class CCSPlayerController;

struct PlayerEntry_t {
	struct Vis {
		int m_iHealth{ };
		float m_flAlpha{ };
		float m_flDormancyFade{ };
	}; Vis Visuals;

	struct Anim {

	}; Anim Animations;

	const char* m_pName{ };

	C_CSPlayerPawn* m_pPawn{ };

	void Reset( C_CSPlayerPawn* pawn ) {
		m_pPawn = pawn;
	}
};

struct Context_t {
	float m_flFrameTime{ };

	CCSPlayerController* m_pLocal{ };

	ViewMatrix_t m_matViewMatrix{ };

	Vector m_vecViewAngles{ };

	bool m_bUnloading{ };

	PlayerEntry_t PlayerEntries[ 64 ]{ };

	void GetLocal( ) {
		const int index{ Interfaces::Engine->GetLocalPlayer( ) };
		m_pLocal = Interfaces::GameResourceService->m_pGameEntitySystem->Get<CCSPlayerController>( index );
	}
};

inline Context_t ctx;