#pragma once
#include "../sdk/color.h"
#include "input.h"
#include <thread>
#include <filesystem>

struct Keybind_t;

struct Config_t {
	Color m_cAccent{ Color( 115, 155, 255 ) };
	Color m_cAccent2{ Color( 182, 139, 252 ) };

#pragma region visuals

#pragma region player ESP
	bool m_bEnabled[ 3 ];
	bool m_bName[ 3 ];
	Color m_colName[ 3 ] = { Color( 255, 255, 255 ), Color( 255, 255, 255 ), Color( 255, 255, 255 ) };

	bool m_bBox[ 3 ];
	Color m_colBox[ 3 ]{ Color( 255, 255, 255 ), Color( 255, 255, 255 ), Color( 255, 255, 255 ) };

	bool m_bAmmo[ 3 ];
	Color m_colAmmo[ 3 ]{ Color( 182, 139, 252 ), Color( 182, 139, 252 ), Color( 182, 139, 252 ) };

	bool m_bHealth[ 3 ];
	bool m_bHealthOverride[ 3 ];
	Color m_colHealth[ 3 ] = { Color( 115, 155, 255 ), Color( 115, 155, 255 ), Color( 115, 155, 255 ) };

	int m_iWeapon[ 3 ];
	Color m_colWeapon[ 3 ] = { Color( 255, 255, 255 ), Color( 255, 255, 255 ), Color( 255, 255, 255 ) };

	bool m_bSkeleton[ 3 ];
	Color m_colSkeleton[ 3 ] = { Color( 255, 255, 255 ), Color( 255, 255, 255 ), Color( 255, 255, 255 ) };

#pragma endregion player ESP

#pragma endregion visuals

#pragma region misc
	bool m_bBunnyhop{ };
	bool m_bAutoStrafer{ };
	int m_iAutoStraferSpeed{ 80 };
	bool m_bBugWalk{ };
	Keybind_t m_kBugWalk{ };

	bool m_bThirdperson{ };
	int m_iThirdpersonDistance{ 130 };
	Keybind_t m_kThirdperson{ };

#pragma endregion misc

#pragma region antiaim
	bool m_bAntiAimEnable{ };
	int m_iAntiAimPitch{ };
	int m_iAntiAimYaw{ };
	int m_iAntiAimYawAdd{ };
	int m_iAntiAimYawRange{ };
	int m_iAntiAimYawSpeed{ };
#pragma endregion antiaim

#pragma region ragebot
	bool m_bRageBotEnable{ };
	bool m_bRageBotAutoFire{ };
	bool m_bRageBotAutoScope{ };
	bool m_bRageBotSilentAim{ };
	int m_iRageBotHitchance{ };
	int m_iRageBotMinimumDamage{ };
	bool m_bRageBotAutowall{ };
	int m_iRageBotMinimumPenetrationDamage{ };
	bool m_bRageBotScaleDamage{ };
	bool m_bRageBotOverrideDamage{ };
	Keybind_t m_kRageBotOverrideDamage{ };
	int m_iRageBotMinimumOverrideDamage{ };
	bool m_bRageBotAutostop{ };
	bool m_bRageBotAutostopMoveBetweenShots{ };
	bool m_bRageBotHitboxes[ 9 ];
	bool m_bRageBotMultipoint[ 9 ];
	bool m_bRageBotStaticPoints{ };
	int m_iRageBotHeadPointScale{ };
	int m_iRageBotBodyPointScale{ };

	bool m_bRapidFire{ };
#pragma endregion ragebot
};

namespace Configs {
	inline bool m_bRefresh = { true };
	inline std::vector<std::string> m_vecFileNames = { };
	inline Config_t m_cConfig{ };
	inline std::filesystem::path m_fsPath = "C:\\Havoc\\Configs\\";

	void SaveCFG( std::string filePath );

	void LoadCFG( std::string filePath );

	void Refresh( );

	void Remove( const std::size_t nIndex );
}