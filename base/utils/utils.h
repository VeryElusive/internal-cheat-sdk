#pragma once
#include <string>

constexpr int _NUMBER_MAX_BASE = 36;

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

	constexpr bool IsSpace( const std::uint8_t uChar ) {
		return ( ( uChar >= '\t' && uChar <= '\r' ) || uChar == ' ' );
	}

	constexpr bool IsAlpha( const std::uint8_t uChar ) {
		return ( ( uChar >= 'A' && uChar <= 'Z' ) || ( uChar >= 'a' && uChar <= 'z' ) );
	}

	constexpr bool IsAlphaNum( const std::uint8_t uChar ) {
		return ( ( uChar >= '0' && uChar <= '9' ) || ( uChar >= 'A' && uChar <= 'Z' ) || ( uChar >= 'a' && uChar <= 'z' ) );
	}

	template <typename T> requires ( std::is_integral_v<T> )
		constexpr T StringToInteger( const char* szSourceBegin, char** pszSourceEnd = nullptr, int iBase = 0 )
	{
		if ( iBase < 0 || iBase == 1 || iBase > _NUMBER_MAX_BASE )
			return 0;

		const char* szSourceCurrent = szSourceBegin;

		// skip to first not whitespace
		while ( IsSpace( *szSourceCurrent ) )
			++szSourceCurrent;

		// determine the sign and strip it
		const bool bIsPositive = ( ( *szSourceCurrent == '+' || *szSourceCurrent == '-' ) ? ( *szSourceCurrent++ == '+' ) : true );
		constexpr bool bIsUnsigned = ( ( std::numeric_limits<T>::min )( ) == 0U );

		// user provided exact number base
		if ( iBase > 0 )
		{
			// strip 0x or 0X
			if ( iBase == 16 && *szSourceCurrent == '0' && ( szSourceCurrent[ 1 ] | ( 'a' ^ 'A' ) ) == 'x' )
				szSourceCurrent += 2;
		}
		// otherwise try to determine base automatically
		else if ( *szSourceCurrent == '0' )
		{
			if ( ( ( *szSourceCurrent++ ) | ( 'a' ^ 'A' ) ) == 'x' )
			{
				// a hexadecimal number is defined as "the prefix 0x or 0X followed by a sequence of the decimal digits and the letters a (or A) through f (or F) with values 10 through 15 respectively" (C standard 6.4.4.1)
				iBase = 16;
				++szSourceCurrent;
			}
			else
				// an octal number is defined as "the prefix 0 optionally followed by a sequence of the digits 0 through 7 only" (C standard 6.4.4.1) and so any number that starts with 0, including just 0, is an octal number
				iBase = 8;
		}
		else
			// a decimal number is defined as beginning "with a nonzero digit and consisting of a sequence of decimal digits" (C standard 6.4.4.1)
			iBase = 10;

		constexpr std::uint64_t ullNegativeMax = ( bIsUnsigned ? ( std::numeric_limits<T>::max )( ) : ( static_cast< std::uint64_t >( ( std::numeric_limits<T>::max )( ) ) + 1ULL ) );
		const std::uint64_t ullAbsoluteMax = ( bIsPositive ? ( std::numeric_limits<T>::max )( ) : ullNegativeMax );
		const std::uint64_t ullAbsoluteMaxOfBase = ullAbsoluteMax / iBase;

		bool bIsNumber = false;
		std::uint64_t ullResult = 0ULL;

		for ( bool bIsDigit = false, bIsAlpha = false; ( ( bIsDigit = IsDigit( *szSourceCurrent ) ) ) || ( ( bIsAlpha = IsAlpha( *szSourceCurrent ) ) );) // @note: looks slightly unsafe but have possibility to fast path, double parenthesis to suppress warnings
		{
			int iCurrentDigit = 0;

			if ( bIsDigit )
				iCurrentDigit = *szSourceCurrent - '0';
			else if ( bIsAlpha )
				iCurrentDigit = ( *szSourceCurrent | ( 'a' ^ 'A' ) ) - 'a' + 0xA;

			if ( iCurrentDigit >= iBase )
				break;

			bIsNumber = true;
			++szSourceCurrent;

			// if the number has already hit the maximum value for the current type then the result cannot change, but we still need to advance source to the end of the number
			if ( ullResult == ullAbsoluteMax )
				continue;

			if ( ullResult <= ullAbsoluteMaxOfBase )
				ullResult *= iBase;
			else
				ullResult = ullAbsoluteMax;

			if ( ullResult <= ullAbsoluteMax - iCurrentDigit )
				ullResult += iCurrentDigit;
			else
				ullResult = ullAbsoluteMax;
		}

		if ( pszSourceEnd != nullptr )
			*pszSourceEnd = const_cast< char* >( bIsNumber ? szSourceCurrent : szSourceBegin );

		// clamp on overflow
		if ( ullResult == ullAbsoluteMax )
			return ( ( bIsPositive || bIsUnsigned ) ? ( std::numeric_limits<T>::max )( ) : ( std::numeric_limits<T>::min )( ) );

		return ( bIsPositive ? static_cast< T >( ullResult ) : -static_cast< T >( ullResult ) );
	}
}