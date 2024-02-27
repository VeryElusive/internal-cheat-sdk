#pragma once

#define SHOW( cond ) [ ]( ) { return cond; }

MultiElement_t hitboxes[ 8 ]{
	{ &Configs::m_cConfig.m_bRageBotHitboxes[ 0 ], "Head" },
	{ &Configs::m_cConfig.m_bRageBotHitboxes[ 1 ], "Upper chest" },
	{ &Configs::m_cConfig.m_bRageBotHitboxes[ 2 ], "Chest" },
	{ &Configs::m_cConfig.m_bRageBotHitboxes[ 3 ], "Lower chest" },
	{ &Configs::m_cConfig.m_bRageBotHitboxes[ 4 ], "Stomach" },
	{ &Configs::m_cConfig.m_bRageBotHitboxes[ 5 ], "Arms" },
	{ &Configs::m_cConfig.m_bRageBotHitboxes[ 6 ], "Legs" },
	{ &Configs::m_cConfig.m_bRageBotHitboxes[ 7 ], "Feet" }
};

MultiElement_t multipoints[ 8 ]{
	{ &Configs::m_cConfig.m_bRageBotMultipoint[ 0 ], "Head" },
	{ &Configs::m_cConfig.m_bRageBotMultipoint[ 1 ], "Upper chest" },
	{ &Configs::m_cConfig.m_bRageBotMultipoint[ 2 ], "Chest" },
	{ &Configs::m_cConfig.m_bRageBotMultipoint[ 3 ], "Lower chest" },
	{ &Configs::m_cConfig.m_bRageBotMultipoint[ 4 ], "Stomach" },
	{ &Configs::m_cConfig.m_bRageBotMultipoint[ 5 ], "Arms" },
	{ &Configs::m_cConfig.m_bRageBotMultipoint[ 6 ], "Legs" },
	{ &Configs::m_cConfig.m_bRageBotMultipoint[ 7 ], "Feet" }
};

ComboElement_t WeaponTypes[ ]{ ( "None" ), ( "Icon" ), ( "Text" ) };

ComboElement_t RagebotWeaponGroups[ 9 ]{ ( "Pistol" ), ( "Heavy pistol" ), ( "SMG" ), ( "Rifle" ), ( "Shotgun" ), ( "Awp" ), ( "Scout" ), ( "Auto" ), ( "Machine gun" ) };

ComboElement_t pitch[ 5 ]{ ( "Default" ), ( "Up" ), ( "Down" ), ( "Zero" ), ( "Random" ) };
ComboElement_t yaw[ 4 ]{ ( "Default" ), ( "Backward" ), ( "Left" ), ( "Right" ) };
ComboElement_t yawAdd[ 5 ]{ ( "None" ), ( "Jitter" ), ( "Rotate" ), ( "Spin" ), ( "Random" ) };

int selectedConfig{ };
std::string typedConfig{ };