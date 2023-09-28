#include "render.h"
#include "../core/displacement.h"
#include "../menu/menu.h"

void Render::BeginDraw( ) {
    GLint viewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, viewport );

    m_vecSize.x = viewport[ 0 ];
    m_vecSize.y = viewport[ 1 ];

    glViewport( 0, 0, viewport[ 2 ], viewport[ 3 ] );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    glOrtho( 0, viewport[ 2 ], viewport[ 3 ], 0, -1, 1 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    glDisable( GL_DEPTH_TEST );
    glEnable( GL_BLEND );

    glDisable( 0x809D ); // disables anti aliasing, microsoft removed the macro, but didnt actually remove the code behind the flag
    glDisable( GL_LINE_SMOOTH );
}

void Render::EndDraw( ) {
    glEnable( GL_DEPTH_TEST );
}

void Render::InitFonts( ) {
    if ( Fonts::Menu.hdc == wglGetCurrentDC( ) )
        return;

    Fonts::Tabs.Setup( "test2", 35, FW_NORMAL, ANTIALIASED_QUALITY );
    Fonts::Menu.Setup( "Bitstream Vera Sans Mono", 10, FW_DONTCARE, NONANTIALIASED_QUALITY );
}

void Render::SetClipping( Vector2D pos, Vector2D size ) {
    glEnable( GL_SCISSOR_TEST );
    glScissor( pos.x, pos.y, size.x, size.y );
}

void Render::Gradient( Vector2D pos, Vector2D size, Color col, Color col2, bool horizontal ) {
    if ( Menu::m_bRendering ) {
        col.a *= Menu::m_flAlpha;
        col2.a *= Menu::m_flAlpha;
    }

    glColor4ub( col.r, col.g, col.b, col.a );

    glBegin( GL_QUADS );

    glVertex2f( pos.x, pos.y );

    if ( !horizontal )
        glColor4ub( col2.r, col2.g, col2.b, col2.a );

    glVertex2f( pos.x + size.x, pos.y );

    if ( horizontal )
        glColor4ub( col2.r, col2.g, col2.b, col2.a );

    glVertex2f( pos.x + size.x, pos.y + size.y );

    if ( !horizontal )
        glColor4ub( col.r, col.g, col.b, col.a );

    glVertex2f( pos.x, pos.y + size.y );

    glEnd( );
}

void Render::Rect( Vector2D pos, Vector2D size, Color col ) {
    if ( Menu::m_bRendering )
        col.a *= Menu::m_flAlpha;

    glColor4ub( col.r, col.g, col.b, col.a );

    glBegin( GL_LINE_LOOP );

    glVertex2f( pos.x, pos.y );
    glVertex2f( pos.x + size.x, pos.y );
    glVertex2f( pos.x + size.x, pos.y + size.y );
    glVertex2f( pos.x, pos.y + size.y );

    glEnd( );
}

void Render::RectFilled( Vector2D pos, Vector2D size, Color col ) {
    if ( Menu::m_bRendering )
        col.a *= Menu::m_flAlpha;

    // GL_QUADS size differs to GL_LINE_LOOP
    //pos -= 1;
    //size.x += 1;
    //size.y += 2;

    glColor4ub( col.r, col.g, col.b, col.a );

    glBegin( GL_QUADS );

    glVertex2f( pos.x, pos.y );
    glVertex2f( pos.x + size.x, pos.y );
    glVertex2f( pos.x + size.x, pos.y + size.y );
    glVertex2f( pos.x, pos.y + size.y );

    glEnd( );
}

void Render::Line( Vector2D pos, Vector2D pos2, Color col ) {
    if ( Menu::m_bRendering )
        col.a *= Menu::m_flAlpha;

    glColor4ub( col.r, col.g, col.b, col.a );

    glBegin( GL_LINES );
    glVertex2f( ( float )pos.x, ( float )pos.y );
    glVertex2f( ( float )pos2.x, ( float )pos2.y );
    glEnd( );
}

void Render::Text( const char* str, Vector2D pos, Color col ) {
    /*int str@<edx>,
        _BYTE *left@<ecx>,
        int top,
        GLubyte r,
        GLubyte g,
        char b,
        GLubyte alpha,
        int cursor,
        int maxwidth
    */

    const auto left{ pos.x };
    const auto alpha{ col.a };
    const auto b{ col.b };
    const auto g{ col.g };
    const auto r{ col.r };
    const auto top{ pos.y };


    // REFERENCE: 68 ? ? ? ? 50 E8 ? ? ? ? 6A FF 6A FF FF 35 ? ? ? ? BA
    __asm {
        push    str;
        push    eax;
        call    Displacement::defformatstring;

        // now call draw_text

        push 0xFFFFFFFF; maxwidth;
        push 0xFFFFFFFF; cursor;
        push alpha;

        mov edx, 0x28;
        lea ecx, left;

        push b;
        push g;
        push r;
        push top;

        call Displacement::draw_text;

        add esp, 0x1C;
    }
}


/*__asm {
        push edx
        push ecx

        mov ecx, left
        mov edx, str

        push -1 ; maxwidth
        push -1 ; cursor
        push alpha
        push b
        push g
        push r
        push top

        mov eax, Displacement::draw_text

        call eax

        add esp, 0x124
    }*/