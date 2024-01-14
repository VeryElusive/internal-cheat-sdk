#include "interfaces.h"
#include "memory.h"
#include "../utils/utils.h"

using InstantiateInterfaceFn_t = void* ( * )( );
class CInterfaceRegister
{
public:
	InstantiateInterfaceFn_t fnCreate;
	const char* szName;
	CInterfaceRegister* pNext;
};

template <typename T = void*>
T* Capture( const CInterfaceRegister* pModuleRegister, const char* szInterfaceName )
{
	for ( const CInterfaceRegister* pRegister = pModuleRegister; pRegister != nullptr; pRegister = pRegister->pNext )
	{
		if (strcmp( szInterfaceName, pRegister->szName ) == 0 )
		{
			// capture our interface
			void* pInterface = pRegister->fnCreate( );

			return static_cast< T* >( pInterface );
		}
	}

	return nullptr;
}

static const CInterfaceRegister* GetRegisterList( const wchar_t* wszModuleName )
{
	void* hModule = Memory::GetModuleBaseHandle( wszModuleName );
	if ( hModule == nullptr )
		return nullptr;

	std::uint8_t* pCreateInterface = reinterpret_cast< std::uint8_t* >( Memory::GetExportAddress( hModule, _( "CreateInterface" ) ) );

	if ( !pCreateInterface )
		return nullptr;

	return *reinterpret_cast< CInterfaceRegister** >( Memory::ResolveRelativeAddress( pCreateInterface, 0x3, 0x7 ) );
}

bool Interfaces::Init( ) {
	ENTER_REGION( "Interfaces" );

	const auto pEngineRegisterList = GetRegisterList( ENGINE2_DLL );
	CHECK_AND_FAIL( pEngineRegisterList );

	GameResourceService = Capture<IGameResourceService>	( pEngineRegisterList, _( "GameResourceServiceClientV001" ) );
	CHECK_AND_FAIL( GameResourceService );

	Engine = Capture<IEngineClient>		( pEngineRegisterList, _( "Source2EngineToClient001" ) );
	CHECK_AND_FAIL( Engine );

	const auto pClientRegister = GetRegisterList( CLIENT_DLL );
	CHECK_AND_FAIL( pClientRegister );

	Client = Capture<ISource2Client>( pClientRegister, _( "Source2Client002" ) );
	CHECK_AND_FAIL( Client );

	const auto pSchemaSystemRegisterList = GetRegisterList( SCHEMASYSTEM_DLL );
	CHECK_AND_FAIL( pSchemaSystemRegisterList );

	SchemaSystem = Capture<ISchemaSystem>( pSchemaSystemRegisterList, _( "SchemaSystem_001" ) );
	CHECK_AND_FAIL( SchemaSystem );

	const auto pTier0Handle = Memory::GetModuleBaseHandle( TIER0_DLL );
	CHECK_AND_FAIL( pTier0Handle );

	MemAlloc = *reinterpret_cast< IMemAlloc** >( Memory::GetExportAddress( pTier0Handle, _( "g_pMemAlloc" ) ) );
	CHECK_AND_FAIL( MemAlloc );

	const auto tier0Register = GetRegisterList( TIER0_DLL );
	CHECK_AND_FAIL( tier0Register );

	Cvar = Capture<IEngineCVar>( tier0Register, _( "VEngineCvar007" ) );

	const auto pLocalizeList = GetRegisterList( LOCALIZE_DLL );
	CHECK_AND_FAIL( pLocalizeList );

	Localize = Capture<ILocalize>( pLocalizeList, _( "Localize_001" ) );
	CHECK_AND_FAIL( Localize );

	// STRING XREF: "%s:  %f tick(%d) curtime(%f) OnSequenceCycleChanged: %s : %d=[%s]"
	GlobalVars = *reinterpret_cast< CGlobalVarsBase** >( Memory::ResolveRelativeAddress( Memory::FindPattern( CLIENT_DLL, _( "48 89 0D ? ? ? ? 48 89 41" ) ), 0x3, 0x7 ) );
	CHECK_AND_FAIL( GlobalVars );

	Input = *reinterpret_cast< CCSGOInput** >( Memory::ResolveRelativeAddress( Memory::FindPattern( CLIENT_DLL, _( "48 8B 0D ? ? ? ? 48 8B 01 FF 50 ? 8B DF" ) ), 0x3, 0x7 ) );
	CHECK_AND_FAIL( Input );

	return true;
}