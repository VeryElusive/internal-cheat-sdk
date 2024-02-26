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

	const auto localPawn{ ctx.GetLocalPawn( ) };
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