#include "render.h"

void Render::Render( ) {
	// MAYBE NEED pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);

    immediateContext->OMSetRenderTargets( 1, &RenderTargetView, NULL );

    immediateContext->VSSetShader( vertexShader, nullptr, 0 );
    immediateContext->PSSetShader( pixelShader, nullptr, 0 );

    immediateContext->OMSetBlendState( blendState, nullptr, 0xffffffff );

    immediateContext->VSSetConstantBuffers( 0, 1, &screenProjectionBuffer );

    immediateContext->IASetInputLayout( inputLayout );

    UINT stride = sizeof( Vertex );
    UINT offset = 0;
    immediateContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );

    fontWrapper->DrawString( immediateContext, L"", 0.0f, 0.0f, 0.0f, 0xff000000, FW1_RESTORESTATE | FW1_NOFLUSH );

    // begin rendering

    if ( std::size( renderList->vertices ) > 0 )
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        immediateContext->Map( vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
        {
            std::memcpy( mappedResource.pData, renderList->vertices.data( ), sizeof( Vertex ) * std::size( renderList->vertices ) );
        }
        immediateContext->Unmap( vertexBuffer, 0 );
    }

    std::size_t pos = 0;

    for ( const auto& batch : renderList->batches )
    {
        immediateContext->IASetPrimitiveTopology( batch.topology );
        immediateContext->Draw( static_cast< UINT >( batch.count ), static_cast< UINT >( pos ) );

        pos += batch.count;
    }

    fontWrapper->Flush( immediateContext );
    fontWrapper->DrawGeometry( immediateContext, renderList->textGeometry, nullptr, nullptr, FW1_RESTORESTATE );

    // end
    renderList->clear( );
}

void Render::FilledRect( Vector2D pos, Vector2D size, Color col ) {
    Vertex v[ ]
    {
        { pos.x,			pos.y,				0.f, col },
        { pos.x + size.x,	pos.y,				0.f, col },
        { pos.x,			pos.y + size.y,	0.f, col },

        { pos.x + size.x,	pos.y,				0.f, col },
        { pos.x + size.x,	pos.y + size.y,	0.f, col },
        { pos.x,			pos.y + size.y,	0.f, col }
    };

    AddVertices( v, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void Render::AddVertex( Vertex& vertex, D3D11_PRIMITIVE_TOPOLOGY topology ) {
#ifdef DEBUG
    assert( topology != D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
        && "addVertex >Use addVertices to draw line/triangle strips!" );
    assert( topology != D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ
        && "addVertex >Use addVertices to draw line/triangle strips!" );
    assert( topology != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
        && "addVertex >Use addVertices to draw line/triangle strips!" );
    assert( topology != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ
        && "addVertex >Use addVertices to draw line/triangle strips!" );
#endif // DEBUG

    // TODO: error logging?
    if ( std::size( renderList->vertices ) >= maxVertices )
        return;

    if ( std::empty( renderList->batches ) || renderList->batches.back( ).topology != topology )
        renderList->batches.emplace_back( 0, topology );

    renderList->batches.back( ).count += 1;
    renderList->vertices.push_back( vertex );
}

template <std::size_t N>
void Render::AddVertices( Vertex( &vertexArr )[ N ], D3D11_PRIMITIVE_TOPOLOGY topology ) {
    // TODO: error log ?
    if ( std::size( renderList->vertices ) + N >= maxVertices )
        return;

    if ( std::empty( renderList->batches ) || renderList->batches.back( ).topology != topology )
        renderList->batches.emplace_back( 0, topology );

    renderList->batches.back( ).count += N;

    renderList->vertices.resize( std::size( renderList->vertices ) + N );
    std::memcpy( &renderList->vertices[ std::size( renderList->vertices ) - N ], &vertexArr[ 0 ], N * sizeof( Vertex ) );

    switch ( topology )
    {
    case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP:
    case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ:
    case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
    case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ:
    {
        Vertex seperator{};
        AddVertex( seperator, D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED );
        break;
    }
    default:
        break;
    }
}