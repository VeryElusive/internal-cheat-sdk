#pragma once
#include "../../../core/memory.h"
#include "../usercmd.h"

#define MULTIPLAYER_BACKUP 150

class CSubTickData {
public:
	unsigned int m_iRenderTick{ };//0x0
	float m_flRenderTickFraction{ };//0x4
	unsigned int m_iPlayerTick{ };//0x8
	float m_flPlayerTickFraction{ };//0xc
	MEM_PAD( 0x18 );//0x10
	unsigned int m_iTargetEntIndex{ };//0x28
	MEM_PAD( 0x24 );//0x2c
	unsigned int m_nFrameNumber{ };//0x50
	MEM_PAD( 0xC );//0x54
};

class CCSGOInput
{
public:
	void* m_pVtable;//0x0
	MEM_PAD( 0x248 );//0x8
	CUserCmd m_arrCommands[ MULTIPLAYER_BACKUP ];// 0x250
	MEM_PAD( 0x1 );//0xB4D0
	bool m_bCameraInThirdPerson;//0xB4D1
	MEM_PAD( 0x22 );//0xB4D2
	int m_nSequenceNumber;//0xB4F4
	MEM_PAD( 0x1268 );//0xB4F8

	int m_iAttackSlot{ };//0xC760
	int m_Unk{ };//0xA094
	int m_iAttackSlot2{ };//0xA098
	int m_Unk2{ };//0xA09C
	int m_nUnknownSlot{ };//0xA0A0
	MEM_PAD( 0xc );//A0A4
	int m_nUnknownSlot2{ };//0xA0B0

	//MEM_PAD( 0x4 );//0xA0A4
	//CSubTickData m_arrSplitScreenPlayerSubTickData[ 2 ];//0xA0A8

	CUserCmd* GetUserCmd( ) {
		return &m_arrCommands[ m_nSequenceNumber % MULTIPLAYER_BACKUP ];
	}

	void SetViewAngles( Vector angle ) {
		return Displacement::SetViewAngles( this, 0, &angle );
	}
};