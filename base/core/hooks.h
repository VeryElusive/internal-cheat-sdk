#pragma once
#include "../dependencies/minhook/minhook.h"
#include "../sdk/matrix.h"
#include "../sdk/valve/interfaces/iviewrender.h"
#include "../sdk/valve/events.h"

#include <d3d11.h>

class CHooked {
public:
	bool Hook( void* ptrTo, void* newPTR ) {
		auto status{ MH_CreateHook( ptrTo, newPTR, &m_pOriginal ) };

		// log unsuccessful hooking attemps mb?
		if ( status != MH_OK )
			return false;

		status = MH_EnableHook( ptrTo );

		return status == MH_OK;
	}

	template <typename Fn>
	Fn Original( ) {
		return static_cast< Fn >( m_pOriginal );
	}

	void* m_pOriginal{ };
};

#define NEW_HOOK(name, returnVal, callingConvention) inline CHooked name{ }; returnVal callingConvention hk##name(
#define INIT_HOOK(name, address) name.Hook( address, &hk##name );

namespace Hooks {
	NEW_HOOK( Present, HRESULT, APIENTRY ) IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags );
	NEW_HOOK( LevelInit, __int64*, __fastcall ) void* rcx, const char* szNewMap );
	NEW_HOOK( LevelShutdown, __int64, __fastcall ) void* rcx );
	NEW_HOOK( GetMatrixForView, ViewMatrix_t*, __fastcall ) void* pRenderGameSystem, IViewRender* pViewRender, ViewMatrix_t* pOutWorldToView, ViewMatrix_t* pOutViewToProjection, ViewMatrix_t* pOutWorldToProjection, ViewMatrix_t* pOutWorldToPixels );
	NEW_HOOK( MouseInputEnabled, bool, __fastcall ) void* rcx );
	NEW_HOOK( FrameStageNotify, void, __fastcall ) void* rcx, int nFrameStage );
	NEW_HOOK( CreateMove, bool, __fastcall ) void* rcx, unsigned int sequenceNumber, std::int64_t a3 );
	NEW_HOOK( OverrideView, void, __fastcall ) void* rcx, CViewSetup* setup );
	NEW_HOOK( HandleEvents, void, __fastcall ) void* rcx, Events_t* const event );

	NEW_HOOK( CalcWorldSpaceBones, void, __fastcall ) void* rcx, int flags );
	//NEW_HOOK( ModifyBonePosition, bool, __fastcall ) __int64 a1, int* a2, int* a3, void* a4 );
	NEW_HOOK( CalcAnimationState, void, __fastcall ) void* rcx, int edx );

	NEW_HOOK( Lagcompensation, void*, __fastcall ) void* subTickData, void* inputHistoryFieldCurrent, char a3, __int64 a4, __int64 a5, __int64 a6 );

	NEW_HOOK( PredictionUpdate, void, __fastcall ) void* rcx, int a2, int a3 );

	// part of CClientInput
	//NEW_HOOK( Unknown01, void, __fastcall ) void* rcx, int edx, char a3, unsigned char a4 );
	NEW_HOOK( Unknown02, void, __fastcall ) void* rcx, int edx );
	NEW_HOOK( CallsCreatemove, void*, __fastcall ) void* rcx );
	NEW_HOOK( ReadFrameInput, void*, __fastcall ) void* rcx, int edx );
	//NEW_HOOK( PostNetworkDataReceived, void*, __fastcall ) void* rcx );
	//NEW_HOOK( SendMove, void*, __fastcall ) void* rcx, void* rdx );

	//NEW_HOOK( SetTargetTime, void*, __fastcall ) int* output, int* input );
	//NEW_HOOK( ReadFrameInput, bool, __fastcall ) void* rcx, int edx );

	inline WNDPROC WndProc;
	LRESULT CALLBACK hkWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Init( );
}