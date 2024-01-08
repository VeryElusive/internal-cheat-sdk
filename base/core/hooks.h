#pragma once
#include "../dependencies/minhook/minhook.h"
#include "../sdk/matrix.h"
#include "../sdk/valve/interfaces/iviewrender.h"

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
	NEW_HOOK( LevelInit, __int64*, __fastcall ) void* ecx, const char* szNewMap );
	NEW_HOOK( LevelShutdown, __int64, __fastcall ) void* ecx );
	NEW_HOOK( GetMatrixForView, ViewMatrix_t*, __fastcall ) void* pRenderGameSystem, IViewRender* pViewRender, ViewMatrix_t* pOutWorldToView, ViewMatrix_t* pOutViewToProjection, ViewMatrix_t* pOutWorldToProjection, ViewMatrix_t* pOutWorldToPixels );
	NEW_HOOK( MouseInputEnabled, bool, __fastcall ) void* ecx );

	inline WNDPROC WndProc;
	inline HWND hwnd;
	LRESULT CALLBACK hkWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Init( );
}