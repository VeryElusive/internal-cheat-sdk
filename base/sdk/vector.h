#pragma once
#include <cmath>
#include <algorithm>

struct Vector {
	float x{ }, y{ }, z{ };

	Vector( ) {};

	Vector Cross( const Vector& vecCross ) const {
		return Vector( this->y * vecCross.z - this->z * vecCross.y, this->z * vecCross.x - this->x * vecCross.z, this->x * vecCross.y - this->y * vecCross.x );
	}

	float Dot( const Vector& v ) const {
		return ( this->x * v.x +
			this->y * v.y + this->z * v.z );
	}

	float LengthSquared( ) const {
		return ( this->Dot( *this ) );
	}

	float Length( ) const {
		return ( std::sqrt( this->LengthSquared( ) ) );
	}

	const float Length2DSqr( ) const {
		return ( this->x * this->x + this->y * this->y );
	}

	const float Length2D( ) const {
		return std::sqrtf( this->Length2DSqr( ) );
	}

	float DistTo( const Vector& vecEnd ) const {
		return ( *this - vecEnd ).Length( );
	}

	bool IsEqual( const Vector& angEqual, const float flErrorMargin = std::numeric_limits<float>::epsilon( ) ) const {
		return ( std::fabsf( this->x - angEqual.x ) < flErrorMargin &&
			std::fabsf( this->y - angEqual.y ) < flErrorMargin &&
			std::fabsf( this->z - angEqual.z ) < flErrorMargin );
	}

	auto& ClampAngle( ) { this->x = std::clamp( this->x, -89.f, 89.f ); this->y = std::clamp( this->y, -180.f, 180.f ); this->z = std::clamp( this->z, -50.f, 50.f ); return *this; }

	void NormalizeAngle( ) {
		this->x = std::remainderf( this->x, 180.f );
		this->y = std::remainderf( this->y, 360.f );
	}

	float NormalizeInPlace( ) {
		const float flLength = this->Length( );
		const float flRadius = 1.0f / ( flLength + std::numeric_limits<float>::epsilon( ) );

		this->x *= flRadius;
		this->y *= flRadius;
		this->z *= flRadius;

		return flLength;
	}

	Vector Normalized( ) const {
		Vector vecOut = *this;
		vecOut.NormalizeInPlace( );
		return vecOut;
	}

	bool operator==( const Vector& angBase ) const {
		return this->IsEqual( angBase );
	}


	Vector operator+( const Vector& vecAdd ) const {
		return Vector( this->x + vecAdd.x, this->y + vecAdd.y, this->z + vecAdd.z );
	}

	Vector operator-( const Vector& vecSubtract ) const {
		return Vector( this->x - vecSubtract.x, this->y - vecSubtract.y, this->z - vecSubtract.z );
	}

	Vector operator*( const Vector& vecMultiply ) const {
		return Vector( this->x * vecMultiply.x, this->y * vecMultiply.y, this->z * vecMultiply.z );
	}

	Vector operator/( const Vector& vecDivide ) const {
		return Vector( this->x / vecDivide.x, this->y / vecDivide.y, this->z / vecDivide.z );
	}

	Vector operator+( const float flAdd ) const {
		return Vector( this->x + flAdd, this->y + flAdd, this->z + flAdd );
	}

	Vector operator-( const float flSubtract ) const {
		return Vector( this->x - flSubtract, this->y - flSubtract, this->z - flSubtract );
	}

	Vector operator*( const float flMultiply ) const {
		return Vector( this->x * flMultiply, this->y * flMultiply, this->z * flMultiply );
	}

	Vector operator/( const float flDivide ) const {
		return Vector( this->x / flDivide, this->y / flDivide, this->z / flDivide );
	}

	constexpr Vector& operator+=( const Vector& vecBase ) {
		this->x += vecBase.x; this->y += vecBase.y; this->z += vecBase.z;
		return *this;
	}

	constexpr Vector& operator-=( const Vector& vecBase ) {
		this->x -= vecBase.x; this->y -= vecBase.y; this->z -= vecBase.z;
		return *this;
	}

	constexpr Vector& operator*=( const Vector& vecBase ) {
		this->x *= vecBase.x; this->y *= vecBase.y; this->z *= vecBase.z;
		return *this;
	}

	constexpr Vector& operator/=( const Vector& vecBase ) {
		this->x /= vecBase.x; this->y /= vecBase.y; this->z /= vecBase.z;
		return *this;
	}

