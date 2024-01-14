#pragma once
#include "../../../core/memory.h"
#include "../usercmd.h"

#define MULTIPLAYER_BACKUP 150

class CCSGOInput
{
public:
	void* m_pVtable;
	MEM_PAD( 0x248 );
	CUserCmd m_arrCommands[ MULTIPLAYER_BACKUP ];
	MEM_PAD( 0x1 );
	bool m_bCameraInThirdPerson;
	MEM_PAD( 0x22 );
	int32_t m_nSequenceNumber;

	CUserCmd* GetUserCmd( ) {
		return &m_arrCommands[ m_nSequenceNumber % MULTIPLAYER_BACKUP ];
	}
};