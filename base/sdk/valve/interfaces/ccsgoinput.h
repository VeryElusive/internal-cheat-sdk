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
	MEM_PAD( 0x1 );//0x5200
	bool m_bCameraInThirdPerson;//0x5201
	MEM_PAD( 0x22 );//0x5202
	int m_nSequenceNumber;//0x5224
	MEM_PAD( 0x2B8 );//0x5228
	int m_iCommandPassCount{ };//0x54E0
	MEM_PAD( 0x4 );//0x54E4
	CSubTickData* m_pSubTickData{ };//0x54E8

	//MEM_PAD( 0x4 );//0xA0A4
	//CSubTickData m_arrSplitScreenPlayerSubTickData[ 2 ];//0xA0A8

	CUserCmd* GetUserCmd( ) {
		return &m_arrCommands[ m_nSequenceNumber % MULTIPLAYER_BACKUP ];
	}

	void SetViewAngles( Vector angle ) {
		return Displacement::SetViewAngles( this, 0, &angle );
	}

	// client.dll 49 8D 40 03 48 8D 04 40 49 8D 14 C1 90 48 8B 42 F8 48 3B C6 74 08 48 3D
	// ReadInputFrame
	void AddButton( ECommandButtons button ) {
		const auto buttonState{ Displacement::GetButtonState( this, 0 ) };

		auto& buttonCount{ RECAST_AT_OFFSET( int, buttonState, 0x34 ) };

		auto currentButton{ ( BYTE* ) ( reinterpret_cast< std::uintptr_t >( buttonState ) + 24 * ( 3 + buttonCount ) ) };

		auto& v15{ RECAST_AT_OFFSET( __int64, currentButton, -8 ) };
		v15 = button;
		*currentButton |= button;

		buttonCount += 1;
	}

	void RemoveButton( ECommandButtons button ) {
		const auto buttonState{ Displacement::GetButtonState( this, 0 ) };

		auto& buttonCount{ RECAST_AT_OFFSET( int, buttonState, 0x34 ) };

		auto v6{ RECAST_AT_OFFSET( int, this, 0x54F8 ) };

		while ( ( int ) v6 < buttonCount ) {
			auto currentButton{ ( BYTE* ) ( reinterpret_cast< std::uintptr_t >( buttonState ) + 24 * ( 3 + v6 ) ) };

			auto& v15{ RECAST_AT_OFFSET( __int64, currentButton, -8 ) };
			if ( v15 & button )
				v15 = 0;

			++v6;
		}
	}
};