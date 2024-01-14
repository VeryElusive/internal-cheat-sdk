#include "../menu.h"

#define GET_HEIGHT( percent ) static_cast<int>( std::ceil( ( Menu::m_vecSize.y - MARGIN * 2 - ( rowBelow ? MARGIN : 0 ) ) * percent ) )

void CMenuGroup::Render( int groupNum, std::vector < CMenuGroup >& groups ) {
	const auto& groupCount{ groups.size( ) };

	// TODO: properly handle this in the case that we use 3 groups
	const auto rowBelow{ groupNum == 1 ? groupCount > m_iParentMaxGroups && this->m_flPercent < 1.f
		: groupNum == 2 ? ( groupCount > m_iParentMaxGroups && groups.front( ).m_flPercent >= 1.f ) || groupCount >= 4 : groupNum > m_iParentMaxGroups };

	const auto size{ Vector2D{ ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * ( m_iParentMaxGroups + 1 ) ) / m_iParentMaxGroups, GET_HEIGHT( this->m_flPercent ) } };
	const bool hovered{ Input::Hovered( Menu::m_vecDrawPos, size ) };

	// new line
	if ( groupNum > 2 ) {
		if ( groups.front( ).m_flPercent + groups.front( ).m_flPercent > 0.99f ) {
			Menu::m_vecDrawPos.x += size.x + MARGIN;
			Menu::m_vecDrawPos.y += GET_HEIGHT( groups.at( 1 ).m_flPercent ) + MARGIN;
		}
		else {
			Menu::m_vecDrawPos.y += GET_HEIGHT( groups.front( ).m_flPercent ) + MARGIN;
		}
	}


	Render::Rect( Menu::m_vecDrawPos - Vector2D( 1, 1 ), size + Vector2D( 2, 2 ), OUTLINE_DARK );
	Render::Rect( Menu::m_vecDrawPos, size, OUTLINE_LIGHT );
	Render::RectFilled( Menu::m_vecDrawPos + Vector2D( 1, 1 ), size - Vector2D( 2, 2 ), GROUP );

	// TODO:
	//Render::SetClipping( Menu::m_vecDrawPos + 2, size - 4 ); 

	const auto backupDrawPos{ Menu::m_vecDrawPos };
	Menu::m_vecDrawPos.x += PADDING * 2;
	Menu::m_vecDrawPos.y += PADDING * 2;
	Menu::m_vecDrawPos.y -= m_flScroll;


	if ( this->m_vecItems.size( ) ) {
		for ( auto& element : this->m_vecItems )
			element.Render( );
	}

	//Render::SetClipping( { 0,0 }, { 1920, 1080 } );

	const auto outside{ std::max( 0.f, Menu::m_vecDrawPos.y - backupDrawPos.y - size.y + m_flScroll ) };
	if ( hovered ) {
		m_flScroll -= Input::m_flScroll * 12;
		m_flScroll = std::min( outside, m_flScroll );
		m_flScroll = std::max( 0.f, m_flScroll );
	}

	Menu::m_vecDrawPos = backupDrawPos;

	if ( outside ) {
		const auto percent{ m_flScroll / outside };
		const auto barLength{ ( size.y - PADDING * 2 ) * ( 1.f - ( outside / size.y ) ) };
		Render::RectFilled( Menu::m_vecDrawPos + Vector2D( size.x - 3, PADDING * 2 + percent * ( size.y - PADDING * 2 - barLength ) ),
			{ 2, barLength }, ACCENT );
	}

	// TODO:
	//Render::Gradient( Menu::m_vecDrawPos.x + 2, Menu::m_vecDrawPos.y + 2, size.x - 4, PADDING * 2, GROUP, GROUP.Set<COLOR_A>( 0.f ), false );
	//Render::Gradient( Menu::m_vecDrawPos.x + 2, Menu::m_vecDrawPos.y + size.y - PADDING * 2 - 1, size.x - 4 - 1, PADDING * 2, GROUP.Set<COLOR_A>( 0.f ), GROUP, false );

	const auto textX{ Render::GetTextSize( m_szName, 13, Render::Fonts.Menu ).x + ( PADDING * 2 ) };
	Render::RectFilled( Menu::m_vecDrawPos + Vector2D{ PADDING, -1.f }, { textX, 4.f, }, GROUP );

	Render::Text( Menu::m_vecDrawPos + Vector2D{ ( PADDING * 2 ), -4 }, m_szName, Color( 255, 255, 255 ), 0, 13, Render::Fonts.Menu );

	// new line
	if ( groupNum == 2 && groupCount > 2 )
		Menu::m_vecDrawPos.x = Menu::m_vecPos.x + BAR_SIZE + MARGIN;
	else
		Menu::m_vecDrawPos.x += size.x + MARGIN;
}

void CMenuGroup::Reset( ) {
	if ( this->m_vecItems.size( ) ) {
		for ( auto& element : this->m_vecItems ) {
			if ( element.m_eItemType == SLIDER_FLOAT || element.m_eItemType == SLIDER_INT )
				reinterpret_cast< SliderArgs_t* >( element.m_pArgs )->m_flValue = 0;

			element.m_cColor = BACKGROUND;
		}
	}
}

void CMenuItem::RenderFocus( ) {
	switch ( this->m_eItemType ) {
	case SLIDER_FLOAT:
		SliderFloatFocus( );
		break;
	case SLIDER_INT:
		SliderIntFocus( );
		break;
	case COMBO:
		ComboFocus( );
		break;
	case MULTI_COMBO:
		MultiComboFocus( );
		break;
	case COLOR_PICKER:
		ColorPickerFocus( );
		break;
	case TEXT_INPUT:
		TextInputFocus( );
		break;
	case KEY_BIND:
		KeybindFocus( );
		break;
	default:
		break;
	}
}

void CMenuItem::Render( ) {
	switch ( this->m_eItemType ) {
	case CHECKBOX:
		Checkbox( );
		break;
	case SLIDER_FLOAT:
		SliderFloat( );
		break;
	case SLIDER_INT:
		SliderInt( );
		break;
	case COMBO:
		Combo( );
		break;
	case MULTI_COMBO:
		MultiCombo( );
		break;
	case COLOR_PICKER:
		ColorPicker( );
		break;
	case BUTTON:
		Button( );
		break;
	case LIST_BOX:
		Listbox( );
		break;
	case KEY_BIND:
		Keybind( );
		break;
	case TEXT_INPUT:
		TextInput( );
		break;
	case LABEL:
		Label( );
	default:
		break;
	}
}