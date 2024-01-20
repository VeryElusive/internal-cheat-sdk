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

	if ( Configs::m_cConfig.m_bBox[ type ] )
		DrawBox( entry, type );

	if ( Configs::m_cConfig.m_bName[ type ] )
		DrawName( entry, type );

	if ( Configs::m_cConfig.m_bHealth[ type ] )
		DrawHealth( entry, type );

	if ( Configs::m_cConfig.m_bSkeleton[ type ] )
		DrawSkeleton( entry, type );

	DrawWeapon( entry, type, DrawAmmo( entry, type ) );
}

void CVisuals::DrawBox( const PlayerEntry_t& entry, uint8_t type ) {
	Color col{ Configs::m_cConfig.m_colBox[ type ] };
	entry.Visuals.ApplyDormancy( col );

	const Color outline{ Color( 0, 0, 0, static_cast< int >( col.a ) ) };
	
	Render::Rect( entry.Visuals.m_cBBox.Position( ) + 1, entry.Visuals.m_cBBox.Size( ) - 2, outline );
	Render::Rect( entry.Visuals.m_cBBox.Position( ), entry.Visuals.m_cBBox.Size( ), col );
	Render::Rect( entry.Visuals.m_cBBox.Position( ) - 1, entry.Visuals.m_cBBox.Size( ) + 2, outline );
}

void CVisuals::DrawName( const PlayerEntry_t& entry, uint8_t type ) {
	Color col{ Configs::m_cConfig.m_colName[ type ] };
	entry.Visuals.ApplyDormancy( col );

	Render::Text( { entry.Visuals.m_cBBox.x + entry.Visuals.m_cBBox.w / 2, entry.Visuals.m_cBBox.y - 13 }, entry.m_pName, col, TEXT_CENTER | TEXT_DROPSHADOW, 13, Render::Fonts.NameESP );
}

void CVisuals::DrawHealth( const PlayerEntry_t& entry, uint8_t type ) {
	Color col{ Configs::m_cConfig.m_colHealth[ type ] };

	if ( !Configs::m_cConfig.m_bHealthOverride[ type ] ) {
		const int green{ static_cast< int >( static_cast< float >( std::min( entry.Visuals.m_iHealth, 100 ) ) * 2.55f ) };
		const int red{ 255 - green };
		col = Color( red, green, 0 );
	}

	entry.Visuals.ApplyDormancy( col );

	const Color outline{ Color( 0, 0, 0, static_cast< int >( col.a ) ) };

	const int bar_height{ static_cast< int >( static_cast< float >( entry.Visuals.m_iHealth ) * static_cast< float >( entry.Visuals.m_cBBox.h ) / 100.0f ) };
	const float offset{ entry.Visuals.m_cBBox.h - bar_height };

	Render::RectFilled( { entry.Visuals.m_cBBox.x - 6, entry.Visuals.m_cBBox.y - 1 }, { 4, entry.Visuals.m_cBBox.h + 2 }, outline.Alpha( outline.a * 0.5f ) );
	Render::RectFilled( { entry.Visuals.m_cBBox.x - 5, entry.Visuals.m_cBBox.y + offset }, { 2, bar_height }, col );

	Render::Rect( { entry.Visuals.m_cBBox.x - 6, entry.Visuals.m_cBBox.y - 1 }, { 4, entry.Visuals.m_cBBox.h + 2 }, outline );

	if ( entry.Visuals.m_iHealth <= 92 )
		Render::Text( { entry.Visuals.m_cBBox.x - 5, entry.Visuals.m_cBBox.y + offset - 3 }, std::to_string( entry.Visuals.m_iHealth ).c_str( ), Color( 255, 255, 255, ( int ) col.a ), TEXT_CENTER | TEXT_OUTLINE, 8, Render::Fonts.NameESP );
}

