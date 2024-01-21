#pragma once
#include <cinttypes>
#include <string_view>
#include "../../core/displacement.h"

inline static constexpr int k_unk_key_hash = 0x31415926;

struct KeyString_t {
    __forceinline explicit KeyString_t( const std::string string, const std::uintptr_t key ) :
        m_pKey( key == 0u ? FindKey( string ) : key ),
        m_szString( string.c_str( ) ) {}

    // particles.dll; 48 89 5C 24 08 57 48 81 EC C0 00 00 00 33 C0 8B
    __forceinline static std::uintptr_t FindKey( const std::string name ) {
        return Displacement::FindVarMaterial( name.data( ), 0x12, k_unk_key_hash );
    }
private:
    std::uintptr_t m_pKey{ };
    const char* m_szString{ };
};