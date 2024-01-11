#include "../havoc.h"
#include "../core/hooks.h"
#include "../menu/menu.h"

bool __fastcall Hooks::hkMouseInputEnabled( void* rcx ) {
	const auto og{ MouseInputEnabled.Original<decltype( &hkMouseInputEnabled )>( ) };

	return Menu::m_bOpened ? false : og( rcx );
}