#pragma once
//Thank you c5 for your help!
#include <windows.h>
#include <stdio.h>
#include <gl\GL.h>
#include "../sdk/vector.h"
#include "../sdk/color.h"

enum EAlignment {
	LEFT,
	CENTERED,
	RIGHT
};

class CFont {
public:
	HDC hdc{ };

	bool m_bBuilt{ };
	unsigned int m_iBase{ };
	uint8_t m_iPixelWidth{ };

	TEXTMETRIC m_cTextMetric{ };

	void Setup( const char* fontName, int height, int weight, int quality, int pixelWidth = 0 );
	void Render( Vector2D pos, Color col, const char* text, EAlignment alignment = LEFT );
	Vector2D GetTextSize( const char* text );
};