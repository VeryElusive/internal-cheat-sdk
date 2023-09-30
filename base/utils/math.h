#pragma once

namespace Math {
	template <class T>
	__forceinline T Lerp( float flPercent, T const& A, T const& B ) {
		return A + ( B - A ) * flPercent;
	}

	__forceinline float Interpolate( const float from, const float to, const float percent ) { 
		if ( from == to ) 
			return from; 

		return to * percent + from * ( 1.f - percent ); 
	}
}