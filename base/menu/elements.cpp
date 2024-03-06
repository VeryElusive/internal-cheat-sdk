#include "menu.h"
#include "abuse.h"

int weaponGroup{ };

void Menu::Register( ) {
	m_cTabs[ 0 ].m_szName = "A";
	m_cTabs[ 1 ].m_szName = "F";
	m_cTabs[ 2 ].m_szName = "C";
	m_cTabs[ 3 ].m_szName = "D";
	m_cTabs[ 4 ].m_szName = "E";

	auto aimbot{ &m_cTabs[ 0 ].m_vecSubtabs.emplace_back( "Aimbot", 2 ) };
	{
		auto generalGroup{ aimbot->AddGroup( "General", 1.f ) };
		{
			generalGroup->Register( ( "Enable" ), &Configs::m_cConfig.m_bRageBotEnable );
			//generalGroup->Register( ( "Weapon group" ), &weaponGroup, 9, RagebotWeaponGroups );
			generalGroup->Register( ( "Auto fire" ), &Configs::m_cConfig.m_bRageBotAutoFire );
			generalGroup->Register( ( "Auto scope" ), &Configs::m_cConfig.m_bRageBotAutoScope );
			generalGroup->Register( ( "Silent aim" ), &Configs::m_cConfig.m_bRageBotSilentAim );

			generalGroup->Register( ( "Hitchance" ), &Configs::m_cConfig.m_iRageBotHitchance, 0, 100 );

			generalGroup->Register( ( "Minimum damage" ), &Configs::m_cConfig.m_iRageBotMinimumDamage, 0, 100 );
			generalGroup->Register( ( "Scale damage" ), &Configs::m_cConfig.m_bRageBotScaleDamage );
			generalGroup->Register( ( "Penetrate walls" ), &Configs::m_cConfig.m_bRageBotAutowall );
			generalGroup->Register( ( "Minimum penetration damage" ), &Configs::m_cConfig.m_iRageBotMinimumPenetrationDamage, 0, 100, SHOW( Configs::m_cConfig.m_bRageBotAutowall ) );
			generalGroup->Register( ( "Minimum damage overide" ), &Configs::m_cConfig.m_bRageBotOverrideDamage );
			generalGroup->Register( &Configs::m_cConfig.m_kRageBotOverrideDamage );
			generalGroup->Register( ( "Minimum damage override" ), &Configs::m_cConfig.m_iRageBotMinimumOverrideDamage, 0, 100 );

			generalGroup->Register( ( "Auto stop" ), &Configs::m_cConfig.m_bRageBotAutostop );
			generalGroup->Register( ( "Move between shots" ), &Configs::m_cConfig.m_bRageBotAutostopMoveBetweenShots );
		}
		auto selectionGroup{ aimbot->AddGroup( "Selection", 0.6f ) };
		{
			selectionGroup->Register( ( "Hitboxes" ), 8, hitboxes );
			selectionGroup->Register( ( "Multipoint hitboxes" ), 8, multipoints );

			selectionGroup->Register( ( "Static point scale" ), &Configs::m_cConfig.m_bRageBotStaticPoints );
			selectionGroup->Register( ( "Head point scale" ), &Configs::m_cConfig.m_iRageBotHeadPointScale, 0, 110, SHOW( Configs::m_cConfig.m_bRageBotStaticPoints ) );
			selectionGroup->Register( ( "Body point scale" ), &Configs::m_cConfig.m_iRageBotBodyPointScale, 0, 110, SHOW( Configs::m_cConfig.m_bRageBotStaticPoints ) );
		}
		auto exploitsGroup{ aimbot->AddGroup( "Exploits", 0.4f ) };
		{
			exploitsGroup->Register( ( "Rapidfire" ), &Configs::m_cConfig.m_bRapidFire );

		}
	}

	auto antiaim{ &m_cTabs[ 0 ].m_vecSubtabs.emplace_back( "Anti-aim", 2 ) };
	{
		auto generalGroup{ antiaim->AddGroup( "General", 1.f ) };
		{
			generalGroup->Register( ( "Enable" ), &Configs::m_cConfig.m_bAntiAimEnable );
			generalGroup->Register( ( "Pitch" ), &Configs::m_cConfig.m_iAntiAimPitch, 5, pitch );
			generalGroup->Register( ( "Yaw" ), &Configs::m_cConfig.m_iAntiAimYaw, 4, yaw );
			generalGroup->Register( ( "Yaw add" ), &Configs::m_cConfig.m_iAntiAimYawAdd, 5, yawAdd );

			generalGroup->Register( ( "Yaw range" ), &Configs::m_cConfig.m_iAntiAimYawRange, 2, 180, SHOW( Configs::m_cConfig.m_iAntiAimYawAdd && Configs::m_cConfig.m_iAntiAimYawAdd != 3 ) );

			generalGroup->Register( ( "Yaw speed" ), &Configs::m_cConfig.m_iAntiAimYawSpeed, 1, 60, SHOW( Configs::m_cConfig.m_iAntiAimYawAdd == 2 || Configs::m_cConfig.m_iAntiAimYawAdd == 3 ) );
		}
	}

	m_cTabs[ 1 ].m_vecSubtabs.emplace_back( ( "Enemy" ), 2 );
	m_cTabs[ 1 ].m_vecSubtabs.emplace_back( ( "Team" ), 2 );
	m_cTabs[ 1 ].m_vecSubtabs.emplace_back( ( "Local" ), 2 );
	{
		for ( int i{ }; i < 3; ++i ) {
			auto subTab{ &m_cTabs[ 1 ].m_vecSubtabs.at( i ) };
			auto ESPGroup{ subTab->AddGroup( "ESP", 1.f ) };

			ESPGroup->Register( ( "Enable" ), &Configs::m_cConfig.m_bEnabled[ i ] );
			ESPGroup->Register( ( "Name" ), &Configs::m_cConfig.m_bName[ i ] );
			ESPGroup->Register( &Configs::m_cConfig.m_colName[ i ] );

			ESPGroup->Register( ( "Box" ), &Configs::m_cConfig.m_bBox[ i ] );
			ESPGroup->Register( &Configs::m_cConfig.m_colBox[ i ] );

			ESPGroup->Register( ( "Health" ), &Configs::m_cConfig.m_bHealth[ i ] );
			// TODO: make this conditional
			ESPGroup->Register( ( "Health color override" ), &Configs::m_cConfig.m_bHealthOverride[ i ]/*, SHOW( Configs::m_cConfig.m_bHealth[ i ] )*/ );
			ESPGroup->Register( &Configs::m_cConfig.m_colHealth[ i ] );

			ESPGroup->Register( ( "Ammo bar" ), &Configs::m_cConfig.m_bAmmo[ i ] );
			ESPGroup->Register( &Configs::m_cConfig.m_colAmmo[ i ] );

			ESPGroup->Register( ( "Weapon" ), &Configs::m_cConfig.m_iWeapon[ i ], 3, WeaponTypes );
			ESPGroup->Register( ( "Weapon color" ) );
			ESPGroup->Register( &Configs::m_cConfig.m_colWeapon[ i ] );

			ESPGroup->Register( ( "Skeleton" ), &Configs::m_cConfig.m_bSkeleton[ i ] );
			ESPGroup->Register( &Configs::m_cConfig.m_colSkeleton[ i ] );
		}
	}

	auto test1{ &m_cTabs[ 2 ].m_vecSubtabs.emplace_back( "test", 2 ) };
	auto misc{ &m_cTabs[ 3 ].m_vecSubtabs.emplace_back( "Misc", 2 ) };
	{
		auto generalGroup{ misc->AddGroup( "General", 1.f ) };
		{
			generalGroup->Register( ( "Thirdperson" ), &Configs::m_cConfig.m_bThirdperson );
			generalGroup->Register( &Configs::m_cConfig.m_kThirdperson );
			generalGroup->Register( ( "Thirdperson distance" ), &Configs::m_cConfig.m_iThirdpersonDistance, 30, 250 );

			generalGroup->Register( "Accent color" );
			generalGroup->Register( &Configs::m_cConfig.m_cAccent );			
			
			generalGroup->Register( "Secondary accent color" );
			generalGroup->Register( &Configs::m_cConfig.m_cAccent2 );
		}

		auto movementGroup{ misc->AddGroup( "Movement", 1.f ) };
		{
			movementGroup->Register( ( "Bunnyhop" ), &Configs::m_cConfig.m_bBunnyhop );
			movementGroup->Register( ( "Auto strafer" ), &Configs::m_cConfig.m_bAutoStrafer );
			movementGroup->Register( ( "Auto strafer speed" ), &Configs::m_cConfig.m_iAutoStraferSpeed, 0.f, 100.f, SHOW( Configs::m_cConfig.m_bAutoStrafer ) );
			movementGroup->Register( ( "Bungus jump bug" ), &Configs::m_cConfig.m_bBugWalk );
			movementGroup->Register( &Configs::m_cConfig.m_kBugWalk );
		}
	}

	auto configSubtab{ &m_cTabs[ 4 ].m_vecSubtabs.emplace_back( "Configs", 1 ) };
	{
		auto configGroup{ configSubtab->AddGroup( "Configurations", 1.f ) };

		configGroup->Register( &selectedConfig );
		configGroup->Register( &typedConfig );
		configGroup->Register( "Refresh", [ ] {
			Configs::Refresh( );
			} );

		configGroup->Register( "Create", [ ] {
			const auto filePath{ typedConfig };
			Configs::SaveCFG( filePath );
			Configs::Refresh( );
			} );

		configGroup->Register( "Remove", [ ] {
			Configs::Remove( selectedConfig );
			Configs::Refresh( );
			} );

		configGroup->Register( "Load", [ ] {
			const auto filePath{ Configs::m_vecFileNames.at( selectedConfig ) };
			Configs::LoadCFG( filePath );
			} );

		configGroup->Register( "Save", [ ] {
			const auto filePath{ Configs::m_vecFileNames.at( selectedConfig ) };
			Configs::SaveCFG( filePath );
			} );
	}
}

