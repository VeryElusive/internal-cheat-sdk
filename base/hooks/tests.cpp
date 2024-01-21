#include "../havoc.h"
#include "../core/hooks.h"
#include "../core/config.h"

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