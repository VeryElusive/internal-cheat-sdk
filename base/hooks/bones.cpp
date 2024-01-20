#include "../havoc.h"
#include "../core/hooks.h"
#include "../core/config.h"
#include "../sdk/valve/entity.h"

/*bool __fastcall Hooks::hkModifyBonePosition( __int64 a1, int* a2, int* a3, void* a4 ) {
	const auto og{ ModifyBonePosition.Original<decltype( &hkModifyBonePosition )>( ) };

	ctx.GetLocal( );
	if ( !Configs::m_cConfig.m_bBugWalk )
		return og( a1, a2, a3, a4 );

	return 1;
}*/

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

	if ( flags != FLAG_ALL_BONE_FLAGS )
		return;

	static Vector lastAbsOrigin{ };
	if ( ctx.m_bAllowBoneUpdate ) {
		ctx.m_bAllowBoneUpdate = false;

		lastAbsOrigin = skeleton->m_vecAbsOrigin( );
		return og( rcx, flags );
	}
	else if ( lastAbsOrigin != skeleton->m_vecAbsOrigin( ) ) {
		const auto delta{ skeleton->m_vecAbsOrigin( ) - lastAbsOrigin };

		const auto modelState{ skeleton->m_modelState( ) };

		const auto model{ modelState.m_hModel };
		if ( !model.IsValid( ) )
			return;

		auto backupBones{ Interfaces::MemAlloc->Alloc( model->m_iBoneCount * sizeof( CBoneData ) ) };
		std::memcpy( backupBones, &modelState.m_pBones[ 0 ], model->m_iBoneCount * sizeof( CBoneData ) );

		og( rcx, flags );

		std::memcpy( &modelState.m_pBones[ 0 ], backupBones, model->m_iBoneCount * sizeof( CBoneData ) );

		for ( std::int32_t i = 0; i < model->m_iBoneCount; ++i )
			modelState.m_pBones[ i ].m_vecPosition += delta;

		Interfaces::MemAlloc->Free( backupBones );

		lastAbsOrigin = skeleton->m_vecAbsOrigin( );
	}
}