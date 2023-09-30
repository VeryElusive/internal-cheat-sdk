#include "../menu.h"
#include <string>

void CMenuItem::SliderFloatFocus( ) {
	auto& value{ *reinterpret_cast< float* >( m_pValue ) };
	auto& args{ *reinterpret_cast< SliderArgs_t* >( m_pArgs ) };

	const auto size{ Vector2D( ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * ( args.m_iMaxGroups + 1 ) ) / args.m_iMaxGroups - PADDING * 4, ITEM_HEIGHT ) };

	const auto sliderRatio{ Menu::mapNumber( args.m_flValue, args.m_flMin, args.m_flMax, 0.f, 1.f ) };
	Render::Text( Menu::m_pFocusItem.m_vecDrawPos, m_szName, Color( 255, 255, 255 ), 0 FONT_MENU );

	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos - Vector2D( 1, 1 ), size + 2, OUTLINE_DARK );
	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos, size, OUTLINE_LIGHT );
	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos + 1, size - Vector2D( 2, 2 ), OUTLINE_DARK );

	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos + 2, Vector2D( std::max( std::ceil( ( size.x - 4 ) * sliderRatio ), 8.f ), size.y - 4 ), this->m_cColor );

	Render::Text( Menu::m_pFocusItem.m_vecDrawPos + Vector2D( size.x / 2, -1 ), std::to_string( static_cast< int >( value ) ), Color( 255, 255, 255 ), FW1_CENTER FONT_MENU );
}

void CMenuItem::SliderFloat( ) {
	auto& args{ *reinterpret_cast< SliderArgs_t* >( m_pArgs ) };

	if ( !m_fnShow( ) ) {
		args.m_flValue = 0.f;
		return;
	}

	auto& value{ *reinterpret_cast< float* >( m_pValue ) };

	const auto size{ Vector2D( ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * ( args.m_iMaxGroups + 1 ) ) / args.m_iMaxGroups - PADDING * 4, ITEM_HEIGHT ) };

	value = std::clamp( value, args.m_flMin, args.m_flMax );

	Render::Text( Menu::m_vecDrawPos, m_szName, Color( 255, 255, 255 ), 0 FONT_MENU );
	Menu::m_vecDrawPos.y += 16;
	const auto hovered{ Input::Hovered( Menu::m_vecDrawPos, size ) };

	args.m_flValue = Math::Interpolate( args.m_flValue, value, ANIMATION_SPEED );

	Menu::LerpToCol( this->m_cColor, Menu::m_pFocusItem.m_pItem == this ? ACCENT : hovered ? HOVERED_ELEMENT : DIM_ELEMENT );

	if ( Menu::m_pFocusItem.m_pItem == this ) {
		const auto offset{ std::clamp( Vector2D( Input::m_vecMousePos - Menu::m_vecDrawPos ).x, 0.f, size.x ) };
		value = Menu::mapNumber( offset, 0, size.x, args.m_flMin, args.m_flMax );

		if ( Input::Released( VK_LBUTTON ) )
			Menu::m_pFocusItem.m_pItem = nullptr;
	}
	else {
		if ( !Menu::m_pFocusItem.m_pItem && hovered && !Menu::m_pFocusItem.m_bFrameAfterFocus && !Menu::m_pFocusItem.m_pItem ) {
			if ( Input::Pressed( VK_LBUTTON ) )
				Menu::m_pFocusItem.Update( this );
		}

		const auto sliderRatio{ Menu::mapNumber( args.m_flValue, args.m_flMin, args.m_flMax, 0.f, 1.f ) };

		Render::RectFilled( Menu::m_vecDrawPos - Vector2D( 1, 1 ), size + 2, OUTLINE_DARK );
		Render::RectFilled( Menu::m_vecDrawPos, size, OUTLINE_LIGHT );
		Render::RectFilled( Menu::m_vecDrawPos + 1, size - Vector2D( 2, 2 ), OUTLINE_DARK );

		Render::RectFilled( Menu::m_vecDrawPos + 2, Vector2D( std::max( std::ceil( ( size.x - 4 ) * sliderRatio ), 8.f ), size.y - 4 ), this->m_cColor );

		Render::Text( Menu::m_vecDrawPos + Vector2D( size.x / 2, -1 ), std::to_string( static_cast< int >( value ) ), Color( 255, 255, 255 ), FW1_CENTER FONT_MENU );
	}

	Menu::m_vecDrawPos.y += ITEM_HEIGHT + PADDING;
}

