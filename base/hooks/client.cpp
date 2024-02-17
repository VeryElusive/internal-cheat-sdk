#include "../havoc.h"
#include "../core/hooks.h"
#include "../utils/render/render.h"
#include "../menu/menu.h"
#include "../features/visuals/visuals.h"
#include "../features/rage/lag_record.h"

void __fastcall Hooks::hkFrameStageNotify( void* rcx, int frameStage ) {
	const auto og{ FrameStageNotify.Original<decltype( &hkFrameStageNotify )>( ) };


	switch ( frameStage ) {
	case FRAME_RENDER_END:
		break;
	default:
		break;
	}

	ctx.GetLocal( );
	if ( !ctx.m_pLocal )
		return og( rcx, frameStage );

	const auto localPawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };
	if ( !localPawn )
		return og( rcx, frameStage );

	const auto backup{ localPawn->m_vecViewOffset( ) };

	og( rcx, frameStage );

	localPawn->m_vecViewOffset( ) = backup;

	switch ( frameStage ) {
	case FRAME_NET_UPDATE_END:
		break;
	default:
		break;
	}
}