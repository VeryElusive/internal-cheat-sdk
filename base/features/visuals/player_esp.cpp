#include "visuals.h"
#include "../../havoc.h"

void CVisuals::HandlePlayer( const CCSPlayerController* player ) {
	const C_CSPlayerPawn* localPawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };
	const C_CSPlayerPawn* playerPawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( player->m_hPawn( ) ) };


}