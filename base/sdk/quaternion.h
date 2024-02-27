#pragma once
#include "matrix.h"

struct Quaternion
{
	constexpr Quaternion( const float x = 0.0f, const float y = 0.0f, const float z = 0.0f, const float w = 0.0f ) :
		x( x ), y( y ), z( z ), w( w ) { }

	[[nodiscard]] bool IsValid( ) const
	{
		return ( std::isfinite( x ) && std::isfinite( y ) && std::isfinite( z ) && std::isfinite( w ) );
	}

	/// @param[in] vecOrigin [optional] translation for converted matrix
	/// @returns: matrix converted from quaternion
	[[nodiscard]] Matrix3x4_t ToMatrix( const Vector& vecOrigin = {} ) const
	{
		Matrix3x4_t matOut;

#ifdef _DEBUG // precalculate common multiplications
		const float x2 = this->x + this->x, y2 = this->y + this->y, z2 = this->z + this->z;
		const float xx = this->x * x2, xy = this->x * y2, xz = this->x * z2;
		const float yy = this->y * y2, yz = this->y * z2;
		const float zz = this->z * z2;
		const float wx = this->w * x2, wy = this->w * y2, wz = this->w * z2;

		matOut[ 0 ][ 0 ] = 1.0f - ( yy + zz );
		matOut[ 1 ][ 0 ] = xy + wz;
		matOut[ 2 ][ 0 ] = xz - wy;

		matOut[ 0 ][ 1 ] = xy - wz;
		matOut[ 1 ][ 1 ] = 1.0f - ( xx + zz );
		matOut[ 2 ][ 1 ] = yz + wx;

		matOut[ 0 ][ 2 ] = xz + wy;
		matOut[ 1 ][ 2 ] = yz - wx;
		matOut[ 2 ][ 2 ] = 1.0f - ( xx + yy );
#else // let the compiler optimize calculations itself
		matOut[ 0 ][ 0 ] = 1.0f - 2.0f * this->y * this->y - 2.0f * this->z * this->z;
		matOut[ 1 ][ 0 ] = 2.0f * this->x * this->y + 2.0f * this->w * this->z;
		matOut[ 2 ][ 0 ] = 2.0f * this->x * this->z - 2.0f * this->w * this->y;

		matOut[ 0 ][ 1 ] = 2.0f * this->x * this->y - 2.0f * this->w * this->z;
		matOut[ 1 ][ 1 ] = 1.0f - 2.0f * this->x * this->x - 2.0f * this->z * this->z;
		matOut[ 2 ][ 1 ] = 2.0f * this->y * this->z + 2.0f * this->w * this->x;

		matOut[ 0 ][ 2 ] = 2.0f * this->x * this->z + 2.0f * this->w * this->y;
		matOut[ 1 ][ 2 ] = 2.0f * this->y * this->z - 2.0f * this->w * this->x;
		matOut[ 2 ][ 2 ] = 1.0f - 2.0f * this->x * this->x - 2.0f * this->y * this->y;
#endif

		matOut[ 0 ][ 3 ] = vecOrigin.x;
		matOut[ 1 ][ 3 ] = vecOrigin.y;
		matOut[ 2 ][ 3 ] = vecOrigin.z;
		return matOut;
	}

	Quaternion Conjugate( ) const {
		return Quaternion( w, -x, -y, -z );
	}

	Quaternion operator*( const Vector& v ) const {
		// Efficiently calculate using Hamilton product
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;
		float xx = x * x;
		float xy = x * y;
		float xz = x * z;
		float yx = y * x;
		float yy = y * y;
		float yz = y * z;
		float zx = z * x;
		float zy = z * y;
		float zz = z * z;

		return Quaternion(
			w * v.x - wy - xz + zy,
			wx + xy - yz - zz,
			wx + yx - zx - zy,
			wz + yz + xy - xx );
	};

	Quaternion operator*( const Quaternion& other ) const {
		// Efficiently calculate using Hamilton product
		float w1 = w, x1 = x, y1 = y, z1 = z;
		float w2 = other.w, x2 = other.x, y2 = other.y, z2 = other.z;

		return Quaternion(
			w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2,
			w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2,
			w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2,
			w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2 );
	}



	float x, y, z, w;
};

struct alignas( 16 ) QuaternionAligned_t : Quaternion
{
	QuaternionAligned_t& operator=( const Quaternion& quatOther )
	{
		this->x = quatOther.x;
		this->y = quatOther.y;
		this->z = quatOther.z;
		this->w = quatOther.w;
		return *this;
	}
};

static_assert( alignof( QuaternionAligned_t ) == 16 );