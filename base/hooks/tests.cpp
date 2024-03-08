#include "../havoc.h"
#include "../core/hooks.h"
#include "../core/config.h"
#include "../sdk/valve/usercmd.h"

/*void* __fastcall Hooks::hkPostNetworkDataReceived( void* rcx ) {
	const auto og{ PostNetworkDataReceived.Original<decltype( &hkPostNetworkDataReceived )>( ) };

	const auto ack = RECAST_AT_OFFSET( int, rcx, 72 );
	printf( "%i\n", ack );

	return og( rcx );
}*/

void __fastcall Hooks::hkCalcAnimationState( void* rcx, int flags ) {
	const auto og{ CalcAnimationState.Original<decltype( &hkCalcAnimationState )>( ) };
	auto skeleton{ ( ( CSkeletonInstance* ) rcx ) };

	const auto pawn{ skeleton->m_pPawn( ) };
	if ( !pawn )
		return og( rcx, flags );

	auto it{ ctx.m_mapPlayerEntries.find( pawn->GetRefEHandle( ).m_nIndex ) };
	if ( it == ctx.m_mapPlayerEntries.end( ) )
		return og( rcx, flags );

	auto& entry = it->second;

	if ( entry.Animations.m_bShouldUpdateBones )
		og( rcx, flags );
}

/*void* __fastcall Hooks::hkSetTargetTime( int* output, int* input ) {
	const auto og{ SetTargetTime.Original<decltype( &hkSetTargetTime )>( ) };

	const auto ret{ og( output, input ) };

	//printf( "input: %i | output: %i | tickcount: %i | dbg tickcount: %i\n", *input, *output, Interfaces::GlobalVars->m_nTickCount, ctx.DEBUGBacktrackCMDTickcount );

	return ret;
}

bool __fastcall Hooks::hkReadFrameInput( void* rcx, int edx ) {
	const auto og{ ReadFrameInput.Original<decltype( &hkReadFrameInput )>( ) };
	auto test= og( rcx, edx );

	if ( Interfaces::Input->m_iAttackSlot >= 0 ) {
		auto cmd{ Interfaces::Input->GetUserCmd( ) };

		const auto subTickAttack{ cmd->cmd.inputHistoryField.pRep->tElements[ Interfaces::Input->m_iAttackSlot ] };
		PRINT_PTR( subTickAttack->cl_interp );
		PRINT_PTR( subTickAttack );
	}

	return test;
}*/