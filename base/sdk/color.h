#pragma once
#include <stdint.h>


class Color {
public:
	Color( ) {};
	Color( uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255 ) : r( red ), g( green ), b( blue ), a( alpha ) {};
	Color( int red, int green, int blue, int alpha = 255 ) : r( red ), g( green ), b( blue ), a( alpha ) {};
	Color( float red, float green, float blue, float alpha = 1.f ) : r( static_cast<uint8_t>( red * 255.f ) ), g( static_cast< uint8_t >( green * 255.f ) ), b( static_cast< uint8_t >( blue * 255.f ) ), a( static_cast< uint8_t >( alpha * 255.f ) ) {};
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

	// skips alpha
	__forceinline Color Lerp( Color to, float strength ) {
		return Color(
			static_cast< int >( ( to.r - r ) * strength ) + r,
			static_cast< int >( ( to.g - g ) * strength ) + g,
			static_cast< int >( ( to.b - b ) * strength ) + b//,
			//static_cast< int >( ( to.a - a ) * strength ) + a
		);
	}

	uint8_t r{ }, g{ }, b{ }, a{ 255 };
};