bool CVisuals::DrawAmmo( const PlayerEntry_t& entry, uint8_t type ) {
	if ( !Configs::m_cConfig.m_bAmmo[ type ] )
		return false;

	const auto weaponServices{ entry.m_pPawn->m_pWeaponServices( ) };
	if ( !weaponServices )
		return false;

	const auto weapon{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSWeaponBase>( weaponServices->m_hActiveWeapon( ) ) };
	if ( !weapon )
		return false;

	Color col{ Configs::m_cConfig.m_colAmmo[ type ] };
	entry.Visuals.ApplyDormancy( col );

	const auto weaponData{ weapon->m_pWeaponData( ) };
	if ( !weaponData )
		return false;

	const auto ammo{ weapon->m_iClip1( ) };
	const auto maxClip{ weaponData->m_iMaxClip1( ) };
	if ( !maxClip )
		return false;

	const Color outline{ Color( 0, 0, 0, static_cast< int >( col.a ) ) };

	// outline
	Render::RectFilled( { entry.Visuals.m_cBBox.x - 1, entry.Visuals.m_cBBox.y + entry.Visuals.m_cBBox.h + 2 }, { entry.Visuals.m_cBBox.w + 2, 4 }, outline.Alpha( outline.a * .5f ) );

	// color
	if ( ammo )
		Render::RectFilled( { entry.Visuals.m_cBBox.x, entry.Visuals.m_cBBox.y + entry.Visuals.m_cBBox.h + 3 }, { std::min( entry.Visuals.m_cBBox.w, ammo * entry.Visuals.m_cBBox.w / maxClip ), 2 }, col );

	Render::Rect( { entry.Visuals.m_cBBox.x - 1, entry.Visuals.m_cBBox.y + entry.Visuals.m_cBBox.h + 2 }, { entry.Visuals.m_cBBox.w + 2, 4 }, outline );

	return true;
}

void CVisuals::DrawWeapon( const PlayerEntry_t& entry, uint8_t type, const bool didAmmoBar ) {
	if ( !Configs::m_cConfig.m_iWeapon[ type ] )
		return;

	const auto weaponServices{ entry.m_pPawn->m_pWeaponServices( ) };
	if ( !weaponServices )
		return;

	const auto weapon{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSWeaponBase>( weaponServices->m_hActiveWeapon( ) ) };
	if ( !weapon )
		return;

	const auto identity{ weapon->m_pEntity( ) };
	if ( !identity )
		return;

	if ( !identity->m_designerName( ) )
		return;

	Color col{ Configs::m_cConfig.m_colWeapon[ type ] };
	entry.Visuals.ApplyDormancy( col );

	std::string name{ identity->m_designerName( ) };
	size_t pos = 0;
	while ( ( pos = name.find( "weapon_", pos ) ) != std::string::npos )
		name.erase( pos, 7 );

	std::replace( name.begin( ), name.end( ), '_', ' ' );

	for ( auto& n : name )
		n = std::toupper( n );

	int append{ 2 };
	if ( didAmmoBar )
		append += 6;

	Render::Text( { entry.Visuals.m_cBBox.x + entry.Visuals.m_cBBox.w / 2, entry.Visuals.m_cBBox.y + entry.Visuals.m_cBBox.h + append }, name.c_str( ), col, TEXT_CENTER | TEXT_OUTLINE, 12, Render::Fonts.NameESP );
}

void CVisuals::DrawSkeleton( const PlayerEntry_t& entry, uint8_t type ) {
	const auto gameSceneNode{ entry.m_pPawn->m_pGameSceneNode( ) };
	if ( !gameSceneNode )
		return;

	const auto skeleton{ gameSceneNode->GetSkeletonInstance( ) };
	if ( !skeleton )
		return;

	const auto modelState{ skeleton->m_modelState( ) };

	const auto model{ modelState.m_hModel };
	if ( !model.IsValid( ) )
		return;

	Color col{ Configs::m_cConfig.m_colSkeleton[ type ] };
	entry.Visuals.ApplyDormancy( col );

	for ( std::int32_t i = 0; i < model->m_iBoneCount; ++i ) {
		const auto boneFlag{ model->GetBoneFlags( i ) };
		if ( !( boneFlag & FLAG_HITBOX ) ) 
			continue;

		const auto boneParentIndex{ model->GetBoneParent( i ) };
		if ( boneParentIndex == -1 )
			continue;

		Vector2D screen{ };
		Vector2D parentScreen{ };

		if ( Render::WorldToScreen( modelState.m_pBones[ i ].m_vecPosition, screen ) 
			&& Render::WorldToScreen( modelState.m_pBones[ boneParentIndex ].m_vecPosition, parentScreen ) )
			Render::Line( screen, parentScreen, col );
	}
}