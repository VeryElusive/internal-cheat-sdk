#include "hooks.h"
#include "../utils/render/render.h"
#include <d3d9.h>
#include <thread>

void Hooks::Init( ) {
	if ( MH_Initialize( ) != MH_OK )
		return;

	if ( !Render::PrepareMethodTable( ) )
		return;

	INIT_HOOK( Present, Render::MethodsTable[ 8 ] );
	//INIT_HOOK( ResizeBuffers, Render::MethodsTable[ 13 ] );
}