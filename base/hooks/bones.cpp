#include "../havoc.h"
#include "../core/hooks.h"
#include "../core/config.h"
#include "../sdk/valve/entity.h"

/*bool __fastcall Hooks::hkModifyBonePosition( __int64 a1, int* a2, int* a3, void* a4 ) {
	const auto og{ ModifyBonePosition.Original<decltype( &hkModifyBonePosition )>( ) };

	ctx.GetLocal( );
	if ( !Configs::m_cConfig.m_bSlowWalk )
		return og( a1, a2, a3, a4 );

	return 1;
}*/

void __fastcall Hooks::hkCalcWorldSpaceBones( void* rcx, int flags ) {
	const auto og{ CalcWorldSpaceBones.Original<decltype( &hkCalcWorldSpaceBones )>( ) };

	auto skeleton{ ( ( CSkeletonInstance* ) rcx ) };

	if ( !ctx.GetLocal( ) )
		return og( rcx, flags );

	const auto pawn{ skeleton->m_pPawn( ) };
	if ( !pawn )
		return og( rcx, flags );

	auto it{ ctx.m_mapPlayerEntries.find( pawn->GetRefEHandle( ).m_nIndex ) };
	if ( it == ctx.m_mapPlayerEntries.end( ) )
		return og( rcx, flags );

	auto& entry = it->second;

	if ( flags != FLAG_ALL_BONE_FLAGS )
		return;

	if ( entry.Animations.m_bShouldUpdateBones ) {
		entry.Animations.m_vecLastBoneOrigin = skeleton->m_vecAbsOrigin( );

		og( rcx, flags );
		entry.Animations.m_bShouldUpdateBones = false;
		return;
	}
	else if ( entry.Animations.m_vecLastBoneOrigin != skeleton->m_vecAbsOrigin( ) ) {
		const auto delta{ skeleton->m_vecAbsOrigin( ) - entry.Animations.m_vecLastBoneOrigin };

		const auto modelState{ skeleton->m_modelState( ) };

		const auto model{ modelState.m_hModel };
		if ( !model.IsValid( ) )
			return;

		auto backupBones{ Interfaces::MemAlloc->Alloc( 128 * sizeof( CBoneData ) ) };
		std::memcpy( backupBones, &modelState.m_pBones[ 0 ], 128 * sizeof( CBoneData ) );

		og( rcx, flags );

		std::memcpy( &modelState.m_pBones[ 0 ], backupBones, 128 * sizeof( CBoneData ) );

		for ( std::int32_t i = 0; i < 128; ++i )
			modelState.m_pBones[ i ].m_vecPosition += delta;

		Interfaces::MemAlloc->Free( backupBones );

		entry.Animations.m_vecLastBoneOrigin = skeleton->m_vecAbsOrigin( );
	}
}