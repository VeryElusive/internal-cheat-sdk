#pragma once
#include <cinttypes>

#define TICKS_TO_TIME( ticks ) ( static_cast< float >( ticks ) / 64.f )
#define TIME_TO_TICKS( timez ) static_cast< int >( ( timez * 64.f ) + 0.5f )

class CGlobalVarsBase {
public:
	float m_flRealTime;
	std::int32_t m_nFrameCount;
	std::uint8_t pad0[ 0x8 ];
	std::int32_t m_nMaxClients;
	float m_flIntervalPerTick;
	std::uint8_t pad1[ 0x14 ];
	float m_flCurTime;
	float m_flCurTime2;
	std::uint8_t pad2[ 0xC ];
	std::int32_t m_nTickCount;
	float m_flIntervalPerTick2;
	std::uint8_t padding_3[ 0x138 ];
	std::uint64_t m_iCurrentMap;
	std::uint64_t m_iCurrentMapName; // ???
};