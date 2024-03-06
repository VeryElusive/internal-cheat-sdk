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

	const auto local{ ctx.GetLocal( ) };
	if ( !local
		|| !local->m_bPawnIsAlive( ) )
		return og( rcx, edx, a3 );

	if ( !cmd->pBase )
		return og( rcx, edx, a3 );

	const auto localPawn{ ctx.GetPlayerPawn( local ) };
	if ( !localPawn )
		return og( rcx, edx, a3 );

	const auto weaponServices{ localPawn->m_pWeaponServices( ) };
	if ( !weaponServices )
		return og( rcx, edx, a3 );;

	const auto weapon{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSWeaponBase>( weaponServices->m_hActiveWeapon( ) ) };
	if ( !weapon )
		return og( rcx, edx, a3 );;

	ctx.DEBUGSpread = weapon->GetSpread( );
	ctx.DEBUGInac = weapon->GetInaccuracy( );

	// RAPIDFIRE:
	// TODO: investigate this!!!!

	if ( Configs::m_cConfig.m_bRapidFire ) {
		static int ticks{ };

		ticks++;
		if ( ticks < 64 ) {
			//Interfaces::Input->AddButton( IN_ATTACK );

			//cmd->m_cButtonStates.m_iHeld |= IN_ATTACK;
			//Features::RageBot.Main( localPawn, cmd );
			return false;
		}

		ticks = 0;
	}

	//printf( "%i | %i\n", Interfaces::Input->m_nSequenceNumber, Interfaces::Input->m_iCommandPassCount );

	if ( Interfaces::Input->m_iCommandPassCount == 0 )
		Features::RageBot.Main( localPawn, cmd );

	const auto result{ og( rcx, edx, a3 ) };

	//ctx.m_flRenderTickFraction = Interfaces::Input->m_pSubTickData->m_flRenderTickFraction;
	//ctx.m_iRenderTick = Interfaces::Input->m_pSubTickData->m_iRenderTick;
	//if ( Interfaces::Input->m_iCommandPassCount == 1 )
		//Features::LagCompensation.Main( );

	Features::Movement.Main( localPawn, cmd );

	const auto backupViewAngles{ cmd->pBase->pViewangles->angValue.y };

	Features::RageBot.PostCMove( localPawn, cmd );

	Features::Movement.MoveMINTFix( localPawn, cmd, backupViewAngles );

	Features::AntiAim.Main( localPawn, cmd );

	//Features::Movement.ClampMovement( cmd );

	ctx.m_flForwardmove = cmd->pBase->flForwardMove;
	ctx.m_flSidemove = cmd->pBase->flSideMove;

	cmd->pBase->pViewangles->angValue.NormalizeAngle( );
	cmd->pBase->pViewangles->angValue.ClampAngle( );

	ctx.m_mapPlayerEntries[ localPawn->GetRefEHandle( ).m_nIndex ].Animations.m_bShouldUpdateBones = true;

	return result;
}

// called from EndCommand
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
	ctx.m_iPlayerTick = std->m_iPlayerTick;
	ctx.m_flPlayerTickFraction = std->m_flPlayerTickFraction;

	return og( subTickData, inputHistoryFieldCurrent, a3, a4, a5, a6 );
}

void* __fastcall Hooks::hkCallsCreatemove( void* rcx ) {
	const auto og{ CallsCreatemove.Original<decltype( &hkCallsCreatemove )>( ) };

	const auto result{ og( rcx ) };

	return result;
}