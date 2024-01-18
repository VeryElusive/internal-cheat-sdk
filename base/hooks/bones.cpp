#include "../havoc.h"
#include "../core/hooks.h"
#include "../core/config.h"
#include "../sdk/valve/entity.h"

void __fastcall Hooks::hkCalcWorldSpaceBones( void* rcx, int flags ) {
	const auto og{ CalcWorldSpaceBones.Original<decltype( &hkCalcWorldSpaceBones )>( ) };

	ctx.GetLocal( );
	if ( !ctx.m_pLocal )
		return og( rcx, flags );

	const C_CSPlayerPawn* local{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };
	if ( !local )
		return og( rcx, flags );

	const auto gsn{ local->m_pGameSceneNode( ) };
	if ( !gsn )
		return og( rcx, flags );

	const auto skeleton{ gsn->GetSkeletonInstance( ) };
	if ( !skeleton )
		return og( rcx, flags );

	if ( rcx != skeleton )
		return og( rcx, flags );

	if ( ctx.m_bAllowBoneUpdate 
		&& flags == FLAG_ALL_BONE_FLAGS ) {
		ctx.m_bAllowBoneUpdate = false;

		return og( rcx, flags );
	}

}