#pragma once
#include "../../../sdk/matrix.h"


class IViewRender
{
public:
	Vector m_vecOrigin; // 0x0000
	Vector m_vecAngles; // 0x000C
	float m_flFov; // 0x0018
	char pad_0x001C[ 0x14 ]; // 0x001C
	ViewMatrix_t matUNK1; // 0x0030
	char pad_0x0070[ 0x30 ]; // 0x0070
	ViewMatrix_t matUNK2; // 0x00A0
	char pad_0x00E0[ 0xC8 ]; // 0x00E0
	ViewMatrix_t matUNK3; // 0x01A8
	char pad_0x01E8[ 0x28 ]; // 0x01E8
};