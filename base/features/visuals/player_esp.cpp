#include "visuals.h"
#include "../../havoc.h"
#include "../../utils/render/render.h"
#include "../../utils/math.h"
#include "../../core/config.h"

bool CVisuals::GetBBox( const C_CSPlayerPawn* pawn, BBox_t& bbox ) {
	const CCollisionProperty* collision{ pawn->m_pCollision( ) };
	if ( !collision )
		return false;

	// pasted i dont care about making this func myself
	const auto min = collision->m_vecMins( );
	const auto max = collision->m_vecMaxs( );

	Vector dir = ctx.m_vecViewAngles;
	Vector vF, vR, vU;
	dir.x = 0;
	dir.z = 0;

	Math::AngleVectors( dir, &vF, &vR, &vU );

	const auto zh = vU * max.z + vF * max.y + vR * min.x; // = Front left front
	const auto e = vU * max.z + vF * max.y + vR * max.x; //  = Front right front
	const auto d = vU * max.z + vF * min.y + vR * min.x; //  = Front left back
	const auto c = vU * max.z + vF * min.y + vR * max.x; //  = Front right back

	const auto g = vU * min.z + vF * max.y + vR * min.x; //  = Bottom left front
	const auto f = vU * min.z + vF * max.y + vR * max.x; //  = Bottom right front
	const auto a = vU * min.z + vF * min.y + vR * min.x; //  = Bottom left back
	const auto b = vU * min.z + vF * min.y + vR * max.x; //  = Bottom right back*-

	Vector pointList[ ] = {
		a,
		b,
		c,
		d,
		e,
		f,
		g,
		zh,
	};

	Vector2D transformed[ ARRAYSIZE( pointList ) ];

	for ( int i = 0; i < ARRAYSIZE( pointList ); i++ )
	{
		auto origin = pawn->GetAbsOrigin( );

		pointList[ i ] += origin;

		if ( !Render::WorldToScreen( pointList[ i ], transformed[ i ] ) )
			return false;
	}

	float left = FLT_MAX;
	float top = -FLT_MAX;
	float right = -FLT_MAX;
	float bottom = FLT_MAX;

	for ( int i = 0; i < ARRAYSIZE( pointList ); i++ ) {
		if ( left > transformed[ i ].x )
			left = transformed[ i ].x;
		if ( top < transformed[ i ].y )
			top = transformed[ i ].y;
		if ( right < transformed[ i ].x )
			right = transformed[ i ].x;
		if ( bottom > transformed[ i ].y )
			bottom = transformed[ i ].y;
	}

	bbox.x = left;
	bbox.y = bottom;
	bbox.w = right - left;
	bbox.h = top - bottom + 5;

	return true;
}

void CVisuals::HandlePlayer( PlayerEntry_t& entry ) {
	const C_CSPlayerPawn* local{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };

	uint8_t type{ ESP_TYPE::ENEMY };

	// TODO: proper team check
	if ( local == entry.m_pPawn )
		type = ESP_TYPE::LOCAL;
	else if ( local->m_iTeamNum( ) == entry.m_pPawn->m_iTeamNum( ) )
		type = ESP_TYPE::TEAM;

	if ( !Configs::m_cConfig.m_bEnabled[ type ] )
		return;

	// reset stored health
	if ( entry.Visuals.m_iHealth < std::min( entry.m_pPawn->m_iHealth( ), 100 ) )
		entry.Visuals.m_iHealth = std::min( entry.m_pPawn->m_iHealth( ), 100 );

	// animation
	if ( entry.Visuals.m_iHealth > entry.m_pPawn->m_iHealth( ) )
		entry.Visuals.m_iHealth -= 6.f * ctx.m_flFrameTime * ( entry.Visuals.m_iHealth - entry.m_pPawn->m_iHealth( ) );

	if ( entry.m_pPawn->Dormant( ) ) {
		entry.Visuals.m_flAlpha -= ctx.m_flFrameTime / 5.f;
		entry.Visuals.m_flDormancyFade += ctx.m_flFrameTime;
	}
	else {
		entry.Visuals.m_flAlpha += ctx.m_flFrameTime * 2.f;
		entry.Visuals.m_flDormancyFade -= ctx.m_flFrameTime * 2.f;
	}

	entry.Visuals.m_flAlpha = std::clamp<float>( entry.Visuals.m_flAlpha, 0.f, 1.f );
	entry.Visuals.m_flDormancyFade = std::clamp<float>( entry.Visuals.m_flDormancyFade, 0.f, 1.f );

	BBox_t bbox;
	if ( !GetBBox( entry.m_pPawn, bbox ) )
		return;

	if ( Configs::m_cConfig.m_bBox[ type ] )
		DrawBox( entry, type, bbox );

	if ( Configs::m_cConfig.m_bName[ type ] )
		DrawName( entry, type, bbox );

	if ( Configs::m_cConfig.m_bHealth[ type ] )
		DrawHealth( entry, type, bbox );
}

