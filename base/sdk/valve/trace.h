#pragma once
#include "../../core/memory.h"
#include "../vector.h"

inline static constexpr std::uint32_t K_MAIN_PEN_MASK = 0x1C300Bu;
inline static constexpr std::uint32_t K_ENT_ENTRY_MASK = 0x7FFF;

struct SurfaceData_t {
    MEM_PAD( 0x8 );
    float m_flPenetrationModifier{ };
    float m_flDamageModifier{ };
    MEM_PAD( 0x4 );
    int m_iMaterial{ };
};

struct TraceHitboxData_t {
    MEM_PAD( 0x38 );
    int m_iHitgroup{ };
};

struct GameTrace_t {
    void* m_pSurface{ };
    void* m_pHitEntity{ };
    TraceHitboxData_t* m_pHitboxData{ };
    MEM_PAD( 0x38 );
    unsigned int m_iContents{ };
    MEM_PAD( 0x24 );
    Vector m_vecStart{ };
    Vector m_vecEnd{ };
    Vector m_vecNormal{ };
    Vector m_vecPos{ };
    MEM_PAD( 0x4 );
    float m_flFraction{ };
    MEM_PAD( 0x6 );
    bool m_bAllSolid{ };
    MEM_PAD( 0x4D );
};

struct TraceFilter_t {
    __int64 m_iMask{ };
    __int64 m_arrUnk[ 2 ]{ };
    int m_arrSkipHandles[ 4 ]{ };
    short m_iCollisions[ 2 ]{ };
    short m_v3{ };
    bool m_v4{ }, m_v5{ }, m_v6{ };

    TraceFilter_t( std::uintptr_t mask, const void* skip, int layer ) noexcept;
};

struct UpdateValue_t {
    float m_flPreviousLengthMod{ };
    float m_flCurrentLengthMod{ };
    MEM_PAD( 0x8 )
    short m_hHandleIdx{ };
    MEM_PAD( 0x6 )
};

struct UnknownTraceMember_t {
    MEM_PAD( 0x30 )
};

struct TraceData_t {
    int m_uk1{ };
    float m_uk2{ 52.0f };
    void* m_pUnk{ };
    int m_uk3{ 128 };
    int m_uk4{ static_cast< int >( 0x80000000 ) };
    UnknownTraceMember_t m_arr[ 0x80 ]{ };
    MEM_PAD( 0x8 );
    __int64 m_iNumUpdate{ };
    void* m_pUpdateValue{ };
    MEM_PAD( 0xC8 );
    Vector m_vecStart{ };
    Vector m_vecEnd{ };
    MEM_PAD( 0x50 );
};

struct HandleBulletPenetrationData_t {
    HandleBulletPenetrationData_t( const float dmgMod, const float pen, const float rangeMod, const float range, const int penCount, const bool failed ) :
        m_flDamage( dmgMod ),
        m_flPenetration( pen ),
        m_flRangeMod( rangeMod ),
        m_flRange( range ),
        m_iPenCount( penCount ),
        m_bFailed( failed ) {}

    float m_flDamage{ };
    float m_flPenetration{ };
    float m_flRangeMod{ };
    float m_flRange{ };
    int m_iPenCount{ };
    bool m_bFailed{ };
};

void CreateTrace( TraceData_t* const trace, const Vector start, const Vector end, const TraceFilter_t& filter, const int penetrationCount );
void InitTraceInfo( GameTrace_t* const hit );
void GetTraceInfo( TraceData_t* const trace, GameTrace_t* const hit, const float unknownFloat, void* unknown );
bool HandleBulletPenetration( TraceData_t* const trace, HandleBulletPenetrationData_t* stats, UpdateValue_t* const mod_value, const bool draw_showimpacts );