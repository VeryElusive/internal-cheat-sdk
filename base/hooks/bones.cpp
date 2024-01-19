#include "../havoc.h"
#include "../core/hooks.h"
#include "../core/config.h"
#include "../sdk/valve/entity.h"

void __fastcall Hooks::hkCalcWorldSpaceBones( void* rcx, int flags ) {
	const auto og{ CalcWorldSpaceBones.Original<decltype( &hkCalcWorldSpaceBones )>( ) };

	auto skeleton{ ( ( CSkeletonInstance* ) rcx ) };

	ctx.GetLocal( );
	if ( !ctx.m_pLocal )
		return og( rcx, flags );

	const C_CSPlayerPawn* local{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };
	if ( !local )
		return og( rcx, flags );

	if ( skeleton->m_pPawn( ) != local )
		return og( rcx, flags );

	//PRINT_PTR( rcx );

	static Vector lastAbsOrigin{ };

	static bool once{ };

	if ( ctx.m_bAllowBoneUpdate ) {
		if ( flags == FLAG_ALL_BONE_FLAGS ) {
			ctx.m_bAllowBoneUpdate = false;

			lastAbsOrigin = skeleton->m_vecAbsOrigin( );

			return og( rcx, flags );
		}
	}
	/*else if ( lastAbsOrigin != skeleton->m_vecAbsOrigin( ) ) {
		const auto delta{ skeleton->m_vecAbsOrigin( ) - lastAbsOrigin };
		CModelState modelState{ skeleton->m_modelState( ) };

		lastAbsOrigin = local->GetAbsOrigin( );

		const CStrongHandle<CModel> model = modelState.modelHandle;

		if ( !model.IsValid( ) ) 
			return;

		for ( std::int32_t i = 0; i < model->BoneCount; ++i ) {
			const std::int32_t bone_parent_index = model->GetBoneParent( i );

			// Skip bones with no parent.
			if ( bone_parent_index == -1 ) 
				continue;

 			modelState.bones[ i ].position += delta;
		}
	}*/
}  