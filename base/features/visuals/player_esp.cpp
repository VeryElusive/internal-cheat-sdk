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

	if ( entry.m_pPawn->Dormant( ) ) {
		entry.Visuals.m_flAlpha -= ctx.m_flFrameTime / 5.f;
		entry.Visuals.m_flDormancyFade += ctx.m_flFrameTime;
	}
	else {
		entry.Visuals.m_flAlpha += ctx.m_flFrameTime * 2.f;
		entry.Visuals.m_flDormancyFade -= ctx.m_flFrameTime * 2.f;
	}

	BBox_t bbox;
	if ( !GetBBox( entry.m_pPawn, bbox ) )
		return;

	if ( Configs::m_cConfig.m_bBox[ type ] )
		DrawBox( entry, type, bbox );
}

void CVisuals::DrawBox( const PlayerEntry_t& entry, uint8_t type, const BBox_t& bbox ) {
	Color col{ Configs::m_cConfig.m_colBox[ type ] };
	col.a *= entry.Visuals.m_flAlpha;

	col = col.Lerp( DormantCol.Alpha( col.a * 0.4f ), entry.Visuals.m_flDormancyFade );

	const auto outline{ Color( 0, 0, 0, static_cast< int >( col.a ) ) };
	
	Render::Rect( bbox.Position( ) + 1, bbox.Size( ) - 2, outline );
	Render::Rect( bbox.Position( ), bbox.Size( ), col );
	Render::Rect( bbox.Position( ) - 1, bbox.Size( ) + 2, outline );
}