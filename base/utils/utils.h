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
}