#pragma once
#include "../../havoc.h"
#include "FW1FontWrapper/FW1FontWrapper.h"

#define MAINVP 0
#define SAFE_RELEASE(p) if (p) { p->Release(); p = nullptr; } 

struct Vertex;
struct Batch;


struct HandleData {
	DWORD pid;
	HWND hWnd;
};

class RenderList
{
public:
	using Ptr = std::unique_ptr<RenderList>;

	friend class Renderer;
public:
	RenderList( ) = delete;

	RenderList( IFW1Factory* fontFactory, std::size_t maxVertices = 0 )
	{
		vertices.reserve( maxVertices );
		fontFactory->CreateTextGeometry( &textGeometry );
	}

	~RenderList( )
	{
		SAFE_RELEASE( textGeometry );
	}

	void clear( )
	{
		vertices.clear( );
		batches.clear( );
		textGeometry->Clear( );
	}

	std::vector<Vertex>	vertices;
	std::vector<Batch>	batches;

	IFW1TextGeometry* textGeometry;
};

// TODO: rename everything
namespace Render {
	inline ID3D11DeviceContext* immediateContext;
	inline ID3D11Device* direct3DDevice;

	inline ID3D11InputLayout* inputLayout;
	inline ID3D11BlendState* blendState;
	inline ID3D11VertexShader* vertexShader;
	inline ID3D11PixelShader* pixelShader;
	inline ID3D11Buffer* vertexBuffer;
	inline ID3D11Buffer* screenProjectionBuffer;

	inline ID3D11Texture2D* RenderTargetTexture;
	inline ID3D11RenderTargetView* RenderTargetView = NULL;

	inline IFW1Factory* fontFactory;
	inline IFW1FontWrapper* fontWrapper;

	inline DirectX::XMMATRIX				projection;

	inline RenderList::Ptr			renderList;

	inline std::size_t				maxVertices{ 16384 * 8 * 4 * 3 };
	inline void** MethodsTable = NULL;

	inline D3D11_VIEWPORT pViewports[ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ]{ 0 };
	inline DirectX::XMMATRIX mOrtho;
	
	bool Init( IDXGISwapChain* pSwapchain );
	void Render( );
	bool PrepareMethodTable( );

	// wrappers
	void FilledRect( Vector2D pos, Vector2D size, Color col );

	// adding points
	template <std::size_t N>
	void AddVertices( Vertex( &vertexArr )[ N ], D3D11_PRIMITIVE_TOPOLOGY topology );
	void AddVertex( Vertex& vertex, D3D11_PRIMITIVE_TOPOLOGY topology );
}

struct Vertex {
	Vertex( ) = default;
	Vertex( float x, float y, float z, Color col )
		: pos( x, y, z ) {
		color.f[ 0 ] = static_cast< float >( col.r ) / 255.0f;
		color.f[ 1 ] = static_cast< float >( col.g ) / 255.0f;
		color.f[ 2 ] = static_cast< float >( col.b ) / 255.0f;
		color.f[ 3 ] = static_cast< float >( col.a ) / 255.0f;
	}

	Vector pos;
	DirectX::XMVECTORF32 color;
};

struct Batch
{
	Batch( std::size_t count, D3D11_PRIMITIVE_TOPOLOGY topology )
		: count( count ), topology( topology )
	{}

	std::size_t count;
	D3D11_PRIMITIVE_TOPOLOGY topology;
};

const char shader[ ] = R"(cbuffer screenProjectionBuffer : register(b0)
{
	matrix projection;
};
 
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};
 
struct VS_INPUT
{
	float4 pos : POSITION;
	float4 col : COLOR;
};
 
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
 
	output.pos = mul(projection, float4(input.pos.xy, 0.f, 1.f));
	output.col = input.col;
 
	return output;
}
 
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	return input.col;
})";