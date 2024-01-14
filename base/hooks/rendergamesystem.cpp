#include "../havoc.h"
#include "../core/hooks.h"
#include "../features/visuals/visuals.h"
#include "../sdk/fnv1a.h"

// this is a really gay way to do it, but anyway. it works.
// called from CViewRender::OnRenderStart
ViewMatrix_t* __fastcall Hooks::hkGetMatrixForView( void* pRenderGameSystem, IViewRender* pViewRender, ViewMatrix_t* pOutWorldToView, ViewMatrix_t* pOutViewToProjection, ViewMatrix_t* pOutWorldToProjection, ViewMatrix_t* pOutWorldToPixels ) {
	const auto og{ GetMatrixForView.Original<decltype( &hkGetMatrixForView )>( ) };

	const auto result{ og( pRenderGameSystem, pViewRender, pOutWorldToView, pOutViewToProjection, pOutWorldToProjection, pOutWorldToPixels ) };

	ctx.m_matViewMatrix = *pOutWorldToProjection;

	ctx.m_vecViewAngles = pViewRender->m_vecAngles;

	for ( int i{ 1 }; i < Interfaces::Engine->GetMaxClients( ); ++i ) {
		C_BaseEntity* ent{ Interfaces::GameResourceService->m_pGameEntitySystem->Get( i ) };
		if ( !ent )
			continue;

		SchemaClassInfoData_t* classInfo{ };
		ent->GetSchemaClassInfo( &classInfo );
		if ( !classInfo )
			continue;

		const auto uHashedName{ FNV1A::Hash( classInfo->szNname ) };

		if ( uHashedName == FNV1A::HashConst( "CCSPlayerController" ) ) {
			auto playerController{ static_cast< CCSPlayerController* >( ent ) };

			C_CSPlayerPawn* pawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( playerController->m_hPawn( ) ) };
			if ( !pawn )
				continue;

			PlayerEntry_t& entry{ ctx.PlayerEntries[ i ] };

			if ( entry.m_pPawn != pawn
				|| !playerController->m_bPawnIsAlive( ) )
				entry.Reset( pawn );

			if ( !Features::Visuals.GetBBox( entry.m_pPawn, entry.Visuals.m_cBBox ) )
				entry.Visuals.m_cBBox = { 0,0,0,0 };
		}
	}

	return result;
}