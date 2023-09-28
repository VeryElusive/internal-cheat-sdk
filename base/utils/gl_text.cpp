#pragma once
#include "gl_text.h"
#include "../menu/menu.h"
#include <GL/GLU.h>

void CFont::Setup( const char* fontName, int height, int weight, int quality, int pixelWidth ) {
	hdc = wglGetCurrentDC( );
	m_iBase = glGenLists( 96 );
	HFONT hFont = CreateFont( -( height ), 0, 0, 0, weight, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, quality, FF_DONTCARE | DEFAULT_PITCH, fontName );
	HFONT hOldFont = ( HFONT ) SelectObject( hdc, hFont );
	wglUseFontBitmaps( hdc, 32, 96, m_iBase );
	GetTextMetrics( hdc, &m_cTextMetric );
	SelectObject( hdc, hOldFont );
	DeleteObject( hFont );

	m_iPixelWidth = pixelWidth;

	m_bBuilt = true;
}

Vector2D CFont::GetTextSize( const char* text ) {
	return Vector2D{ ( static_cast<int>( m_cTextMetric.tmAveCharWidth ) * static_cast< int >( strlen( text ) ) ), static_cast< int >( m_cTextMetric.tmHeight ) };
}

void CFont::Render( Vector2D pos, Color col, const char* text, EAlignment alignment ) {
	// magic number: render offset is WAY different
	//pos.x += 51;
	//pos.y += 40;

	pos.y += 10;

	if ( Menu::m_bRendering )
		col.a *= Menu::m_flAlpha;

	if ( alignment == RIGHT )
		pos.x -= ( m_cTextMetric.tmAveCharWidth * strlen( text ) );
	else if ( alignment == CENTERED )
		pos.x -= ( m_cTextMetric.tmAveCharWidth * strlen( text ) ) / 2.f;

	glShadeModel( GL_SMOOTH );
	glColor4ub( col.r, col.g, col.b, col.a );
	glRasterPos2f( pos.x, pos.y );

	glPushAttrib( GL_LIST_BIT );
	glListBase( m_iBase - 32 );
	glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );
	glPopAttrib( );
}