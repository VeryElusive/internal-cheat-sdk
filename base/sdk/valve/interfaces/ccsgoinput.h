#pragma once
#include "../../../core/memory.h"
#include "../usercmd.h"

#define MULTIPLAYER_BACKUP 150

class CCSGOInput
{
public:
	void* m_pVtable;//0x0
	MEM_PAD( 0x3038 );//0x8
	CUserCmd m_arrCommands[ MULTIPLAYER_BACKUP ];// 0x3040
	MEM_PAD( 0x1 );//8E00
	bool m_bCameraInThirdPerson;//8E01
	MEM_PAD( 0x22 );
	int32_t m_nSequenceNumber;

	CUserCmd* GetUserCmd( ) {
		return &m_arrCommands[ m_nSequenceNumber % MULTIPLAYER_BACKUP ];
	}

	__forceinline void test( ) const {
			*reinterpret_cast< int* >( ( reinterpret_cast< std::uintptr_t >( this ) + 0x61C8 ) ) = 1;
	}

	void SetViewAngles( Vector angle ) {
		return Displacement::SetViewAngles( this, 0, &angle );
	}
};