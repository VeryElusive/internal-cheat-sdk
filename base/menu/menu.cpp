#include "menu.h"

void Menu::Render( ) {
	if ( !m_bOpened )
		m_pFocusItem.m_pItem = nullptr;

	if ( m_flAlpha >= 0 && !m_bOpened )
		m_flAlpha -= 5.f * ctx.m_flElapsed;
	else if ( m_flAlpha <= 1 && m_bOpened )
		m_flAlpha += 5.f * ctx.m_flElapsed;

	m_flAlpha = std::clamp( m_flAlpha, 0.f, 1.f );

	if ( m_flAlpha <= 0 )
		return;

	HandleControls( );

	m_bRendering = true;

	Render::Rect( m_vecPos - Vector2D{ 1, 1 }, m_vecSize, ACCENT );
	Render::Rect( m_vecPos + Vector2D{ 1, 1 }, m_vecSize, ACCENT2 );

	Render::RectFilled( m_vecPos, m_vecSize,  BACKGROUND );
	Render::Rect( m_vecPos, m_vecSize,  OUTLINE_DARK );
	Render::Rect( m_vecPos + Vector2D{ 1,1 }, m_vecSize - Vector2D{ 2, 2 },  OUTLINE_LIGHT );

	const auto barSize{ Vector2D{ BAR_SIZE, m_vecSize.y - 4 } };

	Render::RectFilled( m_vecPos + Vector2D{ 2, 2 }, barSize, OUTLINE_DARK );
	Render::Rect( m_vecPos + Vector2D{ 3, 3 }, barSize - Vector2D{ 2, 2 },  OUTLINE_LIGHT );

	Fonts::Menu.Render( m_vecPos + Vector2D( BAR_SIZE / 2, 20 ), ACCENT, "HAVOC", CENTERED );
	Fonts::Menu.Render( m_vecPos + Vector2D( m_vecSize.x - 120, m_vecSize.y - MARGIN + 2 ), DIM_ELEMENT, "Developed by", RIGHT );
	Fonts::Menu.Render( m_vecPos + Vector2D( m_vecSize.x - 115, m_vecSize.y - MARGIN + 2 ), ACCENT, "Artie", LEFT );

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