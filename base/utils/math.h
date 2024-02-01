#pragma once
#include <algorithm>

#pragma region math_definitions
#define M_HPI           1.5707963267948966f  // half pi
#define M_QPI           0.7853981633974483f  // quarter pi
#define M_PI            3.1415926535897932f  // pi const
#define M_2PI           6.2831853071795865f  // pi double
#define M_GPI           1.6180339887498f     // golden ratio
#define M_RADPI         57.295779513082f     // pi in radians
#pragma endregion

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

	__forceinline void SinCos( float radians, float& sine, float& cosine ) { 
		sine = sin( radians ); 
		cosine = cos( radians ); 
	}

	
	__forceinline float DegreeToRadians( float degrees ) {
		return degrees * static_cast< float >( M_PI ) / 180.0f;
	}

	__forceinline float RadiansToDegree( float fRadians ) noexcept {
		return fRadians * ( 180.0f / M_PI ); 
	}

	__forceinline void AngleVectors( const Vector& angView, Vector* pForward, Vector* pRight, Vector* pUp ) {
		float sp, sy, sr, cp, cy, cr;

		SinCos( DegreeToRadians( angView.x ), sp, cp );
		SinCos( DegreeToRadians( angView.y ), sy, cy );
		SinCos( DegreeToRadians( angView.z ), sr, cr );

		if ( pForward ) {
			pForward->x = cp * cy;
			pForward->y = cp * sy;
			pForward->z = -sp;
		}

		if ( pRight ) {
			pRight->x = -1 * sr * sp * cy + -1 * cr * -sy;
			pRight->y = -1 * sr * sp * sy + -1 * cr * cy;
			pRight->z = -1 * sr * cp;
		}

		if ( pUp ) {
			pUp->x = cr * sp * cy + -sr * -sy;
			pUp->y = cr * sp * sy + -sr * cy;
			pUp->z = cr * cp;
		}
	}

	__forceinline Vector VectorTransform( const Vector& vecTransform, const Matrix3x4_t& matrix ) {
		return Vector( vecTransform.Dot( matrix[ 0 ] ) + matrix[ 0 ][ 3 ],
			vecTransform.Dot( matrix[ 1 ] ) + matrix[ 1 ][ 3 ],
			vecTransform.Dot( matrix[ 2 ] ) + matrix[ 2 ][ 3 ] );
	}

	__forceinline void VectorAngles( const Vector& vecForward, Vector& angView ) {
		Vector  left;
		float   len, pitch, yaw, roll;

		// get 2d length.
		len = vecForward.Length2D( );

		if ( len > 0.f ) {
			// calculate pitch and yaw.
			pitch = RadiansToDegree( std::atan2( -vecForward.z, len ) );
			yaw = RadiansToDegree( std::atan2( vecForward.y, vecForward.x ) );
			roll = 0.f;
		}

		else {
			pitch = float( ( vecForward.z > 0 ) ? 270 : 90 );
			yaw = 0.f;
			roll = 0.f;
		}

		// set out angles.
		angView = { pitch, yaw, roll };
	}
}