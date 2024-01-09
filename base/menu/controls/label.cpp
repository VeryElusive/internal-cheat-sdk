#include "../menu.h"

void CMenuItem::Label( ) {
	if ( !m_fnShow( ) )
		return;

	const auto size{ Vector2D{ ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * 3 ) / 2 - PADDING * 2, 16 } };
	const auto hovered{ Input::Hovered( Menu::m_vecDrawPos, size ) };

	Menu::LerpToCol( this->m_cColor, hovered ? Color( 255, 255, 255 ) : SELECTED_ELEMENT );

	Render::Text( Menu::m_vecDrawPos + Vector2D( 0, -2 ), m_szName, this->m_cColor, TEXT_LEFT, 13, Render::Fonts.Menu );

	Menu::m_vecDrawPos.y += ITEM_HEIGHT + PADDING;
}