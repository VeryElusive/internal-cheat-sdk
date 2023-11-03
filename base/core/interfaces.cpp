#include "interfaces.h"
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
		if ( const std::size_t nInterfaceNameLength = strlen( szInterfaceName );
			// found needed interface
			strcmp( szInterfaceName, pRegister->szName ) == 0 ) )
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

void Interfaces::Init( ) {
	const auto pEngineRegisterList = GetRegisterList( ENGINE2_DLL );
	if ( !pEngineRegisterList )
		return;

	GameResourceService = Capture<IGameResourceService>	( pEngineRegisterList, _( "GameResourceServiceClientV001" ) );
	Engine				= Capture<IEngineClient>		( pEngineRegisterList, _( "Source2EngineToClient001" ) );

	GlobalVars = *reinterpret_cast< CGlobalVarsBase** >( Memory::ResolveRelativeAddress( Memory::FindPattern( CLIENT_DLL, _( "48 89 0D ? ? ? ? 48 89 41" ) ), 0x3, 0x7 ) );
}