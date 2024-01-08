#include "../menu.h"

void CMenuItem::TextInputFocus( ) {
	auto& maxGroups{ *reinterpret_cast< int* > ( m_pArgs ) };
	auto& value{ *reinterpret_cast< std::string* > ( m_pValue ) };

	const auto size{ Vector2D( ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * ( maxGroups + 1 ) ) / maxGroups - PADDING * 4, ITEM_HEIGHT * 2.f ) };
	const auto hovered{ Input::Hovered( Menu::m_vecDrawPos, size ) };

	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos - Vector2D( 1, 1 ), size + 2, OUTLINE_DARK );
	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos, size, OUTLINE_LIGHT );
	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos + 1, size - Vector2D( 2, 2 ), OUTLINE_DARK );

	Render::Text( Menu::m_pFocusItem.m_vecDrawPos + Vector2D{ PADDING, 4 }, value, Color( 255, 255, 255 ), 0, 13, Render::Fonts.Menu );

	const auto textSize{ Render::GetTextSize( value.c_str( ), 13, Render::Fonts.Menu ) };

	const auto second{ static_cast< int >( Interfaces::GlobalVars->m_flRealTime ) };
	this->m_cColor = Color( 1.f, 1.f, 1.f, second & 1 ? Interfaces::GlobalVars->m_flRealTime - second : 1.f - ( Interfaces::GlobalVars->m_flRealTime - second ) );
	Render::Line( Menu::m_pFocusItem.m_vecDrawPos + Vector2D{ PADDING + textSize.x, 4 }, Menu::m_pFocusItem.m_vecDrawPos + Vector2D{ PADDING + textSize.x, size.y - 4 }, this->m_cColor );


	if ( Input::Pressed( VK_ESCAPE ) || Input::Pressed( VK_RETURN ) ) {
		Menu::m_pFocusItem.m_pItem = nullptr;
		Menu::m_pFocusItem.m_bFrameAfterFocus = true;
	}
	else if ( Input::Pressed( VK_BACK ) )
		value = value.substr( 0, value.size( ) - 1 );
	else if ( value.size( ) < 110 ) {
		for ( auto i{ 0 }; i < 255; i++ ) {
			if ( isalpha( i ) ) {
				if ( Input::Pressed( i ) ) {
					auto final{ i };
					if ( !GetKeyState( VK_CAPITAL ) && !Input::Down( VK_SHIFT ) )
						final += 32;

					value += final;
				}
			}
		}

		if ( Input::Pressed( VK_SPACE ) )
			value += ' ';
	}

	if ( Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_bFrameAfterFocus && !hovered ) {
		Menu::m_pFocusItem.m_pItem = nullptr;
		Menu::m_pFocusItem.m_bFrameAfterFocus = true;
	}

	Menu::m_vecDrawPos.y += size.y + PADDING;
}

void CMenuItem::TextInput( ) {
	if ( !m_fnShow( ) )
		return;

	auto& maxGroups{ *reinterpret_cast< int* > ( m_pArgs ) };
	auto& value{ *reinterpret_cast< std::string* > ( m_pValue ) };

	const auto size{ Vector2D( ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * ( maxGroups + 1 ) ) / maxGroups - PADDING * 4, ITEM_HEIGHT * 2.f ) };
	const auto hovered{ Input::Hovered( Menu::m_vecDrawPos, size ) };

	if ( hovered && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_bFrameAfterFocus && !Menu::m_pFocusItem.m_pItem )
		Menu::m_pFocusItem.Update( this );

	Render::RectFilled( Menu::m_vecDrawPos - Vector2D( 1, 1 ), size + 2, OUTLINE_DARK );
	Render::RectFilled( Menu::m_vecDrawPos, size, OUTLINE_LIGHT );
	Render::RectFilled( Menu::m_vecDrawPos + 1, size - Vector2D( 2, 2 ), OUTLINE_DARK );

	Render::Text( Menu::m_vecDrawPos + Vector2D{ PADDING, 4 }, value, Color( 255, 255, 255 ), 0, 13, Render::Fonts.Menu );

	Menu::m_vecDrawPos.y += ITEM_HEIGHT * 2.f + PADDING;
}