	constexpr Vector& operator+=( const float flAdd ) {
		this->x += flAdd; this->y += flAdd; this->z += flAdd;
		return *this;
	}

	constexpr Vector& operator-=( const float flSubtract ) {
		this->x -= flSubtract; this->y -= flSubtract; this->z -= flSubtract;
		return *this;
	}

	constexpr Vector& operator*=( const float flMultiply ) {
		this->x *= flMultiply; this->y *= flMultiply; this->z *= flMultiply;
		return *this;
	}

	constexpr Vector& operator/=( const float flDivide ) {
		this->x /= flDivide; this->y /= flDivide; this->z /= flDivide;
		return *this;
	}

	Vector( int x, int y, int z ) : x( static_cast< float >( x ) ), y( static_cast< float >( y ) ), z( static_cast< float >( z ) ) {};
	Vector( float x, float y, float z ) : x( x ), y( y ), z( z ) {};
	constexpr Vector( const float* arrVector ) : x( arrVector[ 0 ] ), y( arrVector[ 1 ] ), z( arrVector[ 2 ] ) { }
};

static inline Vector VecEmpty{ };

struct Vector2D {
	float x{ }, y{ };

	constexpr bool operator > ( const Vector2D& rhs ) const {
		return ( this->x > rhs.x && this->y > rhs.y );
	}

	constexpr bool operator < ( const Vector2D& rhs ) const {
		return ( this->x < rhs.x&& this->y < rhs.y );
	}

	__forceinline Vector2D operator+( Vector2D v ) const {
		return Vector2D( x + v.x, y + v.y );
	}

	__forceinline Vector2D operator-( Vector2D v ) const {
		return Vector2D( x - v.x, y - v.y );
	}

	__forceinline Vector2D operator+( int v ) const {
		return Vector2D( x + v, y + v );
	}

	__forceinline Vector2D operator-( int v ) const {
		return Vector2D( x - v, y - v );
	}

	__forceinline Vector2D operator*( float v ) const {
		return Vector2D( x * v, y * v );
	}

	__forceinline Vector2D operator/( float v ) const {
		return Vector2D( x / v, y / v );
	}

	__forceinline void operator-=( Vector2D v ) {
		this->x -= v.x;
		this->y -= v.y;
	}

	__forceinline void operator-=( int v ) {
		this->x -= v;
		this->y -= v;
	}

	__forceinline void operator+=( Vector2D v ) {
		this->x += v.x;
		this->y += v.y;
	}

	__forceinline void operator+=( int v ) {
		this->x += v;
		this->y += v;
	}

	__forceinline void operator*=( float v ) {
		this->x *= v;
		this->y *= v;
	}

	void Floor( ) {
		x = std::floor( x );
		y = std::floor( y );
	}

	float Dot( const Vector2D& v ) const {
		return ( this->x * v.x +
			this->y * v.y );
	}

	float LengthSquared( ) const {
		return ( this->Dot( *this ) );
	}

	float Length( ) const {
		return ( sqrt( this->LengthSquared( ) ) );
	}

	Vector2D( ) {};

	constexpr Vector2D( int x, int y ) : x( static_cast< float >( x ) ), y( static_cast< float >( y ) ) {};
	constexpr Vector2D( float x, float y ) : x( x ), y( y ) {};
	constexpr Vector2D( int x, float y ) : x( static_cast< float >( x ) ), y( y ) {};
	constexpr Vector2D( float x, int y ) : x( x ), y( static_cast< float >( y ) ) {};
	constexpr Vector2D( long x, long y ) : x( static_cast< float >( x ) ), y( static_cast< float >( y ) ) {};
	constexpr Vector2D( short x, short y ) : x( static_cast< float >( x ) ), y( static_cast< float >( y ) ) {};
};

struct Vector4D {
	float x{ }, y{ }, z{}, w{ };

};
struct alignas( 16 ) VectorAligned : Vector
{
	VectorAligned( ) = default;

	explicit VectorAligned( const Vector& vecBase )
	{
		this->x = vecBase.x;
		this->y = vecBase.y;
		this->z = vecBase.z;
		this->w = 0.f;
	}

	constexpr VectorAligned& operator=( const Vector& vecBase )
	{
		this->x = vecBase.x;
		this->y = vecBase.y;
		this->z = vecBase.z;
		this->w = 0.f;
		return *this;
	}

	float w = 0.f;
};

static_assert( alignof( VectorAligned ) == 16 );
