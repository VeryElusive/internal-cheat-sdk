#include "../menu.h"

struct hsv_t {
	hsv_t( float hue, float sat, float val ) {
		this->hue = hue;
		this->sat = sat;
		this->val = val;
	}

	hsv_t( ) {
		hue = 1.f;
		sat = 1.f;
		val = 1.f;
	}
	float hue;
	float sat;
	float val;
};

Color hsv2rgb( float hue, float sat, float val ) {
	float red{ }, grn{ }, blu{ };
	float i, f, p, q, t;
	Color result( 0, 0, 0 );

	if ( val == 0 ) {
		red = 0;
		grn = 0;
		blu = 0;
	}
	else {
		hue /= 60.f;
		i = floor( hue );
		f = hue - i;
		p = val * ( 1 - sat );
		q = val * ( 1 - ( sat * f ) );
		t = val * ( 1 - ( sat * ( 1 - f ) ) );
		if ( i == 0 ) { red = val; grn = t; blu = p; }
		else if ( i == 1 ) { red = q; grn = val; blu = p; }
		else if ( i == 2 ) { red = p; grn = val; blu = t; }
		else if ( i == 3 ) { red = p; grn = q; blu = val; }
		else if ( i == 4 ) { red = t; grn = p; blu = val; }
		else if ( i == 5 ) { red = val; grn = p; blu = q; }
	}

	result.r = red * 255.f;
	result.g = grn * 255.f;
	result.b = blu * 255.f;
	result.a = 255.f;

	return result;
}

hsv_t rgb2hsv( Color a ) {
	float red, grn, blu;
	red = ( float ) a.r / 255.f;
	grn = ( float ) a.g / 255.f;
	blu = ( float ) a.b / 255.f;
	float hue, sat, val;
	float x, f, i;
	hsv_t result{ };

	x = std::min( std::min( red, grn ), blu );
	val = std::max( std::max( red, grn ), blu );
	if ( x == val ) {
		hue = 0;
		sat = 0;
	}
	else {
		f = ( red == x ) ? grn - blu : ( ( grn == x ) ? blu - red : red - grn );
		i = ( red == x ) ? 3 : ( ( grn == x ) ? 5 : 1 );
		hue = fmod( ( i - f / ( val - x ) ) * 60, 360 );
		sat = ( ( val - x ) / val );
	}
	result.hue = hue;
	result.sat = sat;
	result.val = val;
	return result;
}


