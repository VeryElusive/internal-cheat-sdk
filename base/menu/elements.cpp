#include "menu.h"
#include "abuse.h"

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
			generalGroup->Register( ( "Enable" ), &Configs::m_cConfig.m_bAimbotEnable );
			//generalGroup->Register( ( "Through walls" ), &Configs::m_cConfig.m_bAimbotThroughWalls );

		}
		auto otherGroup{ aimbot->AddGroup( "Other", 0.6f ) };
		auto other2Group{ aimbot->AddGroup( "Other", 0.4f ) };
	}

	auto antiaim{ &m_cTabs[ 0 ].m_vecSubtabs.emplace_back( "Anti-aim", 2 ) };

	auto test{ &m_cTabs[ 1 ].m_vecSubtabs.emplace_back( "test", 2 ) };
	auto test1{ &m_cTabs[ 2 ].m_vecSubtabs.emplace_back( "test", 2 ) };
	auto test2{ &m_cTabs[ 3 ].m_vecSubtabs.emplace_back( "Elements", 2 ) };
	{
		auto generalGroup{ test2->AddGroup( "Elements", 1.f ) };
		{
			generalGroup->Register( ( "Checkbox" ), &Configs::m_cConfig.m_bAimbotEnable );
			generalGroup->Register( &Configs::m_cConfig.m_cColorPickerTest );
			generalGroup->Register( ( "Slider float" ), &Configs::m_cConfig.m_iAimbotSliderFloat, 0, 100 );
			generalGroup->Register( ( "Slider int" ), &Configs::m_cConfig.m_iAimbotSlider, 0, 100 );
			generalGroup->Register( ( "Combo" ), &Configs::m_cConfig.m_iAimbotCombo, 4, comboElements );
			generalGroup->Register( ( "Multi combo" ), 5, multiComboElements );
			generalGroup->Register( ( "Button" ), [ ] {

				} );
		}
		auto otherGroup{ test2->AddGroup( "Other", 0.6f ) };
		auto other2Group{ test2->AddGroup( "Other", 0.4f ) };
	}

	auto configSubtab{ &m_cTabs[ 4 ].m_vecSubtabs.emplace_back( "Configs", 1 ) };
	{
		auto configGroup{ configSubtab->AddGroup( "Configurations", 1.f ) };

		configGroup->Register( &selectedConfig );
		configGroup->Register( &typedConfig );
		configGroup->Register( "Create", [ ] {
			const auto filePath{ typedConfig };
			Configs::SaveCFG( filePath );
			} );
		configGroup->Register( "Remove", [ ] {
			Configs::Remove( selectedConfig );
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

	if ( !activeTab.m_pSelectedSubtab && activeTab.m_vecSubtabs.size( ) )
		activeTab.m_pSelectedSubtab = &activeTab.m_vecSubtabs.front( );

	const auto tabSize{ Fonts::Tabs.GetTextSize( ( "A" ) ) };

	// subtabs lay underneath during anim
	if ( activeTab.m_vecSubtabs.size( ) ) {
		int i{ };
		for ( auto& subtab : activeTab.m_vecSubtabs ) {
			const auto textSize{ Fonts::Menu.GetTextSize( subtab.m_szName ) };

			const auto pos{ Menu::m_vecPos + Vector2D( Math::Lerp( activeTab.m_flAnimation, BAR_SIZE / 2 + 10, BAR_SIZE - 10 ), tabSize.y / 2 - ( activeTab.m_vecSubtabs.size( ) * 16 ) / 2.f + 80 + sizeDenom * m_iSelectedTab + i * 16 ) };

			const auto hovered{ Input::Hovered( pos - Vector2D( textSize.x, 0 ), Vector2D( textSize.x, 16 ) ) };
			if ( hovered && Input::Pressed( VK_LBUTTON ) )
				activeTab.m_pSelectedSubtab = &subtab;

			LerpToCol( subtab.m_cColor, activeTab.m_pSelectedSubtab == &subtab ? ( m_iSelectedTab % 2 ? ( i % 2 == 0 ? ACCENT2 : ACCENT ) : ( i % 2 == 0 ? ACCENT : ACCENT2 ) ) : hovered ? HOVERED_ELEMENT : DIM_ELEMENT );

			subtab.m_cColor.a = 255 * activeTab.m_flAnimation;
			Fonts::Menu.Render( pos, subtab.m_cColor, subtab.m_szName, RIGHT );
			i++;
		}
	}

	// tabs
	{
		for ( int i{ }; i < 5; ++i ) {
			const auto pos{ Menu::m_vecPos + Vector2D( static_cast<int>( Math::Lerp( m_cTabs[ i ].m_flAnimation, static_cast<float>( BAR_SIZE / 2 ), static_cast< float >( tabSize.x / 2 + 10 ) ) ), 100 + sizeDenom * i ) };

			const auto hovered{ Input::Hovered( pos - Vector2D( tabSize.x / 2 + BAR_SIZE / 2, tabSize.y - 5 ),  Vector2D{ BAR_SIZE, tabSize.y } ) };
			if ( hovered && ( Input::Pressed( VK_LBUTTON ) || Input::Pressed( VK_RBUTTON ) || Input::Pressed( VK_MBUTTON ) ) ) {
				m_iSelectedTab = i;
				if ( Input::Pressed( VK_RBUTTON ) && m_cTabs[ i ].m_vecSubtabs.size( ) > 1 )
					m_cTabs[ i ].m_pSelectedSubtab = &m_cTabs[ i ].m_vecSubtabs.at( 1 );
				else if ( Input::Pressed( VK_MBUTTON ) && m_cTabs[ i ].m_vecSubtabs.size( ) > 2 )
					m_cTabs[ i ].m_pSelectedSubtab = &m_cTabs[ i ].m_vecSubtabs.at( 2 );
				else
					m_cTabs[ i ].m_pSelectedSubtab = &m_cTabs[ i ].m_vecSubtabs.front( );
			}

			LerpToCol( m_cTabs[ i ].m_cColor, m_iSelectedTab == i ? ( i % 2 == 0 ? ACCENT : ACCENT2 ) : hovered ? HOVERED_ELEMENT : DIM_ELEMENT );

			if ( m_iSelectedTab == i )
				m_cTabs[ i ].m_flAnimation = Math::Interpolate( m_cTabs[ i ].m_flAnimation, 1.f, ANIMATION_SPEED );
			else
				m_cTabs[ i ].m_flAnimation = Math::Interpolate( m_cTabs[ i ].m_flAnimation, 0.f, ANIMATION_SPEED );

			if ( m_cTabs[ i ].m_flAnimation <= 0.005f )
				m_cTabs[ i ].m_flAnimation = 0.f;
			else if ( m_cTabs[ i ].m_flAnimation >= 0.995f )
				m_cTabs[ i ].m_flAnimation = 1.f;

			Fonts::Tabs.Render( pos, m_cTabs[ i ].m_cColor, m_cTabs[ i ].m_szName, CENTERED );
		}
	}

	auto& newActiveTab = m_cTabs[ m_iSelectedTab ];

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

	Render::RectFilled( m_vecPos + Vector2D{ BAR_SIZE + 1, MARGIN - 5 }, m_vecSize - Vector2D{ BAR_SIZE + MARGIN, MARGIN * 2 - 7 }, BACKGROUND.Alpha( Menu::m_pFocusItem.m_flFocusAnim * 0.7f * 255.f ) );

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