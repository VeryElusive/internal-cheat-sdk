#include "render.h"
#include "../../menu/menu.h"
#include "../xorstr.h"

#include "../../dependencies/imgui/imstb_truetype.h"

#include "../../dependencies/imgui/backends/imgui_impl_dx11.h"
#include "../../dependencies/imgui/backends/imgui_impl_win32.h"

#define GET_DRAWLIST const auto drawlist{ ImGui::GetBackgroundDrawList( ) }; if ( drawlist ) drawlist

void Render::Line( Vector2D pos, Vector2D pos2, Color col ) {
    if ( Menu::m_bRendering && Menu::m_flAlpha < 1.f )
        col = col.Alpha( col.a * Menu::m_flAlpha );

    // drawlist allows for thickness!!!
    GET_DRAWLIST->AddLine( { pos.x, pos.y }, { pos2.x, pos2.y }, col.ToUInt32( ) );
}

// we can do stroke thickness if we just draw rect filled instead of lines
void Render::Rect( Vector2D pos, Vector2D size, Color col ) {
    if ( Menu::m_bRendering && Menu::m_flAlpha < 1.f )
        col = col.Alpha( col.a * Menu::m_flAlpha );

    GET_DRAWLIST->AddRect( { pos.x, pos.y }, { pos.x + size.x, pos.y + size.y }, col.ToUInt32( ) );
}

void Render::RectFilled( Vector2D pos, Vector2D size, Color col ) {
    if ( Menu::m_bRendering && Menu::m_flAlpha < 1.f )
        col = col.Alpha( col.a * Menu::m_flAlpha );

    GET_DRAWLIST->AddRectFilled( { pos.x, pos.y }, { pos.x + size.x, pos.y + size.y }, col.ToUInt32( ) );
}

// if you ever need to use this and it's fucked, the last param of AddCircle/AddCircleFilled is segments and is defaulted to 0
void Render::CircleFilled( Vector2D pos, float radius, Color col ) {
    if ( Menu::m_bRendering && Menu::m_flAlpha < 1.f )
        col = col.Alpha( col.a * Menu::m_flAlpha );

    GET_DRAWLIST->AddCircleFilled( { pos.x, pos.y }, radius, col.ToUInt32( ) );
}

void Render::Circle( Vector2D pos, float radius, Color col ) {
    if ( Menu::m_bRendering && Menu::m_flAlpha < 1.f )
        col = col.Alpha( col.a * Menu::m_flAlpha );

    GET_DRAWLIST->AddCircle( { pos.x, pos.y }, radius, col.ToUInt32( ) );
}

Vector2D Render::GetTextSize( const std::string& text, float fontSize, const ImFont* font ) {
    const auto ret{ font->CalcTextSizeA( fontSize, FLT_MAX, 0.f, text.c_str( ) ) };
    return { ret.x, ret.y };
}

void Render::Text( const Vector2D& pos, const std::string& text, Color col, std::uint8_t flags, float fontSize, const ImFont* font ) {
    if ( Menu::m_bRendering && Menu::m_flAlpha < 1.f )
        col = col.Alpha( col.a * Menu::m_flAlpha );

    const auto drawlist{ ImGui::GetBackgroundDrawList( ) };

    // set font texture
    drawlist->PushTextureID( font->ContainerAtlas->TexID );

    const ImU32 colOutlinePacked{ Color( 10u, 10u, 10u, col.a ).ToUInt32( ) };

    ImVec2 recastPos{ pos.x, pos.y - 1 };

    if ( flags & TEXT_RIGHT )
        recastPos.x -= GetTextSize( text, fontSize, font ).x;

    if ( flags & TEXT_CENTER )
        recastPos.x -= GetTextSize( text, fontSize, font ).x / 2;

    if ( flags & TEXT_DROPSHADOW )
        drawlist->AddText( font, fontSize, recastPos + ImVec2( 1.f, -1.f ), colOutlinePacked, text.c_str( ) );
    else if ( flags & TEXT_OUTLINE ) {
        drawlist->AddText( font, fontSize, recastPos + ImVec2( 1.f, -1.f ), colOutlinePacked, text.c_str( ) );
        drawlist->AddText( font, fontSize, recastPos + ImVec2( -1.f, 1.f ), colOutlinePacked, text.c_str( ) );
    }

    drawlist->AddText( font, fontSize, recastPos, col.ToUInt32( ), text.c_str( ) );
    drawlist->PopTextureID( );
}

