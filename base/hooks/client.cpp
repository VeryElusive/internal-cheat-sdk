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
		ctx.GetLocal( );
		break;
	default:
		break;
	}

	og( rcx, frameStage );

	switch ( frameStage ) {
	case FRAME_NET_UPDATE_END:
		Features::LagCompensation.Main( true );
		break;
	default:
		break;
	}
}