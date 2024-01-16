#include <thread>
#include "core/hooks.h"
#include "core/interfaces.h"
#include "utils/schema.h"
#include "menu/menu.h"
#include "sdk/valve/class_offsets.h"

void Entry( HMODULE hModule ) {
	//CursorArrow = LoadCursor( 0, IDC_ARROW );
	//CursorResize = LoadCursor( 0, IDC_SIZENWSE );
	_MM_SET_FLUSH_ZERO_MODE( _MM_FLUSH_ZERO_ON );
	_MM_SET_DENORMALS_ZERO_MODE( _MM_DENORMALS_ZERO_ON );

#ifdef _DEBUG
	AllocConsole( );
	FILE* file;
	freopen_s( &file, "CONIN$", "r", stdin );
	freopen_s( &file, "CONOUT$", "w", stdout );
	freopen_s( &file, "CONOUT$", "w", stderr );
#endif

	Menu::Register( );

	if ( !Interfaces::Init( ) ) 
		return;

	Schema::Setup( );

	Displacement::Init( );

	ClassOffsets::Initialise( );

	Hooks::Init( );

#ifdef _DEBUG
	printf( "\nDeployed havoc.\n" );
#endif

	while ( !GetAsyncKeyState( VK_F11 ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

	ctx.m_bUnloading = true;

	std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

	//Render::Unload( );

	MH_DisableHook( MH_ALL_HOOKS );
	MH_RemoveHook( MH_ALL_HOOKS );

	MH_Uninitialize( );

	reinterpret_cast< WNDPROC >( SetWindowLongPtr( ctx.hwnd, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( Hooks::WndProc ) ) );

	FreeConsole( );

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