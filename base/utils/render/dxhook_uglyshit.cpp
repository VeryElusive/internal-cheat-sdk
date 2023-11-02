#include "render.h"

struct HandleData {
	DWORD pid;
	HWND hWnd;
};

BOOL CALLBACK EnumWindowsCallback( HWND hWnd, LPARAM lParam ) {
	HandleData& data = *( HandleData* ) lParam;
	DWORD pid = 0;
	GetWindowThreadProcessId( hWnd, &pid );
	if ( pid == data.pid && GetWindow( hWnd, GW_OWNER ) == HWND( 0 ) && IsWindowVisible( hWnd ) )
	{
		data.hWnd = hWnd;
		return FALSE;
	}

	return TRUE;
}

HWND FindMainWindow( DWORD dwPID ) {
	HandleData handleData{ 0 };
	handleData.pid = dwPID;
	EnumWindows( EnumWindowsCallback, ( LPARAM ) &handleData );
	return handleData.hWnd;
}

WNDCLASSEX WindowClass;
HWND WindowHwnd;

bool InitWindow( ) {
	WindowClass.cbSize = sizeof( WNDCLASSEX );
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = DefWindowProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandle( NULL );
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = NULL;
	WindowClass.hbrBackground = NULL;
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = "b";
	WindowClass.hIconSm = NULL;
	RegisterClassEx( &WindowClass );
	WindowHwnd = CreateWindow( WindowClass.lpszClassName, "a", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL );
	if ( WindowHwnd == NULL ) {
		return false;
	}
	return true;
}

bool DeleteWindow( ) {
	DestroyWindow( WindowHwnd );
	UnregisterClass( WindowClass.lpszClassName, WindowClass.hInstance );
	if ( WindowHwnd != NULL ) {
		return false;
	}
	return true;
}

// DETECTION VECTOR: CreateWindow
bool Render::PrepareMethodTable( ) {
	if ( !InitWindow( ) )
		return false;

	HMODULE D3D11Module = GetModuleHandle( "d3d11.dll" );
	if ( D3D11Module == NULL ) {
		DeleteWindow( );
		return false;
	}

	void* D3D11CreateDeviceAndSwapChain = GetProcAddress( D3D11Module, "D3D11CreateDeviceAndSwapChain" );
	if ( D3D11CreateDeviceAndSwapChain == NULL ) {
		DeleteWindow( );
		return false;
	}

	D3D_FEATURE_LEVEL FeatureLevel;
	const D3D_FEATURE_LEVEL FeatureLevels[ ] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

	DXGI_RATIONAL RefreshRate;
	RefreshRate.Numerator = 60;
	RefreshRate.Denominator = 1;

	DXGI_MODE_DESC BufferDesc;
	BufferDesc.Width = 100;
	BufferDesc.Height = 100;
	BufferDesc.RefreshRate = RefreshRate;
	BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SAMPLE_DESC SampleDesc;
	SampleDesc.Count = 1;
	SampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc = BufferDesc;
	SwapChainDesc.SampleDesc = SampleDesc;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = WindowHwnd;
	SwapChainDesc.Windowed = 1;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;

	if ( ( ( long( __stdcall* )(
		IDXGIAdapter*,
		D3D_DRIVER_TYPE,
		HMODULE,
		UINT,
		const D3D_FEATURE_LEVEL*,
		UINT,
		UINT,
		const DXGI_SWAP_CHAIN_DESC*,
		IDXGISwapChain**,
		ID3D11Device**,
		D3D_FEATURE_LEVEL*,
		ID3D11DeviceContext** ) )( D3D11CreateDeviceAndSwapChain ) )( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, FeatureLevels, 1, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, &FeatureLevel, &Context ) < 0 )
	{
		DeleteWindow( );
		return false;
	}

	MethodsTable = ( void** )::calloc( 205, sizeof( void* ) );
	memcpy( MethodsTable, *( void*** ) SwapChain, 18 * sizeof( void* ) );
	memcpy( MethodsTable + 18, *( void*** ) Device, 43 * sizeof( void* ) );
	memcpy( MethodsTable + 18 + 43, *( void*** ) Context, 144 * sizeof( void* ) );

	SAFE_RELEASE( SwapChain );
	SAFE_RELEASE( Device );
	SAFE_RELEASE( Context );
	DeleteWindow( );
	return true;
}