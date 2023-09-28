#include <thread>
#include "core/hooks.h"
#include "utils/render.h"
#include "menu/menu.h"

void Entry( HMODULE hModule ) {
	ctx.hwnd = FindWindow( "SDL_app", NULL );

	//CursorArrow = LoadCursor( 0, IDC_ARROW );
	//CursorResize = LoadCursor( 0, IDC_SIZENWSE );
	Menu::Register( );

	Hooks::Init( );

	while ( !GetAsyncKeyState( VK_F11 ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

	MH_DisableHook( MH_ALL_HOOKS );
	MH_RemoveHook( MH_ALL_HOOKS );

	MH_Uninitialize( );

	reinterpret_cast< WNDPROC >( SetWindowLongPtr( ctx.hwnd, GWLP_WNDPROC, LONG_PTR( Hooks::OldWndProc ) ) );

	FreeLibraryAndExitThread( hModule, EXIT_SUCCESS );
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD dwReason, LPVOID lpReserved ) {
	if ( dwReason == DLL_PROCESS_ATTACH ) {
		// no need for DLL_THREAD_ATTACH and DLL_THREAD_DETACH calls
		DisableThreadLibraryCalls( hModule );

		// once the cheat is uninjected we want to close the windows handle for our cheat process.
		if ( const auto thread{ CreateThread( nullptr, 0U, LPTHREAD_START_ROUTINE( Entry ), hModule, 0UL, nullptr ) }; thread )
			CloseHandle( thread );

		return TRUE;
	}

	return FALSE;
}