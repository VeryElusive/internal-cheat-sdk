#include "../core/hooks.h"
#include "../utils/render/render.h"

HRESULT APIENTRY Hooks::hkPresent( IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags ) {
	const auto og{ Present.Original<decltype( &hkPresent )>( ) };

	if ( !Render::Init( pSwapChain ) )
		;// what else we gotta do ?

	Render::FilledRect( { 100, 100 }, { 100,100 }, Color( 255,255,255 ) );
	Render::Render( );
		
	return og( pSwapChain, SyncInterval, Flags );
}
