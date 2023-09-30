#pragma once
#include <cmath>

struct Vector {
	float x{ }, y{ }, z{ };

	Vector( ) {};

	float Dot( const Vector& v ) const {
		return ( this->x * v.x +
			this->y * v.y );
	}

	float LengthSquared( ) const {
		return ( this->Dot( *this ) );
	}

	float Length( ) const {
		return ( std::sqrt( this->LengthSquared( ) ) );
	}

	float DistTo( const Vector& vecEnd ) const {
		return ( *this - vecEnd ).Length( );
	}
	Vector operator-( const Vector& vecSubtract ) const {
		return Vector( this->x - vecSubtract.x, this->y - vecSubtract.y, this->z - vecSubtract.z );
	}

	Vector( int x, int y, int z ) : x( static_cast< float >( x ) ), y( static_cast< float >( y ) ), z( static_cast< float >( z ) ) {};
	Vector( float x, float y, float z ) : x( x ), y( y ), z( z ) {};
};


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

	Vector2D( ) {};

	constexpr Vector2D( int x, int y ) : x( x ), y( y ) {};
	constexpr Vector2D( float x, float y ) : x( static_cast<int>( x ) ), y( static_cast< int >( y ) ) {};
	constexpr Vector2D( int x, float y ) : x( static_cast<int>( x ) ), y( static_cast< int >( y ) ) {};
	constexpr Vector2D( float x, int y ) : x( static_cast<int>( x ) ), y( static_cast< int >( y ) ) {};
	constexpr Vector2D( long x, long y ) : x( static_cast<int>( x ) ), y( static_cast< int >( y ) ) {};
	constexpr Vector2D( short x, short y ) : x( static_cast<int>( x ) ), y( static_cast< int >( y ) ) {};
};

struct Vector4D {
	int x{ }, y{ }, z{}, v{ };

};