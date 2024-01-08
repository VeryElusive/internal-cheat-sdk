#include "menu.h"

void Menu::Render( ) {
	if ( !Interfaces::GlobalVars )
		return;

	static float lastRealTime{ };
	if ( lastRealTime )
		ctx.m_flFrameTime = Interfaces::GlobalVars->m_flRealTime - lastRealTime;

	lastRealTime = Interfaces::GlobalVars->m_flRealTime;

	if ( !m_bOpened )
		m_pFocusItem.m_pItem = nullptr;

	if ( m_flAlpha >= 0 && !m_bOpened )
		m_flAlpha -= 5.f * ctx.m_flFrameTime;
	else if ( m_flAlpha <= 1 && m_bOpened )
		m_flAlpha += 5.f * ctx.m_flFrameTime;

	m_flAlpha = std::clamp( m_flAlpha, 0.f, 1.f );

	if ( m_flAlpha <= 0 )
		return;

	HandleControls( );

	m_bRendering = true;

	Render::RoundedRectFilled( m_vecPos - Vector2D{ 1, 1 }, m_vecSize, 5, ACCENT );
	Render::RoundedRectFilled( m_vecPos + Vector2D{ 1, 1 }, m_vecSize, 5, ACCENT2 );

	Render::RoundedRectFilled( m_vecPos, m_vecSize, 5, OUTLINE_DARK );
	Render::RoundedRectFilled( m_vecPos + Vector2D{ 1,1 }, m_vecSize - Vector2D{ 2, 2 }, 5, OUTLINE_LIGHT );
	Render::RoundedRectFilled( m_vecPos + 2, m_vecSize - 4, 5, BACKGROUND );

	const auto barSize{ Vector2D{ BAR_SIZE, m_vecSize.y - 4 } };

	Render::RoundedRectFilled( m_vecPos + 1, barSize + 1, 5, OUTLINE_LIGHT );
	Render::RoundedRectFilled( m_vecPos + 2, barSize - 1, 5, OUTLINE_DARK );

	Render::Text( m_vecPos + Vector2D( BAR_SIZE / 2, 20 ), "Havoc", ACCENT, TEXT_CENTER, 13, Render::Fonts.Menu );
	Render::Text( m_vecPos + Vector2D( m_vecSize.x - 85, m_vecSize.y - MARGIN + 2 ), "Developed by", DIM_ELEMENT, TEXT_RIGHT, 13, Render::Fonts.Menu );
	Render::Text( m_vecPos + Vector2D( m_vecSize.x - 56, m_vecSize.y - MARGIN + 2 ), "Artie", ACCENT, TEXT_RIGHT, 13, Render::Fonts.Menu );

	RenderElements( );

	m_bRendering = false;
	m_pFocusItem.m_bFrameAfterFocus = false;
}

void Menu::HandleControls( ) {
	const auto topBarHovered{ Input::Hovered( m_vecPos, { m_vecSize.x, MARGIN } ) };

	if ( !m_bDraggingMenu && Input::Pressed( VK_LBUTTON ) && topBarHovered )
		m_bDraggingMenu = true;

	else if ( m_bDraggingMenu && Input::Down( VK_LBUTTON ) )
		m_vecPos -= Input::m_vecMouseDelta;
	else if ( m_bDraggingMenu && !Input::Down( VK_LBUTTON ) )
		m_bDraggingMenu = false;


	const auto bottomCornerHovered{ Input::Hovered( m_vecPos + m_vecSize - Vector2D( 20, 20 ), Vector2D( 20, 20 ) ) };

	if ( bottomCornerHovered || m_bDraggingSize ) {
		SetCursor( true );

		if ( Input::Down( VK_LBUTTON ) ) {
			m_bDraggingSize = true;
			m_vecSize += Vector2D( Input::m_vecMousePos - ( m_vecPos + m_vecSize ) );
		}
		else
			m_bDraggingSize = false;

	}
	else
		SetCursor( false );

	m_vecSize.x = std::clamp( static_cast< int >( m_vecSize.x ), 570, 1000 );
	m_vecSize.y = std::clamp( static_cast< int >( m_vecSize.y ), 420, 737 );
}

void Menu::SetCursor( bool resize ) {
	/*if ( resize )
		SetCursor( CursorResize );
	else
		SetCursor( CursorArrow );*/
}

void FocusItem_t::Update( CMenuItem* item ) {
	this->m_pItem = item;
	this->m_vecDrawPos = Menu::m_vecDrawPos;
	this->m_bFadeOut = false;
	this->m_bFrameAfterFocus = true;
}