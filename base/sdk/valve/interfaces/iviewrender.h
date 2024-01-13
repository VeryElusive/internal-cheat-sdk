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

class CViewSetup
{
public:
	MEM_PAD( 0x490 );
	float m_flOrthoLeft; // 0x0494
	float m_flOrthoTop; // 0x0498
	float m_flOrthoRight; // 0x049C
	float m_flOrthoBottom; // 0x04A0
	MEM_PAD( 0x38 );
	float m_flFov; // 0x04D8
	float m_flFovViewmodel; // 0x04DC
	Vector m_vecOrigin; // 0x04E0
	MEM_PAD( 0xC ); // 0x04EC
	Vector m_angView; // 0x04F8
	MEM_PAD( 0x14 ); // 0x0504
	float m_flAspectRatio; // 0x0518
};