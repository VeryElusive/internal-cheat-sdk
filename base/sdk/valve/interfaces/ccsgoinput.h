#pragma once
#include "../../../core/memory.h"
#include "../usercmd.h"

#define MULTIPLAYER_BACKUP 150

class CSubTickData {
public:
	unsigned int m_iRenderTick{ };
	float m_flRenderTickFraction{ };
	unsigned int m_iPlayerTick{ };
	float m_flPlayerTickFraction{ };

	MEM_PAD( 0x50 );
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
	CSubTickData m_arrSplitScreenPlayerSubTickData[ 2 ];//0xA0A8 (NOT SURE OF NUM OF ELEMENTS!) this is used alongside the inputHistoryField stuff. so probably same size as that. be cautious using this!

	CUserCmd* GetUserCmd( ) {
		return &m_arrCommands[ m_nSequenceNumber % MULTIPLAYER_BACKUP ];
	}

	void SetViewAngles( Vector angle ) {
		return Displacement::SetViewAngles( this, 0, &angle );
	}
};