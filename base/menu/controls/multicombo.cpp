#include "../menu.h"

#define MAX_ALPHA( col ) col.Alpha( static_cast< uint8_t >( Menu::m_pFocusItem.m_flFocusAnim * col.a ) )

void CMenuItem::MultiComboFocus( ) {
	auto& value{ *reinterpret_cast< int* >( m_pValue ) };
	const auto& args{ *reinterpret_cast< MultiComboArgs_t* >( m_pArgs ) };

	const auto size{ Vector2D( ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * 3 ) / 2 - PADDING * 4, 15 * args.m_iSize + PADDING * 2 ) };

	Render::Rect( Menu::m_pFocusItem.m_vecDrawPos - Vector2D( 1, 1 ), size + 2, MAX_ALPHA( OUTLINE_DARK ) );
	Render::Rect( Menu::m_pFocusItem.m_vecDrawPos, size, MAX_ALPHA( OUTLINE_LIGHT ) );
	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos + 1, size - Vector2D( 2, 2 ), MAX_ALPHA( OUTLINE_DARK ) );

	for ( int i{ }; i < args.m_iSize; ++i ) {
		auto& element{ args.m_pArguments[ i ] };

		const auto hovered{ Input::Hovered( Menu::m_pFocusItem.m_vecDrawPos + Vector2D( PADDING, PADDING + 15 * i ), Vector2D{ size.x, 15 } ) };

		Menu::LerpToCol( element.m_cColor, *element.m_pValue ? ( i & 1 ? ACCENT2 : ACCENT ) : hovered ? HOVERED_ELEMENT : DIM_ELEMENT );

		Render::Text( Menu::m_pFocusItem.m_vecDrawPos + Vector2D( PADDING, PADDING + 15 * i ), element.m_szName, MAX_ALPHA( element.m_cColor ), 0, 13, Render::Fonts.Menu );

		if ( hovered && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_bFrameAfterFocus && !Menu::m_pFocusItem.m_bFadeOut )
			*element.m_pValue = !*element.m_pValue;
	}

	const auto hovered{ Input::Hovered( Menu::m_pFocusItem.m_vecDrawPos, size ) };

	if ( Input::Pressed( VK_LBUTTON ) && !hovered ) {
		Menu::m_pFocusItem.m_bFrameAfterFocus = true;
		Menu::m_pFocusItem.m_bFadeOut = true;
	}
}

#undef MAX_ALPHA

void CMenuItem::MultiCombo( ) {
	if ( !m_fnShow( ) )
		return;

	const auto& value{ *reinterpret_cast< int* >( m_pValue ) };
	const auto& args{ *reinterpret_cast< MultiComboArgs_t* >( m_pArgs ) };

	const auto size{ Vector2D( ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * 3 ) / 2 - PADDING * 4, ITEM_HEIGHT * 1.5f ) };

	Render::Text( Menu::m_vecDrawPos, m_szName, Color( 255, 255, 255 ), 0, 13, Render::Fonts.Menu );
	Menu::m_vecDrawPos.y += 16;
	const auto hovered{ Input::Hovered( Menu::m_vecDrawPos, size ) };

	Render::Rect( Menu::m_vecDrawPos - Vector2D( 1, 1 ), size + 2, OUTLINE_DARK );
	Render::Rect( Menu::m_vecDrawPos, size, OUTLINE_LIGHT );
	Render::RectFilled( Menu::m_vecDrawPos + 1, size - Vector2D( 2, 2 ), OUTLINE_DARK );

	std::string accumulatedValues{ };
	for ( int i{ }; i < args.m_iSize; ++i ) {
		if ( *args.m_pArguments[ i ].m_pValue ) {
			accumulatedValues += args.m_pArguments[ i ].m_szName;
			accumulatedValues += ", ";
		}
	}

	if ( accumulatedValues.size( ) ) {
		accumulatedValues.pop_back( );
		accumulatedValues.pop_back( );
	}
	else
		accumulatedValues = "None";

	if ( accumulatedValues.length( ) * 7 > size.x - PADDING ) {
		accumulatedValues.erase( ( size.x - 30 ) / 7 + 4, accumulatedValues.length( ) - ( size.x - 30 ) / 7 + 4 );
		accumulatedValues.append( "..." );
	}

	Render::Text( Menu::m_vecDrawPos + Vector2D{ PADDING, 2 }, accumulatedValues, Color( 255, 255, 255 ), 0, 13, Render::Fonts.Menu );

	if ( hovered && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_bFrameAfterFocus && !Menu::m_pFocusItem.m_pItem )
		Menu::m_pFocusItem.Update( this );

	Menu::m_vecDrawPos.y += ITEM_HEIGHT * 1.5f + PADDING;
}