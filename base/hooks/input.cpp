#include "../havoc.h"
#include "../core/hooks.h"
#include "../menu/menu.h"
#include "../sdk/valve/entity.h"
#include "../sdk/valve/interfaces/ienginecvar.h"
#include "../features/misc/movement.h"
#include "../features/rage/antiaim.h"
#include "../features/rage/lag_record.h"
#include "../features/rage/ragebot.h"
#include "../core/config.h"

#include <intrin.h>

bool __fastcall Hooks::hkMouseInputEnabled( void* rcx ) {
	const auto og{ MouseInputEnabled.Original<decltype( &hkMouseInputEnabled )>( ) };

	return Menu::m_bOpened ? false : og( rcx );
}

bool __fastcall Hooks::hkCreateMove( void* rcx, unsigned int edx, std::int64_t a3 ) {
	const auto og{ CreateMove.Original<decltype( &hkCreateMove )>( ) };

	auto cmd{ Interfaces::Input->GetUserCmd( ) };

	//printf( "0x%p\n", _AddressOfReturnAddress( ) );

	ctx.a3 = a3;

	ctx.GetLocal( );
	if ( !ctx.m_pLocal 
		|| !ctx.m_pLocal->m_bPawnIsAlive( ) )
		return og( rcx, edx, a3 );

	if ( !cmd->cmd.pBase )
		return og( rcx, edx, a3 );

	const auto localPawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };
	if ( !localPawn )
		return og( rcx, edx, a3 );

	const auto result{ og( rcx, edx, a3 ) };

	/*if ( !forSubTickFrame )*/ {
		Features::LagCompensation.Main( );

		Features::RageBot.Main( localPawn, cmd );

		if ( Features::RageBot.m_cData.m_bValid ) {
			auto res = Displacement::GetButtonState( Interfaces::Input, edx );

			*res |= IN_ATTACK;

			//og( rcx, edx, a3, true );

		}

		Features::Movement.Main( localPawn, cmd );

		const auto backupViewAngles{ cmd->cmd.pBase->pViewangles->angValue.y };

		Features::RageBot.PostCMove( localPawn, cmd );

		Features::Movement.MoveMINTFix( localPawn, cmd, backupViewAngles );

		Features::AntiAim.Main( localPawn, cmd );
	}

	ctx.m_flForwardmove = cmd->cmd.pBase->flForwardMove;
	ctx.m_flSidemove = cmd->cmd.pBase->flSideMove;

	cmd->cmd.pBase->pViewangles->angValue.NormalizeAngle( );
	cmd->cmd.pBase->pViewangles->angValue.ClampAngle( );

	ctx.m_mapPlayerEntries[ localPawn->GetRefEHandle( ).m_nIndex ].Animations.m_bShouldUpdateBones = true;

	cmd->cmd.pBase->flForwardMove = std::clamp( cmd->cmd.pBase->flForwardMove, -1.f, 1.f );
	cmd->cmd.pBase->flSideMove = std::clamp( cmd->cmd.pBase->flSideMove, -1.f, 1.f );

	return result;
}

void __fastcall Hooks::hkUnknown02( void* rcx, int edx ) {
	const auto og{ Unknown02.Original<decltype( &hkUnknown02 )>( ) };

	Vector d;
	Interfaces::Client->GetViewAngles( &d );

	og( rcx, edx );

	Interfaces::Input->SetViewAngles( d );
}

void* __fastcall Hooks::hkLagcompensation( void* subTickData, void* inputHistoryFieldCurrent, char a3, __int64 a4, __int64 a5, __int64 a6 ) {
	const auto og{ Lagcompensation.Original<decltype( &hkLagcompensation )>( ) };

	auto std{ ( ( CSubTickData* ) subTickData ) };
	auto ihe{ ( ( CCSGOInputHistoryEntryPB* ) inputHistoryFieldCurrent ) };

	ctx.m_flRenderTickFraction = std->m_flRenderTickFraction;
	ctx.m_iRenderTick = std->m_iRenderTick;

	return og( subTickData, inputHistoryFieldCurrent, a3, a4, a5, a6 );
}