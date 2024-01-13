#include "../havoc.h"
#include "../core/hooks.h"
#include "../menu/menu.h"
#include "../sdk/valve/entity.h"
#include "../features/misc/movement.h"

bool __fastcall Hooks::hkMouseInputEnabled( void* rcx ) {
	const auto og{ MouseInputEnabled.Original<decltype( &hkMouseInputEnabled )>( ) };

	return Menu::m_bOpened ? false : og( rcx );
}

bool __fastcall Hooks::hkCreateMove( void* rcx, unsigned int sequenceNumber, std::int64_t a3, bool active ) {
	const auto og{ CreateMove.Original<decltype( &hkCreateMove )>( ) };

	auto cmd{ Interfaces::Input->GetUserCmd( ) };

	const auto result{ og( rcx, sequenceNumber, a3, active ) };

	ctx.GetLocal( );
	if ( !ctx.m_pLocal || !ctx.m_pLocal->m_bPawnIsAlive( ) )
		return result;

	const auto localPawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };
	if ( !localPawn )
		return result;


	Features::Movement.Main( localPawn, cmd );

	return result;
}