#include "../menu.h"

void CMenuItem::Button( ) {
	if ( !m_fnShow( ) )
		return;

	auto& args{ *reinterpret_cast< ButtonArgs_t* >( m_pArgs ) };

	const auto size{ Vector2D( ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * ( args.m_iMaxGroups + 1 ) ) / args.m_iMaxGroups - PADDING * 4, ITEM_HEIGHT * 2.f ) };
	const auto hovered{ Input::Hovered( Menu::m_vecDrawPos, size ) };

	Menu::LerpToCol( this->m_cColor, hovered ? Input::Down( VK_LBUTTON ) ? OUTLINE_DARK : DIM_ELEMENT : BACKGROUND );

	if ( hovered && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_bFrameAfterFocus && !Menu::m_pFocusItem.m_pItem )
		args.m_bDown = true;

	if ( args.m_bDown && Input::Released( VK_LBUTTON ) ) {
		args.m_bDown = false;

		// let user drag off and cancel it
		if ( hovered ) {
			if ( args.m_pCallback && args.m_flTimer ) {
				args.m_flTimer = 0.f;
				args.m_pCallback( );
			}
			else if ( !args.m_flTimer )
				args.m_flTimer = 3.f;
		}
	}

	if ( args.m_flTimer )
		args.m_flTimer -= ctx.m_flFrameTime;

	args.m_flTimer = std::max( args.m_flTimer, 0.f );

	Render::RectFilled( Menu::m_vecDrawPos - Vector2D( 1, 1 ), size + 2, OUTLINE_DARK );
	Render::RectFilled( Menu::m_vecDrawPos, size, OUTLINE_LIGHT );
	Render::RectFilled( Menu::m_vecDrawPos + 1, size - Vector2D( 2, 2 ), OUTLINE_DARK );
	Render::RectFilled( Menu::m_vecDrawPos + 2, size - Vector2D( 4, 4 ), this->m_cColor );

	if ( args.m_flTimer )
		Render::Line( Menu::m_vecDrawPos + Vector2D{ 3, size.y - 1 }, Menu::m_vecDrawPos + Vector2D{ ( size.x - 3 ) * ( args.m_flTimer / 3.f ), size.y - 1 }, ACCENT.Alpha( static_cast< uint8_t >( ( args.m_flTimer / 3.f ) * 255.f ) ) );

	Render::Text( Menu::m_vecDrawPos + ( size / 2.f ) - Vector2D{ 0, 8 }, args.m_flTimer ? "Are you sure?" : m_szName, Color( 1.f, 1.f, 1.f ), TEXT_CENTER, 13, Render::Fonts.Menu );

	Menu::m_vecDrawPos.y += ITEM_HEIGHT * 2.f + PADDING;
}