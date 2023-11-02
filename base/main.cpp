#include <thread>
#include "core/hooks.h"
#include "menu/menu.h"

void Entry( HMODULE hModule ) {
	//CursorArrow = LoadCursor( 0, IDC_ARROW );
	//CursorResize = LoadCursor( 0, IDC_SIZENWSE );
	_MM_SET_FLUSH_ZERO_MODE( _MM_FLUSH_ZERO_ON );
	_MM_SET_DENORMALS_ZERO_MODE( _MM_DENORMALS_ZERO_ON );

	Menu::Register( );

	Hooks::Init( );

	while ( !GetAsyncKeyState( VK_F11 ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

	Render::Unload( );

	MH_DisableHook( MH_ALL_HOOKS );
	MH_RemoveHook( MH_ALL_HOOKS );

	MH_Uninitialize( );

	FreeLibraryAndExitThread( hModule, EXIT_SUCCESS );
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD dwReason, LPVOID lpReserved ) {
	if ( dwReason == DLL_PROCESS_ATTACH ) {
		// no need for DLL_THREAD_ATTACH and DLL_THREAD_DETACH calls
		DisableThreadLibraryCalls( hModule );

		if ( const auto thread{ CreateThread( nullptr, 0U, LPTHREAD_START_ROUTINE( Entry ), hModule, 0UL, nullptr ) }; thread )
			CloseHandle( thread );

		return TRUE;
	}

	return FALSE;
}