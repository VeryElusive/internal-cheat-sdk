#include "../havoc.h"
#include "../core/hooks.h"
#include "../core/config.h"

void __fastcall Hooks::hkCalcAnimationState( void* rcx, int edx ) {
	const auto og{ CalcAnimationState.Original<decltype( &hkCalcAnimationState )>( ) };

	if ( ctx.m_bAllowBoneUpdate )
		og( rcx, edx );
}