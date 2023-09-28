#pragma once
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "../sdk/color.h"
#include "../sdk/vector.h"
#include "math.h"
#include "gl_text.h"

#define ANTISCALING 1.7099f

namespace Fonts {
	inline CFont Menu{ };
	inline CFont Tabs{ };
}

namespace Render {
	inline Vector2D m_vecSize{ };
	void BeginDraw( );
	void EndDraw( );
	void InitFonts( );

	void SetClipping( Vector2D pos, Vector2D size );
	void Rect( Vector2D pos, Vector2D size, Color col );
	void Gradient( Vector2D pos, Vector2D size, Color col, Color col2, bool horizontal );
	void RectFilled( Vector2D pos, Vector2D size, Color col );
	void Line( Vector2D pos, Vector2D pos2, Color col );
	void Text( const char* str, Vector2D pos, Color col );
}