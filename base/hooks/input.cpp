#include "../havoc.h"
#include "../core/hooks.h"
#include "../menu/menu.h"
#include "../sdk/valve/entity.h"
#include "../features/misc/movement.h"
#include "../features/rage/antiaim.h"
#include "../core/config.h"

bool __fastcall Hooks::hkMouseInputEnabled( void* rcx ) {
	const auto og{ MouseInputEnabled.Original<decltype( &hkMouseInputEnabled )>( ) };

	return Menu::m_bOpened ? false : og( rcx );
}

bool __fastcall Hooks::hkCreateMove( void* rcx, unsigned int edx, std::int64_t a3, bool morePasses ) {
	const auto og{ CreateMove.Original<decltype( &hkCreateMove )>( ) };

	auto cmd{ Interfaces::Input->GetUserCmd( ) };

	const auto result{ og( rcx, edx, a3, morePasses ) };

	ctx.GetLocal( );
	if ( !ctx.m_pLocal 
		|| !ctx.m_pLocal->m_bPawnIsAlive( )
		|| morePasses )
		return result;

	const auto localPawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };
	if ( !localPawn )
		return result;

	Features::Movement.Main( localPawn, cmd );

	Features::AntiAim.Main( localPawn, cmd );

	ctx.m_flForwardmove = cmd->cmd.pBase->flForwardMove;
	ctx.m_flSidemove = cmd->cmd.pBase->flSideMove;

	cmd->cmd.pBase->subtickMovesField.nCurrentSize = 0;
	cmd->cmd.inputHistoryField.nCurrentSize = 0;

	ctx.m_bAllowBoneUpdate = true;

	ctx.m_pLastCmd = cmd;

	cmd->cmd.pBase->pViewangles->angValue.NormalizeAngle( ).ClampAngle( );

	cmd->cmd.pBase->flForwardMove = std::clamp( cmd->cmd.pBase->flForwardMove, -1.f, 1.f );
	cmd->cmd.pBase->flSideMove = std::clamp( cmd->cmd.pBase->flSideMove, -1.f, 1.f );

	return result;
}

void __fastcall Hooks::hkUnknown01( void* rcx, int edx, char a3, unsigned char a4 ) {
	const auto og{ Unknown01.Original<decltype( &hkUnknown01 )>( ) };

	Vector d;
	Interfaces::Client->GetViewAngles( &d );

	og( rcx, edx, a3, a4 );

	Interfaces::Input->SetViewAngles( d );
}