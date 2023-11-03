#pragma once
#include <string>

namespace Utils {
    __forceinline std::wstring StringToWideString( const std::string& str ) {
        std::wstring wstr;
        wstr.reserve( str.size( ) );

        for ( char c : str )
            wstr.push_back( static_cast< wchar_t >( c ) );

        return wstr;
    }

    __forceinline bool IsDigit( const std::uint8_t uChar ) {
        return ( uChar >= '0' && uChar <= '9' );
    }

    constexpr std::uint32_t CharToHexInt( const std::uint8_t uChar ) {
        const std::uint8_t uCharLower = ( uChar | ( 'a' ^ 'A' ) );
        return ( ( uCharLower >= 'a' && uCharLower <= 'f' ) ? ( uCharLower - 'a' + 0xA ) : ( IsDigit( uChar ) ? ( uChar - '0' ) : 0x0 ) );
    }
}