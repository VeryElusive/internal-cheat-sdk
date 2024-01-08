#pragma once
#include "../../../core/memory.h"
#include "../usercmd.h"

#define MULTIPLAYER_BACKUP 150

class IInput
{
public:
	void* pVtable;
	MEM_PAD( 0x248 );
	CUserCmd arrCommands[ MULTIPLAYER_BACKUP ];
	MEM_PAD( 0x1 );
	bool bInThirdPerson;
	MEM_PAD( 0x22 );
	int32_t nSequenceNumber;

	CUserCmd* GetUserCmd( ) {
		return &arrCommands[ nSequenceNumber % MULTIPLAYER_BACKUP ];
	}
};