void Render::RoundedRectFilled( Vector2D pos, Vector2D size, int radius, Color col ) {
    if ( Menu::m_bRendering && Menu::m_flAlpha < 1.f )
        col = col.Alpha( col.a * Menu::m_flAlpha );

    GET_DRAWLIST->AddRectFilled( { pos.x, pos.y }, { pos.x + size.x, pos.y + size.y }, col.ToUInt32( ), radius );
}

void Render::RoundedRect( Vector2D pos, Vector2D size, int radius, Color col ) {
    if ( Menu::m_bRendering && Menu::m_flAlpha < 1.f )
        col = col.Alpha( col.a * Menu::m_flAlpha );

    GET_DRAWLIST->AddRect( { pos.x, pos.y }, { pos.x + size.x, pos.y + size.y }, col.ToUInt32( ), radius );
}


void Render::Gradient( Vector2D pos, Vector2D size, Color col, Color col2, bool horizontal ) {
    if ( Menu::m_bRendering && Menu::m_flAlpha < 1.f )
        col = col.Alpha( col.a * Menu::m_flAlpha );

    GET_DRAWLIST->AddRectFilledMultiColor( { pos.x, pos.y }, { pos.x + size.x, pos.y + size.y }, 
        col.ToUInt32( ), 
        horizontal ? col2.ToUInt32( ) : col.ToUInt32( ),
        col2.ToUInt32( ),
        horizontal ? col.ToUInt32( ) : col2.ToUInt32( ) );
}

bool Render::WorldToScreen( const Vector& world, Vector2D& screen ) {
    const float flWidth{ ctx.m_matViewMatrix[ 3 ][ 0 ] * world.x + ctx.m_matViewMatrix[ 3 ][ 1 ] * world.y + ctx.m_matViewMatrix[ 3 ][ 2 ] * world.z + ctx.m_matViewMatrix[ 3 ][ 3 ] };

    // check is point can't fit on screen, because it's behind us
    if ( flWidth < 0.001f )
        return false;

    // compute the scene coordinates of a point in 3D
    const float flInverse = 1.0f / flWidth;
    screen.x = ( ctx.m_matViewMatrix[ 0 ][ 0 ] * world.x + ctx.m_matViewMatrix[ 0 ][ 1 ] * world.y + ctx.m_matViewMatrix[ 0 ][ 2 ] * world.z + ctx.m_matViewMatrix[ 0 ][ 3 ] ) * flInverse;
    screen.y = ( ctx.m_matViewMatrix[ 1 ][ 0 ] * world.x + ctx.m_matViewMatrix[ 1 ][ 1 ] * world.y + ctx.m_matViewMatrix[ 1 ][ 2 ] * world.z + ctx.m_matViewMatrix[ 1 ][ 3 ] ) * flInverse;

    // screen transform
    // get the screen position in pixels of given point
    const ImVec2 vecDisplaySize{ ImGui::GetIO( ).DisplaySize };
    screen.x = ( vecDisplaySize.x * 0.5f ) + ( screen.x * vecDisplaySize.x ) * 0.5f;
    screen.y = ( vecDisplaySize.y * 0.5f ) - ( screen.y * vecDisplaySize.y ) * 0.5f;
    return true;
}


void Render::Init( ) {
    ImGuiIO& io{ ImGui::GetIO( ) };

    Fonts.Menu = io.Fonts->AddFontFromFileTTF( _( "C:\\Windows\\Fonts\\Tahoma.ttf" ), 13.f, nullptr, io.Fonts->GetGlyphRangesCyrillic( ) );
    Fonts.Tabs = io.Fonts->AddFontFromFileTTF( _( "C:\\test2.ttf" ), 50.f, nullptr, io.Fonts->GetGlyphRangesDefault( ) );

    //io.Fonts->Build( );
}

void Render::Unload( ) {
    ImGui_ImplDX11_Shutdown( );
    ImGui_ImplWin32_Shutdown( );

    // destroy imgui context
    ImGui::DestroyContext( );
}