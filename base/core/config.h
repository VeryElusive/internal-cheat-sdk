#pragma once
#include "../sdk/color.h"
#include <thread>
#include <filesystem>

struct Config_t {
	bool m_bAimbotEnable{ };
	bool m_bAimbotThroughWalls{ };
	float m_iAimbotSliderFloat{ };
	int m_iAimbotSlider{ };
	int m_iAimbotCombo{ };

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