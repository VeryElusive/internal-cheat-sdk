#pragma once
#include "../sdk/color.h"
#include <thread>
#include <filesystem>

struct Config_t {
#pragma region visuals

#pragma region player ESP
	bool m_bEnabled[ 3 ];
	bool m_bName[ 3 ];
	Color m_colName[ 3 ] = { Color( 255, 255, 255 ) };

	bool m_bBox[ 3 ];
	Color m_colBox[ 3 ]{ Color( 255, 255, 255 ) };

	bool m_bAmmo[ 3 ];
	Color m_colAmmo[ 3 ]{ Color( 182, 139, 252 ) };

	bool m_bHealth[ 3 ];
	bool m_bHealthOverride[ 3 ];
	Color m_colHealth[ 3 ] = { Color( 115, 155, 255 ) };

	int m_iWeapon[ 3 ];
	Color m_colWeapon[ 3 ] = { Color( 255, 255, 255 ) };

#pragma endregion player ESP

#pragma endregion visuals

#pragma region misc
	bool m_bBunnyhop{ };
	bool m_bBugWalk{ };
#pragma endregion misc

	Color m_cColorPickerTest{ Color( 115, 155, 255 ) };

	bool m_bOptions[ 5 ]{ };
};

namespace Configs {
	inline std::vector<std::string> m_vecFileNames = { };
	inline Config_t m_cConfig{ };
	inline std::filesystem::path m_fsPath = "C:\\Havoc-Assault-Cube\\Configs\\";

	void SaveCFG( std::string filePath );

	void LoadCFG( std::string filePath );

	void Refresh( );

	void Remove( const std::size_t nIndex );
}