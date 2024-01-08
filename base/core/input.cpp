/*#include "input.h"

void Input::Update( ) {
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

	m_vecMouseDelta = m_vecMousePos - (Vector2D( mouse.x, mouse.y ) );

	m_vecMousePos = Vector2D( mouse.x, mouse.y );
}

bool Input::Hovered( Vector2D pos, Vector2D size ) {
	return ( m_vecMousePos > pos && m_vecMousePos < pos + size );
}

bool Input::Down( int key ) {
	return m_bState[ key ] && m_bPrevState[ key ];
}

bool Input::Pressed( int key ) {
	return m_bState[ key ] && !m_bPrevState[ key ];
}

bool Input::Released( int key ) {
	return !m_bState[ key ] && m_bPrevState[ key ];
}
*/