void Menu::LerpToCol( Color& col, Color to ) {
	col = Color(
		( to.r > col.r ? int( std::ceil( ( to.r - col.r ) * ANIMATION_SPEED ) + col.r ) : int( std::floor( ( to.r - col.r ) * ANIMATION_SPEED ) + col.r ) ),
		( to.g > col.g ? int( std::ceil( ( to.g - col.g ) * ANIMATION_SPEED ) + col.g ) : int( std::floor( ( to.g - col.g ) * ANIMATION_SPEED ) + col.g ) ),
		( to.b > col.b ? int( std::ceil( ( to.b - col.b ) * ANIMATION_SPEED ) + col.b ) : int( std::floor( ( to.b - col.b ) * ANIMATION_SPEED ) + col.b ) ),
		( to.a > col.a ? int( std::ceil( ( to.a - col.a ) * ANIMATION_SPEED ) + col.a ) : int( std::floor( ( to.a - col.a ) * ANIMATION_SPEED ) + col.a ) )
	);
}

void Menu::RenderElements( ) {
	const auto maxSize{ Menu::m_vecSize.y - 80 };
	const auto sizeDenom{ ( maxSize / 5 ) };

	auto& activeTab{ m_cTabs[ m_iSelectedTab ] };

	const auto backupSubtab{ activeTab.m_pSelectedSubtab };

	if ( !activeTab.m_pSelectedSubtab )
		activeTab.m_pSelectedSubtab = &activeTab.m_vecSubtabs.front( );

	const auto tabSize{ Render::GetTextSize( ( "A" ), 50, Render::Fonts.Tabs ) };

	// tabs
	{
		for ( int i{ }; i < 5; ++i ) {
			const auto pos{ Menu::m_vecPos + Vector2D( Math::Lerp( m_cTabs[ i ].m_flAnimation, static_cast< float >( BAR_SIZE / 2 ), tabSize.x / 2 + 10 ), 60 + sizeDenom * i ) };

			const auto hovered{ Input::Hovered( pos - Vector2D( tabSize.x / 2 + BAR_SIZE / 2, 0 ),  Vector2D{BAR_SIZE, tabSize.y } ) };
			if ( hovered && ( Input::Pressed( VK_LBUTTON ) || Input::Pressed( VK_RBUTTON ) || Input::Pressed( VK_MBUTTON ) ) ) {
				m_iSelectedTab = i;
				if ( Input::Pressed( VK_RBUTTON ) && m_cTabs[ i ].m_vecSubtabs.size( ) > 1 )
					m_cTabs[ i ].m_pSelectedSubtab = &m_cTabs[ i ].m_vecSubtabs.at( 1 );
				else if ( Input::Pressed( VK_MBUTTON ) && m_cTabs[ i ].m_vecSubtabs.size( ) > 2 )
					m_cTabs[ i ].m_pSelectedSubtab = &m_cTabs[ i ].m_vecSubtabs.at( 2 );
				else
					m_cTabs[ i ].m_pSelectedSubtab = &m_cTabs[ i ].m_vecSubtabs.front( );
			}

			LerpToCol( m_cTabs[ i ].m_cColor, m_iSelectedTab == i ? ( i % 2 == 0 ? ACCENT : ACCENT2 ) : hovered ? Color( 120, 120, 120 ) : Color( 90, 90, 90 ) );

			if ( m_iSelectedTab == i )
				m_cTabs[ i ].m_flAnimation = Math::Interpolate( m_cTabs[ i ].m_flAnimation, 1.f, ANIMATION_SPEED );
			else
				m_cTabs[ i ].m_flAnimation = Math::Interpolate( m_cTabs[ i ].m_flAnimation, 0.f, ANIMATION_SPEED );

			if ( m_cTabs[ i ].m_flAnimation <= 0.005f )
				m_cTabs[ i ].m_flAnimation = 0.f;
			else if ( m_cTabs[ i ].m_flAnimation >= 0.995f )
				m_cTabs[ i ].m_flAnimation = 1.f;

			Render::Text( pos, m_cTabs[ i ].m_szName, m_cTabs[ i ].m_cColor, TEXT_CENTER | TEXT_DROPSHADOW, 50, Render::Fonts.Tabs );
		}
	}

	auto& newActiveTab = m_cTabs[ m_iSelectedTab ];

	if ( newActiveTab.m_vecSubtabs.size( ) ) {
		int i{ };
		const char* longest{ };
		for ( auto& subtab : newActiveTab.m_vecSubtabs ) {
			if ( !longest || strlen( subtab.m_szName ) > strlen( longest ) )
				longest = subtab.m_szName;
		}


		if ( longest ) {
			// yikes... but i had to to make it more bold
			Render::Gradient( Menu::m_vecPos + Vector2D{ 0, 60 + sizeDenom * m_iSelectedTab }, { BAR_SIZE, tabSize.y }, OUTLINE_DARK.Alpha( 0 ), OUTLINE_DARK.Alpha( newActiveTab.m_flAnimation * 255.f ), true );
			Render::Gradient( Menu::m_vecPos + Vector2D{ 0, 60 + sizeDenom * m_iSelectedTab }, { BAR_SIZE, tabSize.y }, OUTLINE_DARK.Alpha( 0 ), OUTLINE_DARK.Alpha( newActiveTab.m_flAnimation * 255.f ), true );
			Render::Gradient( Menu::m_vecPos + Vector2D{ 0, 60 + sizeDenom * m_iSelectedTab }, { BAR_SIZE, tabSize.y }, OUTLINE_DARK.Alpha( 0 ), OUTLINE_DARK.Alpha( newActiveTab.m_flAnimation * 255.f ), true );
			Render::Gradient( Menu::m_vecPos + Vector2D{ 0, 60 + sizeDenom * m_iSelectedTab }, { BAR_SIZE, tabSize.y }, OUTLINE_DARK.Alpha( 0 ), OUTLINE_DARK.Alpha( newActiveTab.m_flAnimation * 255.f ), true );
		}

		for ( auto& subtab : newActiveTab.m_vecSubtabs ) {
			if ( !longest || strlen( subtab.m_szName ) > strlen( longest ) )
				longest = subtab.m_szName;

			const auto textSize{ Render::GetTextSize( subtab.m_szName, 13.f, Render::Fonts.Menu ) };

			const auto pos{ Menu::m_vecPos + Vector2D( Math::Lerp( newActiveTab.m_flAnimation, BAR_SIZE / 2 + 10, BAR_SIZE - 10 ), tabSize.y / 2 - ( newActiveTab.m_vecSubtabs.size( ) * 16 ) / 2.f + 60 + sizeDenom * m_iSelectedTab + i * 16 ) };

			const auto hovered{ Input::Hovered( pos - Vector2D( textSize.x, 0 ), Vector2D( textSize.x, 16 ) ) };
			if ( hovered && Input::Pressed( VK_LBUTTON ) && activeTab.m_szName == newActiveTab.m_szName )
				newActiveTab.m_pSelectedSubtab = &subtab;

			LerpToCol( subtab.m_cColor, newActiveTab.m_pSelectedSubtab == &subtab ? ( m_iSelectedTab % 2 ? ( i % 2 == 0 ? ACCENT2 : ACCENT ) : ( i % 2 == 0 ? ACCENT : ACCENT2 ) ) : hovered ? HOVERED_ELEMENT : DIM_ELEMENT );

			Render::Text( pos, subtab.m_szName, subtab.m_cColor.Alpha( newActiveTab.m_flAnimation * 255 ), TEXT_DROPSHADOW | TEXT_RIGHT, 13.f, Render::Fonts.Menu );
			i++;
		}
	}


	m_vecDrawPos = m_vecPos + Vector2D( BAR_SIZE + MARGIN, MARGIN );

	if ( newActiveTab.m_pSelectedSubtab->m_vecGroups.size( ) ) {
		if ( backupSubtab != newActiveTab.m_pSelectedSubtab ) {
			for ( auto& group : newActiveTab.m_pSelectedSubtab->m_vecGroups )
				group.Reset( );
		}

		int i{ };
		for ( auto& group : newActiveTab.m_pSelectedSubtab->m_vecGroups ) {
			i++;
			group.Render( i, newActiveTab.m_pSelectedSubtab->m_vecGroups );
		}
	}

	Render::RectFilled( m_vecPos + Vector2D{ BAR_SIZE + MARGIN, MARGIN - 2 }, m_vecSize - Vector2D{ BAR_SIZE + MARGIN, MARGIN - 2 }, BACKGROUND.Alpha( Menu::m_pFocusItem.m_flFocusAnim * 0.7f * 255.f ) );

	if ( Menu::m_pFocusItem.m_pItem ) {
		auto& element{ *Menu::m_pFocusItem.m_pItem };
		Menu::m_pFocusItem.m_flFocusAnim = Math::Interpolate( Menu::m_pFocusItem.m_flFocusAnim, m_pFocusItem.m_bFadeOut ? 0.f : 1.f, ANIMATION_SPEED );

		element.RenderFocus( );
	}
	else
		Menu::m_pFocusItem.m_flFocusAnim = Math::Interpolate( Menu::m_pFocusItem.m_flFocusAnim, 0.f, ANIMATION_SPEED );

	if ( Menu::m_pFocusItem.m_flFocusAnim <= 0.01f )
		Menu::m_pFocusItem.m_pItem = nullptr;
}