#pragma once
#include "../../havoc.h"
#include "../utils.h"

#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_internal.h"

#define MAINVP 0
#define SAFE_RELEASE(p) if (p) { p->Release(); p = nullptr; } 
#define PI 3.141592654f

enum ETextFlags {
	TEXT_LEFT = ( 1 << 0 ),
	TEXT_CENTER = ( 1 << 1 ),
	TEXT_RIGHT = ( 1 << 2 ),

	TEXT_DROPSHADOW = ( 1 << 3 ),
	TEXT_OUTLINE = ( 1 << 4 )
};

// TODO: rename everything
namespace Render {
	struct _Fonts {
		ImFont* Menu{ };
		ImFont* Tabs{ };
	}; inline _Fonts Fonts;

	inline void** MethodsTable = NULL;

	inline D3D11_VIEWPORT pViewports[ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ]{ 0 };

	void Init( );
	void Unload( );
	bool PrepareMethodTable( );

	// wrappers
	void Line( Vector2D pos, Vector2D pos2, Color col );
	void Rect( Vector2D pos, Vector2D size, Color col );
	void RectFilled( Vector2D pos, Vector2D size, Color col );
	void Gradient( Vector2D pos, Vector2D size, Color col, Color col2, bool horizontal );
	void Circle( Vector2D pos, float radius, const Color& color );
	void CircleFilled( Vector2D pos, float radius, const Color& color );
	void Text( const Vector2D& pos, const std::string& text, const Color& color, std::uint8_t flags, float fontSize, const ImFont* font );
	Vector2D GetTextSize( const std::string& text, float fontSize, const ImFont* font );
	// keep radius an even number
	void RoundedRectFilled( Vector2D pos, Vector2D size, int radius, Color col );
	void RoundedRect( Vector2D pos, Vector2D size, Color col );
}