#pragma once

ComboElement_t comboElements[ ]{ "hello", "what's", "up", "guys" };

MultiElement_t multiComboElements[ 5 ]{
	{ &Configs::m_cConfig.m_bOptions[0], "option 1" },
	{ &Configs::m_cConfig.m_bOptions[1], "option 2" },
	{ &Configs::m_cConfig.m_bOptions[2], "option 3" },
	{ &Configs::m_cConfig.m_bOptions[3], "option 4" },
	{ &Configs::m_cConfig.m_bOptions[4], "option 5" },
};

int selectedConfig{ };
std::string typedConfig{ };