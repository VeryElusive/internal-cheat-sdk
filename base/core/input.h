#pragma once
#include "../havoc.h"

namespace Input {
	inline bool m_bState[ 256 ]{ };
	inline bool m_bPrevState[ 256 ]{ };
	inline float m_flScroll{ };

	inline Vector2D m_vecMousePos{ };
	inline Vector2D m_vecMouseDelta{ };

	void Update( );

	bool Hovered( Vector2D pos, Vector2D size );
	bool Down( int key );
	bool Pressed( int key );
	bool Released( int key );
}