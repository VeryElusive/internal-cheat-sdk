#include "../core/hooks.h"
#include "../core/input.h"
#include "../core/displacement.h"
#include "../utils/render/render.h"
#include "../menu/menu.h"
#include "../features/visuals/visuals.h"

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_internal.h"
#include "../dependencies/imgui/backends/imgui_impl_dx11.h"
#include "../dependencies/imgui/backends/imgui_impl_win32.h"

LRESULT CALLBACK Hooks::hkWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	if ( uMsg == WM_KEYDOWN && wParam == VK_INSERT ) {
		Menu::m_bOpened = !Menu::m_bOpened;
		if ( Interfaces::InputSystem->IsRelativeMouseMode( ) )
			Displacement::fnSetRelativeMouseMode( !Menu::m_bOpened );
	}

	if ( Menu::m_bOpened ) {
		if ( uMsg == WM_MOUSEWHEEL || uMsg == WM_MOUSEHWHEEL )
			Input::m_flScroll += ( float ) GET_WHEEL_DELTA_WPARAM( wParam ) / ( float ) WHEEL_DELTA;

		switch ( uMsg ) {
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
			return 0;

		default:
			break;
		}
	}

	// return input controls to the game
	return CallWindowProc( WndProc, hWnd, uMsg, wParam, lParam );
}

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

			ctx.hwnd = Desc.OutputWindow;
			WndProc = reinterpret_cast< WNDPROC >( SetWindowLongPtr( ctx.hwnd, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( Hooks::hkWndProc ) ) );

			ID3D11Texture2D* BackBuffer;
			pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* ) &BackBuffer );
			if ( BackBuffer ) {
				device->CreateRenderTargetView( BackBuffer, NULL, &renderTargetView );
				BackBuffer->Release( );

				ImGui_ImplWin32_Init( ctx.hwnd );
				ImGui_ImplDX11_Init( device, context );

				Render::Init( );

				ImGui_ImplDX11_CreateDeviceObjects( );
				ImGui::GetIO( ).ImeWindowHandle = ctx.hwnd;

				ImGuiContext& g = *GImGui;
				g.Style.AntiAliasedLines = false;
				g.Style.AntiAliasedFill = false;

				init = true;
			}
		}
	}

	if ( ctx.m_bUnloading )
		return og( pSwapChain, SyncInterval, Flags );

	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );

	// if i decide to move this to an in game hook, this must go before we draw.
	ImGui::NewFrame( );


	ImGui::GetIO( ).MouseDrawCursor = Menu::m_bOpened;

	ctx.GetLocal( );

	if ( Interfaces::GlobalVars ) {
		//if ( Displacement::ViewMatrix )
		//	ctx.m_matViewMatrix = *Displacement::ViewMatrix;

		Input::Update( );
		Input::CheckBinds( );

		Features::Visuals.Main( );

		Menu::Render( );
	}

	//Render::RectFilled( { 300, 300 }, { 100, 800 }, Color( 42, 230, 180 ) );

	ImGui::EndFrame( );
	ImGui::Render( );

	if ( context )
		context->OMSetRenderTargets( 1, &renderTargetView, NULL );

	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );
		
	return og( pSwapChain, SyncInterval, Flags );
}