void CVisuals::DrawBox( const PlayerEntry_t& entry, uint8_t type, const BBox_t& bbox ) {
	Color col{ Configs::m_cConfig.m_colBox[ type ] };
	col.a *= entry.Visuals.m_flAlpha;

	if ( entry.Visuals.m_flDormancyFade )
		col = col.Lerp( DormantCol.Alpha( col.a * 0.4f ), entry.Visuals.m_flDormancyFade );

	const Color outline{ Color( 0, 0, 0, static_cast< int >( col.a ) ) };
	
	Render::Rect( bbox.Position( ) + 1, bbox.Size( ) - 2, outline );
	Render::Rect( bbox.Position( ), bbox.Size( ), col );
	Render::Rect( bbox.Position( ) - 1, bbox.Size( ) + 2, outline );
}

void CVisuals::DrawName( const PlayerEntry_t& entry, uint8_t type, const BBox_t& bbox ) {
	Color col{ Configs::m_cConfig.m_colName[ type ] };
	col.a *= entry.Visuals.m_flAlpha;

	if ( entry.Visuals.m_flDormancyFade )
		col = col.Lerp( DormantCol.Alpha( col.a * 0.4f ), entry.Visuals.m_flDormancyFade );

	Render::Text( { bbox.x + bbox.w / 2, bbox.y - 13 }, entry.m_pName, col, TEXT_CENTER | TEXT_DROPSHADOW, 13, Render::Fonts.NameESP );
}

void CVisuals::DrawHealth( const PlayerEntry_t& entry, uint8_t type, const BBox_t& bbox ) {
	Color col{ Configs::m_cConfig.m_colHealth[ type ] };

	if ( !Configs::m_cConfig.m_bHealthOverride[ type ] ) {
		const int green{ static_cast< int >( static_cast< float >( std::min( entry.Visuals.m_iHealth, 100 ) ) * 2.55f ) };
		const int red{ 255 - green };
		col = Color( red, green, 0 );
	}

	col.a *= entry.Visuals.m_flAlpha;

	if ( entry.Visuals.m_flDormancyFade )
		col = col.Lerp( DormantCol.Alpha( col.a * 0.4f ), entry.Visuals.m_flDormancyFade );

	const Color outline{ Color( 0, 0, 0, static_cast< int >( col.a ) ) };

	const int bar_height{ static_cast< int >( static_cast< float >( entry.Visuals.m_iHealth ) * static_cast< float >( bbox.h ) / 100.0f ) };
	const float offset{ bbox.h - bar_height };

	Render::RectFilled( { bbox.x - 6, bbox.y - 1 }, { 4, bbox.h + 2 }, outline.Alpha( outline.a * 0.5f ) );
	Render::RectFilled( { bbox.x - 5, bbox.y + offset }, { 2, bar_height }, col );

	Render::Rect( { bbox.x - 6, bbox.y - 1 }, { 4, bbox.h + 2 }, outline );

	if ( entry.Visuals.m_iHealth <= 92 )
		Render::Text( { bbox.x - 5, bbox.y + offset - 3 }, std::to_string( entry.Visuals.m_iHealth ).c_str( ), Color( 255, 255, 255, ( int ) col.a ), TEXT_CENTER | TEXT_OUTLINE, 8, Render::Fonts.NameESP );
}