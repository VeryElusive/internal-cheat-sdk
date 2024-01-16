#include "../havoc.h"
#include "../core/hooks.h"
#include "../utils/render/render.h"
#include "../menu/menu.h"
#include "../features/visuals/visuals.h"

void __fastcall Hooks::hkFrameStageNotify( void* rcx, int frameStage ) {
	const auto og{ FrameStageNotify.Original<decltype( &hkFrameStageNotify )>( ) };

	//Interfaces::Input->test( );

	switch ( frameStage ) {
	case FRAME_RENDER_END:
		ctx.GetLocal( );

		// the fuck..?
		//Features::Visuals.Main( );
		break;
	default:
		break;
	}

	return og( rcx, frameStage );
}