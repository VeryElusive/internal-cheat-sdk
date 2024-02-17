#include "../havoc.h"
#include "../core/hooks.h"
#include "../core/config.h"
#include "../sdk/valve/entity.h"

void Hooks::hkPredictionUpdate( void* rcx, int a2, int a3 ) {
	const auto og{ PredictionUpdate.Original<decltype( &hkPredictionUpdate )>( ) };

	const auto localPawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };
	if ( !localPawn )
		return;

	og( rcx, a2, a3 );
}