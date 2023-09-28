#include "../menu.h"

void CMenuItem::Checkbox( ) {
	if ( !m_fnShow( ) )
		return;

	auto& value{ *reinterpret_cast< bool* >( m_pValue ) };
	const auto size{ Vector2D{ ITEM_HEIGHT, ITEM_HEIGHT } };
	const auto hovered{ Input::Hovered( Menu::m_vecDrawPos, size + Vector2D{ Fonts::Menu.GetTextSize( m_szName ).x + ITEM_HEIGHT * 2, 0 } ) };

	if ( hovered && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_pItem && !Menu::m_pFocusItem.m_bFrameAfterFocus )
		value = !value;

	Menu::LerpToCol( this->m_cColor, value ? ACCENT : hovered ? HOVERED_ELEMENT : BACKGROUND );

	Render::RectFilled( Menu::m_vecDrawPos - Vector2D( 1, 1 ), size + 2, OUTLINE_DARK );
	Render::RectFilled( Menu::m_vecDrawPos, size, OUTLINE_LIGHT );
	Render::RectFilled( Menu::m_vecDrawPos + 1, size - Vector2D( 2, 2 ), OUTLINE_DARK );
	Render::RectFilled( Menu::m_vecDrawPos + 2, size - Vector2D( 4, 4 ), this->m_cColor );

	Fonts::Menu.Render( Menu::m_vecDrawPos + Vector2D( ITEM_HEIGHT * 2, 0 ), Color( 255, 255, 255 ), m_szName, LEFT );

	Menu::m_vecDrawPos.y += ITEM_HEIGHT + PADDING;
}