void CMenuItem::SliderIntFocus( ) {
	auto& value{ *reinterpret_cast< int* >( m_pValue ) };
	auto& args{ *reinterpret_cast< SliderArgs_t* >( m_pArgs ) };

	const auto size{ Vector2D( ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * ( args.m_iMaxGroups + 1 ) ) / args.m_iMaxGroups - PADDING * 4, ITEM_HEIGHT ) };

	const auto sliderRatio{ Menu::mapNumber( args.m_flValue, args.m_flMin, args.m_flMax, 0.f, 1.f ) };
	Render::Text( Menu::m_pFocusItem.m_vecDrawPos, m_szName, Color( 255, 255, 255 ), 0 FONT_MENU );

	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos - Vector2D( 1, 1 ), size + 2, OUTLINE_DARK );
	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos, size, OUTLINE_LIGHT );
	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos + 1, size - Vector2D( 2, 2 ), OUTLINE_DARK );

	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos + 2, Vector2D( std::max( std::ceil( ( size.x - 4 ) * sliderRatio ), 8.f ), size.y - 4 ), this->m_cColor );

	Render::Text( Menu::m_pFocusItem.m_vecDrawPos + Vector2D( size.x / 2, -1 ), std::to_string( static_cast< int >( value ) ), Color( 255, 255, 255 ), FW1_CENTER FONT_MENU );
}

void CMenuItem::SliderInt( ) {
	auto& args{ *reinterpret_cast< SliderArgs_t* >( m_pArgs ) };

	if ( !m_fnShow( ) ) {
		args.m_flValue = 0.f;
		return;
	}

	auto& value{ *reinterpret_cast< int* >( m_pValue ) };

	const auto size{ Vector2D( ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * ( args.m_iMaxGroups + 1 ) ) / args.m_iMaxGroups - PADDING * 4, ITEM_HEIGHT ) };

	value = std::clamp<int>( value, args.m_flMin, args.m_flMax );

	Render::Text( Menu::m_vecDrawPos, m_szName, Color( 255, 255, 255 ), 0 FONT_MENU );
	Menu::m_vecDrawPos.y += 16;
	const auto hovered{ Input::Hovered( Menu::m_vecDrawPos, size ) };

	args.m_flValue = Math::Interpolate( args.m_flValue, value, ANIMATION_SPEED );

	Menu::LerpToCol( this->m_cColor, Menu::m_pFocusItem.m_pItem == this ? ACCENT : hovered ? HOVERED_ELEMENT : DIM_ELEMENT );

	if ( Menu::m_pFocusItem.m_pItem == this ) {
		const auto offset{ std::clamp( Vector2D( Input::m_vecMousePos - Menu::m_vecDrawPos ).x, 0.f, size.x ) };
		value = Menu::mapNumber( offset, 0, size.x, args.m_flMin, args.m_flMax );

		if ( Input::Released( VK_LBUTTON ) )
			Menu::m_pFocusItem.m_pItem = nullptr;
	}
	else {
		if ( !Menu::m_pFocusItem.m_pItem && hovered && !Menu::m_pFocusItem.m_bFrameAfterFocus && !Menu::m_pFocusItem.m_pItem ) {
			if ( Input::Pressed( VK_LBUTTON ) )
				Menu::m_pFocusItem.Update( this );
		}

		const auto sliderRatio{ Menu::mapNumber( args.m_flValue, args.m_flMin, args.m_flMax, 0.f, 1.f ) };

		Render::RectFilled( Menu::m_vecDrawPos - Vector2D( 1, 1 ), size + 2, OUTLINE_DARK );
		Render::RectFilled( Menu::m_vecDrawPos, size, OUTLINE_LIGHT );
		Render::RectFilled( Menu::m_vecDrawPos + 1, size - Vector2D( 2, 2 ), OUTLINE_DARK );

		Render::RectFilled( Menu::m_vecDrawPos + 2, Vector2D( std::max( std::ceil( ( size.x - 4 ) * sliderRatio ), 8.f ), size.y - 4 ), this->m_cColor );

		Render::Text( Menu::m_vecDrawPos + Vector2D( size.x / 2, -1 ), std::to_string( static_cast< int >( value ) ), Color( 255, 255, 255 ), FW1_CENTER FONT_MENU );
	}

	Menu::m_vecDrawPos.y += ITEM_HEIGHT + PADDING;
}