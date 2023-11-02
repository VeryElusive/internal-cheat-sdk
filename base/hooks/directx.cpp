#include "../core/hooks.h"
#include "../utils/render/render.h"
#include "../menu/menu.h"

#include "../pch.h"


HRESULT APIENTRY Hooks::hkPresent( IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags ) {
	const auto og{ Present.Original<decltype( &hkPresent )>( ) };

	static bool init{ };
	static ID3D11DeviceContext* context{ };
	static ID3D11RenderTargetView* renderTargetView;
	if ( !init ) {
		ID3D11Device* device{ };
		if ( SUCCEEDED( pSwapChain->GetDevice( __uuidof( ID3D11Device ), ( void** ) &device ) ) ) {
			ImGui::CreateContext( );

			ImGuiIO& io = ImGui::GetIO( ); ( void ) io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			device->GetImmediateContext( &context );

			DXGI_SWAP_CHAIN_DESC Desc;
			pSwapChain->GetDesc( &Desc );

			HWND hwnd;
			hwnd = Desc.OutputWindow;

			ID3D11Texture2D* BackBuffer;
			pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* ) &BackBuffer );
			device->CreateRenderTargetView( BackBuffer, NULL, &renderTargetView );
			BackBuffer->Release( );

			ImGui_ImplWin32_Init( hwnd );
			ImGui_ImplDX11_Init( device, context );
			ImGui_ImplDX11_CreateDeviceObjects( );
			ImGui::GetIO( ).ImeWindowHandle = hwnd;
			//Process::WndProc = ( WNDPROC ) SetWindowLongPtr( Process::Hwnd, GWLP_WNDPROC, ( __int3264 ) ( LONG_PTR ) WndProc );
			init = true;
		}
	}

	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );
	//ImGui::GetIO( ).MouseDrawCursor = var::showMenu;

	Render::RectFilled( { 300, 300 }, { 100, 100 }, Color( 42, 230, 180 ) );

	ImGui::EndFrame( );
	ImGui::Render( );

	if ( context )
		context->OMSetRenderTargets( 1, &renderTargetView, NULL );

	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );
		
	return og( pSwapChain, SyncInterval, Flags );
}