#define MAX_ALPHA( col ) col.Alpha( Menu::m_pFocusItem.m_flFocusAnim * col.a )
void CMenuItem::ColorPickerFocus( ) {
	auto& value{ *reinterpret_cast< Color* >( m_pValue ) };
	auto& args{ *reinterpret_cast< ColorPickerArgs_t* >( m_pArgs ) };

	const auto size{ Vector2D{ 200, 200 } };

	Render::Rect( Menu::m_pFocusItem.m_vecDrawPos - Vector2D( 1, 1 ), size + Vector2D( 2, 2 ),  MAX_ALPHA( OUTLINE_DARK ) );
	Render::Rect( Menu::m_pFocusItem.m_vecDrawPos, size,  MAX_ALPHA( OUTLINE_LIGHT ) );
	Render::RectFilled( Menu::m_pFocusItem.m_vecDrawPos + Vector2D( 1, 1 ), size - Vector2D( 2, 2 ),  MAX_ALPHA( OUTLINE_DARK ) );

	const auto hueDrawPos{ Menu::m_pFocusItem.m_vecDrawPos + Vector2D( 170, 10 ) };
	for ( int i = 0; i < 150; i++ ) {
		float hue = ( ( float ) i / 150 ) * 360.f;
		Color hue_color = hsv2rgb( hue, 1.f, 1.f );

		Render::Line( hueDrawPos + Vector2D( 0, i ), hueDrawPos + Vector2D( 20, i ), MAX_ALPHA( hue_color ) );
	}

	Render::RectFilled( Vector2D{ hueDrawPos.x - 1, hueDrawPos.y + ( args.m_flHue / 359.f ) * 150.f }, Vector2D{ 22, 3 }, MAX_ALPHA( OUTLINE_DARK ) );
	Render::Line( Vector2D{ hueDrawPos.x, hueDrawPos.y + ( args.m_flHue / 359.f ) * 150.f + 1 }, Vector2D{ hueDrawPos.x + 20, hueDrawPos.y + ( args.m_flHue / 359.f ) * 150.f + 1 }, MAX_ALPHA( SELECTED_ELEMENT ) );

	const auto alphaDrawPos{ Menu::m_pFocusItem.m_vecDrawPos + Vector2D( 10, 170 ) };

	if ( args.m_bAlpha ) {
		// checker pattern part two
		for ( int i = 0; i < 2; i++ ) {
			Vector2D squarePos{ alphaDrawPos + Vector2D( 0, 10 * i ) };
			constexpr auto squareSize{ Vector2D( 10, 10 ) };

			for ( int n = 0; n < 150 / 10; ++n ) {
				const auto squareColor{ ( i + n ) % 2 ? Color( 225, 225, 225 ) : Color( 255, 255, 255 ) };

				Render::RectFilled( squarePos + Vector2D( 10 * n, 0 ), squareSize, MAX_ALPHA( squareColor ) );
			}
		}

		Render::Gradient( alphaDrawPos, { 150, 20 }, Color( 0, 0, 0, 0 ), value.Alpha( 255 * Menu::m_pFocusItem.m_flFocusAnim ), false );

		Render::RectFilled( Vector2D{ alphaDrawPos.x + ( value.a / 255.f ) * 150.f, alphaDrawPos.y - 1 }, Vector2D{ 3, 22 }, MAX_ALPHA( OUTLINE_DARK ) );
		Render::Line( Vector2D{ alphaDrawPos.x + ( value.a / 255.f ) * 150.f + 2, alphaDrawPos.y }, Vector2D{ alphaDrawPos.x + ( value.a / 255.f ) * 150.f + 2, alphaDrawPos.y + 20 }, MAX_ALPHA( SELECTED_ELEMENT ) );

		if ( Input::Hovered( alphaDrawPos, Vector2D( 150.f, 20.f ) ) && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_bFrameAfterFocus && !( args.m_bSelectingHue || args.m_bSelectingSat ) )
			args.m_bSelectingAlpha = true;
	}

	const auto actualDrawPos{ Menu::m_pFocusItem.m_vecDrawPos + Vector2D{ 10, 10 } };

	Render::RectFilled( Vector2D{ hueDrawPos.x, alphaDrawPos.y }, Vector2D{ 20, 20 }, MAX_ALPHA( ( *reinterpret_cast< Color* >( m_pValue ) ) ) );

	// actual picker
	Render::Gradient( actualDrawPos, { 150, 150 }, Color( 1.f, 1.f, 1.f, Menu::m_pFocusItem.m_flFocusAnim ), MAX_ALPHA( hsv2rgb( args.m_flHue, 1.f, 1.f ) ), false );

	Render::Gradient( Menu::m_pFocusItem.m_vecDrawPos + 10, { 150, 150 }, Color( 0, 0, 0, 0 ), Color( 0.f, 0.f, 0.f, Menu::m_pFocusItem.m_flFocusAnim ), true );

	// controls
	if ( Input::Hovered( hueDrawPos, Vector2D( 20.f, 150.f ) ) && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_bFrameAfterFocus && !( args.m_bSelectingAlpha || args.m_bSelectingSat ) )
		args.m_bSelectingHue = true;

	if ( Input::Hovered( actualDrawPos, Vector2D( 150.f, 150.f ) ) && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_bFrameAfterFocus && !( args.m_bSelectingAlpha || args.m_bSelectingHue ) )
		args.m_bSelectingSat = true;

	if ( args.m_bSelectingHue ) {
		const auto cursorDelta{ Input::m_vecMousePos - Menu::m_pFocusItem.m_vecDrawPos - Vector2D( 11, 11 ) };
		args.m_flHue = ( std::clamp<float>( Input::m_vecMousePos.y - hueDrawPos.y, 0.f, 150.f ) / 150.f ) * 359.f;
	}

	if ( args.m_bSelectingAlpha ) {
		const auto cursorDelta{ Input::m_vecMousePos - Menu::m_pFocusItem.m_vecDrawPos - Vector2D( 11, 11 ) };
		value = value.Alpha( 255 * std::clamp<float>( cursorDelta.x / 150.f, 0.f, 1.f ) );
	}

	if ( args.m_bSelectingSat ) {
		const auto cursorDelta{ Input::m_vecMousePos - Menu::m_pFocusItem.m_vecDrawPos - Vector2D( 11, 11 ) };

		args.m_flSat = std::clamp<float>( cursorDelta.x / 150.f, 0.f, 1.f );
		args.m_flVal = std::clamp<float>( 1.f - cursorDelta.y / 150.f, 0.f, 1.f );
	}

	if ( Input::Released( VK_LBUTTON ) ) {
		args.m_bSelectingSat = false;
		args.m_bSelectingHue = false;
		args.m_bSelectingAlpha = false;
	}

	const auto alpha = value.a;
	value = hsv2rgb( args.m_flHue, args.m_flSat, args.m_flVal ).Alpha( alpha );

	const auto Hovered{ Input::Hovered( Menu::m_pFocusItem.m_vecDrawPos, size ) };
	if ( !Hovered && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_bFrameAfterFocus ) {
		Menu::m_pFocusItem.m_bFrameAfterFocus = true;
		Menu::m_pFocusItem.m_bFadeOut = true;
	}
}

#undef MAX_ALPHA

void CMenuItem::ColorPicker( ) {
	if ( !m_fnShow( ) )
		return;

	auto& value{ *reinterpret_cast< Color* >( m_pValue ) };
	auto& args{ *reinterpret_cast< ColorPickerArgs_t* >( m_pArgs ) };

	const auto pos{ Menu::m_vecDrawPos + Vector2D{ ( Menu::m_vecSize.x - BAR_SIZE - MARGIN * 3 ) / 2 - PADDING * 4 - ITEM_HEIGHT * 1.5f, -ITEM_HEIGHT - PADDING } };
	const auto size{ Vector2D{ ITEM_HEIGHT * 1.5f, ITEM_HEIGHT } };

	const auto Hovered{ Input::Hovered( pos, size ) };
	if ( Hovered && Input::Pressed( VK_LBUTTON ) && !Menu::m_pFocusItem.m_pItem && !Menu::m_pFocusItem.m_bFrameAfterFocus ) {
		const auto backup{ Menu::m_vecDrawPos };
		Menu::m_vecDrawPos = pos + Vector2D{ size.x + PADDING, 0 };
		Menu::m_pFocusItem.Update( this );
		Menu::m_vecDrawPos = backup;

		const auto tmp{ rgb2hsv( value ) };
		args.m_flHue = tmp.hue;
		args.m_flSat = tmp.sat;
		args.m_flVal = tmp.val;
	}

	Render::RectFilled( pos - Vector2D{ 1.f, 1.f }, size + Vector2D{ 2.f, 2.f },  OUTLINE_LIGHT );
	Render::RectFilled( pos, size,  value );
}