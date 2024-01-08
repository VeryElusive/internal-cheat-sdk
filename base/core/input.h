#pragma once
#include "../havoc.h"

namespace Input {
	inline bool m_bState[ 256 ]{ };
	inline bool m_bPrevState[ 256 ]{ };
	inline float m_flScroll{ };

	inline Vector2D m_vecMousePos{ };
	inline Vector2D m_vecMouseDelta{ };

	inline void Update( ) {
		// TODO:
		//if ( GetForegroundWindow( ) != ctx.hwnd )
		//	return;

		for ( int i{ }; i < 256; ++i ) {
			m_bPrevState[ i ] = m_bState[ i ];
			m_bState[ i ] = GetAsyncKeyState( i );
		}

		POINT mouse{ };
		GetCursorPos( &mouse );
		//ScreenToClient( hwnd, &mouse );

		m_vecMouseDelta = m_vecMousePos - ( Vector2D( mouse.x, mouse.y ) );

		m_vecMousePos = Vector2D( mouse.x, mouse.y );
	}

	inline bool Hovered( Vector2D pos, Vector2D size ) {
		return ( m_vecMousePos > pos && m_vecMousePos < pos + size );
	}

	inline bool Down( int key ) {
		return m_bState[ key ] && m_bPrevState[ key ];
	}

	inline bool Pressed( int key ) {
		return m_bState[ key ] && !m_bPrevState[ key ];
	}

	inline bool Released( int key ) {
		return !m_bState[ key ] && m_bPrevState[ key ];
	}
}