#include "../menu.h"
#include "../../core/config.h"

// STATIC IMPLEMENTATION!
// TODO: (only if using it multiple times)- dynamically do it. for now idc :)
void CMenuItem::Listbox( ) {
	auto& value{ *reinterpret_cast< int* >( m_pValue ) };
	auto& scroll{ *reinterpret_cast< float* >( m_pArgs ) };


	//Render::Text( Fonts::Menu, Menu::m_vecDrawPos, Color( 255, 255, 255 ), FONT_LEFT, m_szName );
	//Menu::m_vecDrawPos.y += 16;

	static std::vector< ComboElement_t > configs{ };
	if ( Configs::m_bRefresh ) {
		Configs::m_bRefresh = false;

		configs.clear( );
		for ( auto& config : Configs::m_vecFileNames )
			configs.push_back( config.c_str( ) );
	}
	static float time{ };

	bool refreshed{ };

	// TODO: dynamic sizing
	// TODO: scrolling
	// TODO: clip rects
	const auto size{ Vector2D( ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * 2 ) / 1 - PADDING * 4, Menu::m_vecSize.y - MARGIN * 2 - PADDING * 4 - PADDING * 6 - ITEM_HEIGHT * 2 * 6 ) };

	Render::Rect( Menu::m_vecDrawPos - Vector2D( 1, 1 ), size + Vector2D( 2, 2 ),  OUTLINE_DARK );
	Render::Rect( Menu::m_vecDrawPos, size,  OUTLINE_LIGHT );
	Render::RectFilled( Menu::m_vecDrawPos + Vector2D( 1, 1 ), size - Vector2D( 2, 2 ),  OUTLINE_DARK );

	//Render::SetClipping( Menu::m_vecDrawPos + 2, size - 4 );

	const auto wholeHovered{ Input::Hovered( Menu::m_vecDrawPos, size ) };

	for ( int i{ }; i < configs.size( ); ++i ) {
		auto& element{ configs.at( i ) };

		const auto pos{ Menu::m_vecDrawPos + Vector2D( PADDING, 2 + 15 * i - scroll ) };

		const auto hovered{ Input::Hovered( pos, Vector2D{ size.x, 15 } ) };

		if ( !refreshed )
			Menu::LerpToCol( element.m_cColor, value == i ? ( i & 1 ? ACCENT2 : ACCENT ) : hovered ? HOVERED_ELEMENT : DIM_ELEMENT );
		else
			element.m_cColor = value == i ? ( i & 1 ? ACCENT2 : ACCENT ) : hovered ? HOVERED_ELEMENT : DIM_ELEMENT;

		Render::Text( pos, element.m_szName, element.m_cColor, 0, 13, Render::Fonts.Menu );

		if ( wholeHovered && hovered && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_pItem && !Menu::m_pFocusItem.m_bFrameAfterFocus )
			value = i;
	}

	// TODO:
	//Render::SetClipping( Menu::m_vecDrawPos + 2, size - 4 );

	const float outside{ std::max( 0.f, PADDING + 15 * ( configs.size( ) ) - scroll - size.y + scroll ) };
	if ( outside ) {
		const float percent{ scroll / outside };
		const float barLength{ size.y * ( 1.f - ( outside / size.y ) ) };
		Render::RectFilled( Menu::m_vecDrawPos + Vector2D( size.x - 3, percent * ( size.y - barLength ) ),
			{ 2, barLength }, ACCENT );
	}

	if ( wholeHovered ) {
		scroll -= Input::m_flScroll * 12;
		scroll = std::min( outside, scroll );
		scroll = std::max( 0.f, scroll );
	}

	Menu::m_vecDrawPos.y += size.y + PADDING;
}