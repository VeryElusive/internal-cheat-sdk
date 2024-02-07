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

bool __fastcall Hooks::hkMouseInputEnabled( void* rcx ) {
	const auto og{ MouseInputEnabled.Original<decltype( &hkMouseInputEnabled )>( ) };

	return Menu::m_bOpened ? false : og( rcx );
}

__int64* sub_18074DE00( unsigned int edx ) {
	auto v2 = *( DWORD* ) ( ((uintptr_t)Interfaces::Input) + 24 * edx + 0x8FB8 );
	if ( v2 )
		return ( __int64* ) ( ( ( uintptr_t ) Interfaces::Input ) + 24 * edx + 0x8FC0 ) + 264 * ( v2 - 1 );
	else
		return ( __int64* ) ( ( ( uintptr_t ) Interfaces::Input ) + 0x5F78 * edx + 0x8E38 );
}

bool __fastcall Hooks::hkCreateMove( void* rcx, unsigned int edx, std::int64_t a3, bool forSubTickFrame ) {
	const auto og{ CreateMove.Original<decltype( &hkCreateMove )>( ) };

	auto cmd{ Interfaces::Input->GetUserCmd( ) };

	static auto test{ Memory::FindPattern( CLIENT_DLL, _( "40 55 53 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 8B DA 48 8B F9 E8 ? ? ? ? 48 85 C0 0F 84" ) ) };

	PRINT_PTR( test );

	if ( ctx.m_pLocal && ctx.m_pLocal->m_bPawnIsAlive( ) ) {
		/*if ( Interfaces::Input->m_nUnknownSlot == Interfaces::Input->m_nUnknownSlot2 ) {
			if ( Interfaces::Input->m_nUnknownSlot < 3 )
				Interfaces::Input->m_nUnknownSlot += 1;
			else
				Interfaces::Input->m_nUnknownSlot -= 1;
		}*/

		auto res = sub_18074DE00( edx );

		*res |= IN_ATTACK;
	}

	const auto result{ og( rcx, edx, a3, forSubTickFrame ) };

	ctx.a3 = a3;

	ctx.GetLocal( );
	if ( !ctx.m_pLocal 
		|| !ctx.m_pLocal->m_bPawnIsAlive( ) )
		return result;

	if ( !cmd->cmd.pBase )
		return result;

	const auto localPawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };
	if ( !localPawn )
		return result;

	if ( !forSubTickFrame ) {
		Features::LagCompensation.Main( );

		Features::Movement.Main( localPawn, cmd );

		const auto backupViewAngles{ cmd->cmd.pBase->pViewangles->angValue.y };

		Features::RageBot.Main( localPawn, cmd );

		Features::Movement.MoveMINTFix( localPawn, cmd, backupViewAngles );
	}

	Features::AntiAim.Main( localPawn, cmd, !forSubTickFrame );

	ctx.m_flForwardmove = cmd->cmd.pBase->flForwardMove;
	ctx.m_flSidemove = cmd->cmd.pBase->flSideMove;

	cmd->cmd.pBase->pViewangles->angValue.NormalizeAngle( );
	cmd->cmd.pBase->pViewangles->angValue.ClampAngle( );

	if ( !forSubTickFrame )
		ctx.m_mapPlayerEntries[ localPawn->GetRefEHandle( ).m_nIndex ].Animations.m_bShouldUpdateBones = true;

	cmd->cmd.pBase->flForwardMove = std::clamp( cmd->cmd.pBase->flForwardMove, -1.f, 1.f );
	cmd->cmd.pBase->flSideMove = std::clamp( cmd->cmd.pBase->flSideMove, -1.f, 1.f );

	if ( !forSubTickFrame ) {
		if ( cmd->cmd.nAttackStartHistoryIndex >= 0 ) {
		}
	}

	return result;
}

void __fastcall Hooks::hkUnknown01( void* rcx, int edx, char a3, unsigned char a4 ) {
	const auto og{ Unknown01.Original<decltype( &hkUnknown01 )>( ) };

	Vector d;
	Interfaces::Client->GetViewAngles( &d );

	og( rcx, edx, a3, a4 );

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