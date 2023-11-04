#pragma once
#include "../dependencies/minhook/minhook.h"

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

	inline WNDPROC WndProc;
	inline HWND hwnd;
	LRESULT CALLBACK hkWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Init( );
}