#pragma once
#include "core/interfaces.h"
#include "features/rage/lag_record.h"

#include "sdk/matrix.h"
#include "sdk/color.h"
#include "sdk/valve/skeleton.h"
#include <unordered_map>

class C_CSPlayerPawn;
class CCSPlayerController;
class CCSWeaponBaseVData;

struct BBox_t {
	float x{ }, y{ }, w{}, h{ };

	const Vector2D Position( ) const { return { std::floor( x ), std::floor( y ) }; }
	const Vector2D Size( ) const { return { std::floor( w ), std::floor( h ) }; }
	const bool Valid( ) const { return x != 0 && y != 0 && w != 0 && h != 0
			&& std::isfinite( x ) && std::isfinite( y ) && std::isfinite( w ) && std::isfinite( h ); }
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
		Vector m_vecLastBoneOrigin{ };

		bool m_bShouldUpdateBones{ };

		std::vector< CLagRecord > m_vecLagRecords{ };

	}; Anim Animations;

	const char* m_pName{ };

	C_CSPlayerPawn* m_pPawn{ };


	void Reset( C_CSPlayerPawn* pawn ) {
		m_pPawn = pawn;

		Visuals.m_cBBox = { 0,0,0,0 };
		Visuals.m_flAlpha = 0.f;
		Visuals.m_flDormancyFade = 0.f;
		Visuals.m_iHealth = 0;
	}
};

struct Context_t {
	float m_flFrameTime{ };

	//CCSPlayerController* m_pLocal{ };

	ViewMatrix_t m_matViewMatrix{ };

	Vector m_vecViewAngles{ };

	bool m_bUnloading{ };

	//PlayerEntry_t PlayerEntries[ 64 ]{ };
	std::unordered_map< unsigned int, PlayerEntry_t > m_mapPlayerEntries{ };

	HWND hwnd{ };

	CBoneData DEBUGBacktrackBones[ 128u ]{ };//

	float m_flRenderTickFraction{ };
	int m_iRenderTick{ };
	float m_flPlayerTickFraction{ };
	int m_iPlayerTick{ };
	int m_nFrameNumber{ };

	float m_flForwardmove{ };
	float m_flSidemove{ };
	float m_flUpmove{ };

	__forceinline CCSPlayerController* GetLocal( );
	__forceinline C_CSPlayerPawn* GetPlayerPawn( CCSPlayerController* player );
	__forceinline C_CSPlayerPawn* GetLocalPawn( );
	__forceinline CCSWeaponBaseVData* GetWeaponData( C_CSPlayerPawn* local );
	__forceinline CCSWeaponBaseVData* GetWeaponData( );
};

inline Context_t ctx;

#include "havoc.inl"