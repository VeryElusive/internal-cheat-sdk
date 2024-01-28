#pragma once
#include "../../../core/memory.h"

enum EClientFrameStage : int
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	// a network packet is being received
	FRAME_NET_UPDATE_START,
	// data has been received and we are going to start calling postdataupdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// data has been received and called postdataupdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// received all packets, we can now do interpolation, prediction, etc
	FRAME_NET_UPDATE_END,
	// start rendering the scene
	FRAME_RENDER_START,
	// finished rendering the scene
	FRAME_RENDER_END,
	FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};

enum FLOWTYPE : int {
	FLOW_OUTGOING = 0,
	FLOW_INCOMING = 1,
};

class CNetChannelInfo {
public:
	float GetLatency( int flow ) {
		return Memory::CallVFunc<int, 10u>( this, flow );
	}
};

class IEngineClient
{
public:
	int GetMaxClients( ) {
		return Memory::CallVFunc<int, 31U>( this );
	}

	bool IsInGame( ) {
		return Memory::CallVFunc<bool, 32U>( this );
	}

	bool IsConnected( ) {
		return Memory::CallVFunc<bool, 33U>( this );
	}

	CNetChannelInfo* GetNetChannelInfo( int splitscreenSlot = 0 ) {
		return Memory::CallVFunc<CNetChannelInfo*, 34U>( this, splitscreenSlot );
	}

	// return CBaseHandle index
	int GetLocalPlayer( ) {
		int nIndex = -1;

		Memory::CallVFunc<void, 44U>( this, std::ref( nIndex ), 0 );

		return nIndex + 1;
	}

	const char* GetLevelName( ) {
		return Memory::CallVFunc<const char*, 53U>( this );
	}

	const char* GetLevelNameShort( ) {
		return Memory::CallVFunc<const char*, 54U>( this );
	}

	const char* GetProductVersionString( ) {
		return Memory::CallVFunc<const char*, 77U>( this );
	}
};