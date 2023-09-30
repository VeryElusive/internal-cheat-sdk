#pragma once
#include <stdint.h>


class Color {
public:
	Color( ) {};
	Color( uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255 ) : r( red ), g( green ), b( blue ), a( alpha ) {};
	Color( int red, int green, int blue, int alpha = 255 ) : r( red ), g( green ), b( blue ), a( alpha ) {};
	Color( float red, float green, float blue, float alpha = 1.f ) : r( red * 255.f ), g( green * 255.f ), b( blue * 255.f ), a( alpha * 255.f ) {};
	Color( int hex ) {
		r = ( ( hex >> 16 ) & 0xFF ) / 255;
		g = ( ( hex >> 8 ) & 0xFF ) / 255;
		b = ( ( hex ) & 0xFF ) / 255;
	};

	uint32_t ToUInt32( ) const {
		return ( static_cast< uint32_t >( a ) << 24 ) | ( static_cast< uint32_t >( b ) << 16 ) | ( static_cast< uint32_t >( g ) << 8 ) | static_cast< uint32_t >( r );
	}


	int ToHex( ) {
		return ( ( r & 0xff ) << 16 ) + ( ( g & 0xff ) << 8 ) + ( b & 0xff );
	}

	Color Alpha( const uint8_t nValue ) const {
		auto colCopy{ *this };
		colCopy.a = nValue;
		return colCopy;
	}

	uint8_t r{ }, g{ }, b{ }, a{ 255 };
};