#include "../havoc.h"
#include "../core/hooks.h"

// this is a really gay way to do it, but anyway. it works.
// called from CViewRender::OnRenderStart
ViewMatrix_t* __fastcall Hooks::hkGetMatrixForView( void* pRenderGameSystem, IViewRender* pViewRender, ViewMatrix_t* pOutWorldToView, ViewMatrix_t* pOutViewToProjection, ViewMatrix_t* pOutWorldToProjection, ViewMatrix_t* pOutWorldToPixels ) {
	const auto og{ GetMatrixForView.Original<decltype( &hkGetMatrixForView )>( ) };

	const auto result{ og( pRenderGameSystem, pViewRender, pOutWorldToView, pOutViewToProjection, pOutWorldToProjection, pOutWorldToPixels ) };

	ctx.m_matViewMatrix = *pOutWorldToProjection;

	ctx.m_vecViewAngles = pViewRender->m_vecAngles;

	return result;
}