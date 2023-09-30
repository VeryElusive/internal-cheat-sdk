#include "../core/hooks.h"
#include "../utils/render/render.h"
#include "../menu/menu.h"

HRESULT APIENTRY Hooks::hkPresent( IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags ) {
	const auto og{ Present.Original<decltype( &hkPresent )>( ) };

	if ( !Render::Init( pSwapChain ) )
		;// what else we gotta do ?

	Input::Update( );
	Menu::Render( );

	Render::RectFilled( { 300, 100 }, { 100,100 }, Color( 255,255,255 ) );
	Render::RoundedRectFilled( { 500, 100 }, { 100,100 }, 10, Color( 0, 255, 0 ) );
	Render::Gradient( { 700, 100 }, { 100,100 }, Color( 255,0,0 ), Color( 0, 0, 255 ), false );

	//Render::drawText( { 300, 400 }, L"Wreakin' havoc", Color( 255, 0, 0 ), FW1_SHADOW, 24, L"Tahoma" );
	Render::Render( );
		
	return og( pSwapChain, SyncInterval, Flags );
}
