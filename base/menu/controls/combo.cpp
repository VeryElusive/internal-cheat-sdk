#include "../menu.h"

#define MAX_ALPHA( col ) col.Alpha( Menu::m_pFocusItem.m_flFocusAnim * 255 )

// TODO: MAKE THESE USE GROUPMAXSIZE
void CMenuItem::ComboFocus( ) {
	auto& value{ *reinterpret_cast< int* >( m_pValue ) };
	const auto& args{ *reinterpret_cast< ComboArgs_t* >( m_pArgs ) };

	const auto size{ Vector2D( ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * 3 ) / 2 - PADDING * 4, 15 * args.m_iSize + PADDING * 2 ) };

	Render::Rect( Menu::m_pFocusItem.m_vecDrawPos - Vector2D( 1, 1 ), size + Vector2D( 2, 2 ), MAX_ALPHA( OUTLINE_DARK ) );
	Render::Rect( Menu::m_pFocusItem.m_vecDrawPos, size, MAX_ALPHA( OUTLINE_LIGHT ) );
	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos + Vector2D( 1, 1 ), size - Vector2D( 2, 2 ), MAX_ALPHA( OUTLINE_DARK ) );

	for ( int i{ }; i < args.m_iSize; ++i ) {
		auto& element{ args.m_pArguments[ i ] };

		const auto hovered{ Input::Hovered( Menu::m_pFocusItem.m_vecDrawPos + Vector2D( PADDING, PADDING + 15 * i ), Vector2D{ size.x, 15 } ) };

		Menu::LerpToCol( element.m_cColor, value == i ? ( i & 1 ? ACCENT2 : ACCENT ) : hovered ? HOVERED_ELEMENT : DIM_ELEMENT );

		Render::Text( Menu::m_pFocusItem.m_vecDrawPos + Vector2D( PADDING, PADDING + 15 * i ), element.m_szName, MAX_ALPHA( element.m_cColor ), 0 FONT_MENU );

		if ( hovered && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_bFrameAfterFocus && !Menu::m_pFocusItem.m_bFadeOut )
			value = i;
	}

	const auto hovered{ Input::Hovered( Menu::m_pFocusItem.m_vecDrawPos, size ) };

	if ( Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_bFrameAfterFocus ) {
		Menu::m_pFocusItem.m_bFrameAfterFocus = true;
		Menu::m_pFocusItem.m_bFadeOut = true;
	}
}

#undef MAX_ALPHA

void CMenuItem::Combo( ) {
	if ( !m_fnShow( ) )
		return;

	const auto& value{ *reinterpret_cast< int* >( m_pValue ) };
	const auto& args{ *reinterpret_cast< ComboArgs_t* >( m_pArgs ) };

	const auto size{ Vector2D( ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * 3 ) / 2 - PADDING * 4, ITEM_HEIGHT * 1.5f ) };

	Render::Text( Menu::m_vecDrawPos, m_szName, Color( 255, 255, 255 ), 0 FONT_MENU );
	Menu::m_vecDrawPos.y += 16;
	const auto hovered{ Input::Hovered( Menu::m_vecDrawPos, size ) };

	Render::Rect( Menu::m_vecDrawPos - Vector2D( 1, 1 ), size + Vector2D( 2, 2 ), OUTLINE_DARK );
	Render::Rect( Menu::m_vecDrawPos, size, OUTLINE_LIGHT );
	Render::RectFilled( Menu::m_vecDrawPos + Vector2D( 1, 1 ), size - Vector2D( 2, 2 ), OUTLINE_DARK );

	const auto& element{ args.m_pArguments[ value ] };
	Render::Text( Menu::m_vecDrawPos + Vector2D{ PADDING, 2 }, element.m_szName, Color( 255, 255, 255 ), 0 FONT_MENU );

	if ( hovered && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_pItem )
		Menu::m_pFocusItem.Update( this );

	Menu::m_vecDrawPos.y += ITEM_HEIGHT * 1.5f + PADDING;
}