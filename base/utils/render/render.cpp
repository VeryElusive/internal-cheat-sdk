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

    if ( std::size( renderList->vertices ) > 0 ) {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        immediateContext->Map( vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
        {
            std::memcpy( mappedResource.pData, renderList->vertices.data( ), sizeof( Vertex ) * std::size( renderList->vertices ) );
        }
        immediateContext->Unmap( vertexBuffer, 0 );
    }

    std::size_t pos = 0;
    fontWrapper->Flush( immediateContext );

    for ( const auto& batch : renderList->batches ) {
        immediateContext->IASetPrimitiveTopology( batch.topology );
        immediateContext->Draw( static_cast< UINT >( batch.count ), static_cast< UINT >( pos ) );

        pos += batch.count;
    }

    fontWrapper->DrawGeometry( immediateContext, renderList->textGeometry, nullptr, nullptr, FW1_RESTORESTATE );

    // end
    // TODO: move to top of visuals.
    renderList->clear( );
}

void Render::Line( Vector2D pos, Vector2D pos2, Color col ) {
    Vertex v[ ]
    {
        { pos.x, pos.y, 0.0f, col },
        { pos2.x, pos2.y, 0.0f, col }
    };

    AddVertices( v, D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
}

// we can do stroke thickness if we just draw rect filled instead of lines
void Render::Rect( Vector2D pos, Vector2D size, Color col ) {
    Line( pos, { pos.x + size.x, pos.y }, col );
    Line( { pos.x + size.x, pos.y }, { pos.x + size.x, pos.y + size.y }, col );
    Line( { pos.x + size.x, pos.y + size.y }, { pos.x, pos.y + size.y }, col );
    Line( { pos.x, pos.y + size.y }, pos, col );
}

void Render::CircleFilled( Vector2D pos, float radius, const Color& color ) {
    const int segments = 36; // Increase the number of segments for a smoother circle
    Vertex v[ segments + 2 ]; // Add 2 vertices for the center point and last point

    // Calculate the center point
    v[ 0 ] = Vertex{ pos.x, pos.y, 0.f, color };

    for ( int i = 1; i <= segments + 1; i++ ) {
        float theta = 2.f * PI * static_cast< float >( i - 1 ) / static_cast< float >( segments );

        v[ i ] = Vertex{
            pos.x + radius * std::cos( theta ),
            pos.y + radius * std::sin( theta ),
            0.f, color
        };
    }

    // Create triangles to fill the circle
    std::vector<Vertex> triangles;
    for ( int i = 1; i <= segments; i++ ) {
        triangles.push_back( v[ 0 ] );
        triangles.push_back( v[ i ] );
        triangles.push_back( v[ i + 1 ] );
    }

    // Pass the triangles to your rendering function
    AddVertices( triangles.data( ), triangles.size( ), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void Render::Circle( Vector2D pos, float radius, const Color& color ) {
    const int segments = 24;

    Vertex v[ segments + 1 ];

    for ( int i = 0; i <= segments; i++ )
    {
        float theta = 2.f * PI * static_cast< float >( i ) / static_cast< float >( segments );

        v[ i ] = Vertex{
            pos.x + radius * std::cos( theta ),
            pos.y + radius * std::sin( theta ),
            0.f, color
        };
    }

    AddVertices( v, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP );
}

Vector2D Render::GetTextSize( const std::string& text, float fontSize, const std::string& fontFamily ) {
    const auto wText{ Utils::StringToWideString( text ) };
    const auto wFontFamily{ Utils::StringToWideString( fontFamily ) };

    return GetTextSize( wText, fontSize, wFontFamily );
}

Vector2D Render::GetTextSize( const std::wstring& text, float fontSize, const std::wstring& fontFamily ) {
    FW1_RECTF nullRect = { 0.f, 0.f, 0.f, 0.f };
    FW1_RECTF rect = fontWrapper->MeasureString( text.c_str( ), fontFamily.c_str( ),
        fontSize, &nullRect, FW1_NOWORDWRAP );
    return{ rect.Right, rect.Bottom };
}

// TODO: make this threadsafe.
// TODO: make this progressive, can be done at same time as threadsafe by making its own queue system.
void Render::Text( const Vector2D& pos, const std::string& text, const Color& color, std::uint32_t flags, float fontSize, const std::string& fontFamily ) {
    const auto wText{ Utils::StringToWideString( text ) };
    const auto wFontFamily{ Utils::StringToWideString( fontFamily ) };

    Text( pos, wText, color, flags, fontSize, wFontFamily );
}

void Render::Text( const Vector2D& pos, const std::wstring& text, const Color& color, std::uint32_t flags, float fontSize, const std::wstring& fontFamily ) {
    if ( flags & FW1_SHADOW ) {
        std::uint32_t shadowColor = Color( 25, 25, 25, 216 * ( color.a / 255 ) ).ToUInt32( );
        FW1_RECTF shadowRect = { pos.x + 1.0f, pos.y + 1.0f, pos.x + 1.0f, pos.y + 1.0f };

        fontWrapper->AnalyzeString( nullptr, text.c_str( ), fontFamily.c_str( ), fontSize, &shadowRect, shadowColor, flags | FW1_NOFLUSH | FW1_NOWORDWRAP, renderList->textGeometry );
    }

    std::uint32_t transformedColor = color.ToUInt32( );
    FW1_RECTF rect = { pos.x, pos.y, pos.x, pos.y };

    fontWrapper->AnalyzeString( nullptr, text.c_str( ),
        fontFamily.c_str( ), fontSize, &rect, transformedColor, flags | FW1_NOFLUSH | FW1_NOWORDWRAP, renderList->textGeometry );

    renderList->batches.emplace_back( 0xFADED, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP );
}

// TODO: at some point make this properly.
void Render::RoundedRectFilled( Vector2D pos, Vector2D size, int radius, Color col ) {
    // Ensure cornerRadius is within a valid range
    radius = std::min<int>( radius, std::min( size.x, size.y ) * 0.5f );

    // Draw the central rectangle
    RectFilled( pos + Vector2D( radius, 0 ), size - Vector2D( radius * 2, 0 ), col );
    RectFilled( pos + Vector2D( 0, radius ), size - Vector2D( 0, radius * 2 ), col );

    // Draw the top-left corner
    CircleFilled( pos + Vector2D( radius, radius ), radius, col );

    // Draw the top-right corner
    CircleFilled( pos + Vector2D( size.x - radius, radius ), radius, col );

    // Draw the bottom-left corner
    CircleFilled( pos + Vector2D( radius, size.y - radius ), radius, col );

    // Draw the bottom-right corner
    CircleFilled( pos + Vector2D( size.x - radius, size.y - radius ), radius, col );
}

void Render::Gradient( Vector2D pos, Vector2D size, Color col, Color col2, bool horizontal ) {
    Vertex v[ ]
    {
        { pos.x,			pos.y,				0.f, col },
        { pos.x + size.x,	pos.y,				0.f, horizontal ? col2 : col },
        { pos.x,			pos.y + size.y,	    0.f, horizontal ? col : col2 },

        { pos.x + size.x,	pos.y,				0.f, horizontal ? col2 : col },
        { pos.x + size.x,	pos.y + size.y,	    0.f, col2 },
        { pos.x,			pos.y + size.y,	    0.f, horizontal ? col : col2 }
    };

    AddVertices( v, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void Render::RectFilled( Vector2D pos, Vector2D size, Color col ) {
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

void Render::AddVertices( Vertex vertexArr[ ], int size, D3D11_PRIMITIVE_TOPOLOGY topology ) {
    // TODO: error log ?
    if ( std::size( renderList->vertices ) + size >= maxVertices )
        return;

    if ( std::empty( renderList->batches ) || renderList->batches.back( ).topology != topology )
        renderList->batches.emplace_back( 0, topology );

    renderList->batches.back( ).count += size;

    renderList->vertices.resize( std::size( renderList->vertices ) + size );
    std::memcpy( &renderList->vertices[ std::size( renderList->vertices ) - size ], &vertexArr[ 0 ], size * sizeof( Vertex ) );

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