#pragma once
#include "core/interfaces.h"

#include "sdk/matrix.h"
#include "sdk/color.h"

class C_CSPlayerPawn;
class CCSPlayerController;

struct BBox_t {
	float x{ }, y{ }, w{}, h{ };

	const Vector2D Position( ) const { return { std::floor( x ), std::floor( y ) }; }
	const Vector2D Size( ) const { return { std::floor( w ), std::floor( h ) }; }
	const bool Valid( ) const { return x != 0 && y != 0, w != 0 && h != 0; }
};

struct PlayerEntry_t {
	struct Vis {
		int m_iHealth{ };
		float m_flAlpha{ };
		float m_flDormancyFade{ };
		BBox_t m_cBBox{ };

		inline void ApplyDormancy( Color& col ) const {
			col.a *= this->m_flAlpha;

			if ( this->m_flDormancyFade )
				col = col.Lerp( Color( 200, 200, 200, static_cast< int >( col.a * 0.4f ) ), this->m_flDormancyFade );
		}
	}; Vis Visuals;

	struct Anim {

	}; Anim Animations;

	const char* m_pName{ };

	C_CSPlayerPawn* m_pPawn{ };

	void Reset( C_CSPlayerPawn* pawn ) {
		m_pPawn = pawn;

		Visuals.m_cBBox = { 0,0,0,0 };
	}
};

struct Context_t {
	float m_flFrameTime{ };

	CCSPlayerController* m_pLocal{ };

	ViewMatrix_t m_matViewMatrix{ };

	Vector m_vecViewAngles{ };

	bool m_bUnloading{ };

	PlayerEntry_t PlayerEntries[ 64 ]{ };

	float m_flForwardmove{ };
	float m_flSidemove{ };
	float m_flUpmove{ };

	void GetLocal( ) {
		const int index{ Interfaces::Engine->GetLocalPlayer( ) };
		m_pLocal = Interfaces::GameResourceService->m_pGameEntitySystem->Get<CCSPlayerController>( index );
	}
};

inline Context_t ctx;