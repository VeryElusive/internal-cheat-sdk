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
	MEM_PAD( 0x3038 );//0x8
	CUserCmd m_arrCommands[ MULTIPLAYER_BACKUP ];// 0x3040
	MEM_PAD( 0x1 );//0x8E00
	bool m_bCameraInThirdPerson;//0x8E01
	MEM_PAD( 0x22 );//0x8E02
	int m_nSequenceNumber;//0x8E24
	MEM_PAD( 0x1278 );//0x8E28

	// TODO: this seems to be wrong. not sure why.
	int m_nSplitScreenPlayers{ };//0xA0A0
	MEM_PAD( 0x4 );//0xA0A4
	CSubTickData m_arrSplitScreenPlayerSubTickData[ 2 ];//0xA0A8

	CUserCmd* GetUserCmd( ) {
		return &m_arrCommands[ m_nSequenceNumber % MULTIPLAYER_BACKUP ];
	}

	void SetViewAngles( Vector angle ) {
		return Displacement::SetViewAngles( this, 0, &angle );
	}
};