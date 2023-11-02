#include "render.h"

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
	WindowClass.lpszClassName = "MJ";
	WindowClass.hIconSm = NULL;
	RegisterClassEx( &WindowClass );
	WindowHwnd = CreateWindow( WindowClass.lpszClassName, "DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL );
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

void Render::Unload( ) {
	SAFE_RELEASE( vertexShader );
	SAFE_RELEASE( pixelShader );
	SAFE_RELEASE( vertexBuffer );
	SAFE_RELEASE( screenProjectionBuffer );
	SAFE_RELEASE( inputLayout );
	SAFE_RELEASE( blendState );
	SAFE_RELEASE( fontWrapper );
}

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

bool Render::Init( IDXGISwapChain* pSwapchain ) {
	ID3D11Device* dev{ };
	HRESULT hr = pSwapchain->GetDevice( __uuidof( ID3D11Device ), ( void** ) &dev );
	if ( FAILED( hr ) || dev == direct3DDevice )
		return false;

	direct3DDevice = dev;

	D3D11_INPUT_ELEMENT_DESC layout[ ] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 16 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;

	direct3DDevice->GetImmediateContext( &immediateContext );

	FW1CreateFactory( FW1_VERSION, &fontFactory );

	renderList = std::make_unique<RenderList>( fontFactory, maxVertices );

	fontFactory->CreateFontWrapper( direct3DDevice, L"Tahoma", &fontWrapper );
	fontFactory->Release( );

	D3DCompile( shader, sizeof( shader ), nullptr, nullptr, nullptr, "VS", "vs_4_0", 0, 0, &vsBlob, nullptr );
	D3DCompile( shader, sizeof( shader ), nullptr, nullptr, nullptr, "PS", "ps_4_0", 0, 0, &psBlob, nullptr );

	direct3DDevice->CreateVertexShader( vsBlob->GetBufferPointer( ), vsBlob->GetBufferSize( ), nullptr, &vertexShader );
	direct3DDevice->CreatePixelShader( psBlob->GetBufferPointer( ), psBlob->GetBufferSize( ), nullptr, &pixelShader );

	direct3DDevice->CreateInputLayout( layout, static_cast< UINT >( sizeof( layout ) ), vsBlob->GetBufferPointer( ), vsBlob->GetBufferSize( ), &inputLayout );

	SAFE_RELEASE( vsBlob );
	SAFE_RELEASE( psBlob );

	D3D11_BLEND_DESC blendDesc{};

	blendDesc.RenderTarget->BlendEnable = TRUE;
	blendDesc.RenderTarget->SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget->DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget->SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget->DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget->BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget->BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	direct3DDevice->CreateBlendState( &blendDesc, &blendState );

	D3D11_BUFFER_DESC bufferDesc{};

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof( Vertex ) * static_cast< UINT >( maxVertices );
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	direct3DDevice->CreateBuffer( &bufferDesc, nullptr, &vertexBuffer );

	bufferDesc = {};

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof( DirectX::XMMATRIX );
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	direct3DDevice->CreateBuffer( &bufferDesc, nullptr, &screenProjectionBuffer );

	D3D11_VIEWPORT viewport{};
	UINT numViewports = 1;

	immediateContext->RSGetViewports( &numViewports, &viewport );

	projection = DirectX::XMMatrixOrthographicOffCenterLH( 0.0f, viewport.Width, viewport.Height, 0.0f, -100.0f, 100.0f );

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	immediateContext->Map( screenProjectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	{
		std::memcpy( mappedResource.pData, &projection, sizeof( DirectX::XMMATRIX ) );
	}
	immediateContext->Unmap( screenProjectionBuffer, 0 );

	if ( SUCCEEDED( pSwapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* ) &RenderTargetTexture ) ) )
	{
		//warning: this will crash on res change, nothing seems to fix it (no crash when switching window/fullscreen)
		direct3DDevice->CreateRenderTargetView( RenderTargetTexture, NULL, &RenderTargetView );
		RenderTargetTexture->Release( );
	}

	return true;
}
