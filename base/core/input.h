#pragma once
#include "../havoc.h"

enum EKeyMode {
	AlwaysOn,
	Toggle,
	Hold,
	Off
};

struct Keybind_t {
	int m_iKey{ 0 };
	bool m_bEnabled{ };
	bool m_bPrevState{ };
	int m_iMode{ EKeyMode::Hold };

	void Resolve( ) {

		switch ( this->m_iMode ) {
		case EKeyMode::AlwaysOn:
			this->m_bEnabled = true;
			break;
		case EKeyMode::Toggle: {
			bool cock = false;
			cock = GetAsyncKeyState( this->m_iKey );
			if ( cock && !this->m_bPrevState )
				this->m_bEnabled = !this->m_bEnabled;

			this->m_bPrevState = cock;

			this->m_bEnabled;
			break;
		}
		case EKeyMode::Hold:
			this->m_bEnabled = GetAsyncKeyState( this->m_iKey );
			break;
		case EKeyMode::Off:
			this->m_bEnabled = !GetAsyncKeyState( this->m_iKey );
			break;
		}
	}
};


namespace Input {
	inline bool m_bState[ 256 ]{ };
	inline bool m_bPrevState[ 256 ]{ };
	inline float m_flScroll{ };

	inline Vector2D m_vecMousePos{ };
	inline Vector2D m_vecMouseDelta{ };

	void CheckBinds( );

	inline void Update( ) {
		// TODO:
		//if ( GetForegroundWindow( ) != ctx.hwnd )
		//	return;

		// TODO: UpdateNeededKeys like in csgo havoc?
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