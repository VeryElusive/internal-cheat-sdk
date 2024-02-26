#include "sdk/valve/entity.h"
#include "havoc.h"

CCSPlayerController* Context_t::GetLocal( ) {
	const int index{ Interfaces::Engine->GetLocalPlayer( ) };
	return Interfaces::GameResourceService->m_pGameEntitySystem->Get<CCSPlayerController>( index );
}

C_CSPlayerPawn* Context_t::GetPlayerPawn( CCSPlayerController* player ) {
	if ( !player )
		return nullptr;

	return Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( player->m_hPawn( ) );
}

C_CSPlayerPawn* Context_t::GetLocalPawn( ) {
	return GetPlayerPawn( GetLocal( ) );
}

CCSWeaponBaseVData* Context_t::GetWeaponData( C_CSPlayerPawn* localPawn ) {
	if ( !localPawn )
		return nullptr;

	const auto weaponServices{ localPawn->m_pWeaponServices( ) };
	if ( !weaponServices )
		return nullptr;

	const auto weapon{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSWeaponBase>( weaponServices->m_hActiveWeapon( ) ) };
	if ( !weapon )
		return nullptr;

	return weapon->m_pWeaponData( );
}

CCSWeaponBaseVData* Context_t::GetWeaponData( ) {
	const auto localPawn{ GetPlayerPawn( GetLocal( ) ) };
	if ( !localPawn )
		return nullptr;

	const auto weaponServices{ localPawn->m_pWeaponServices( ) };
	if ( !weaponServices )
		return nullptr;

	const auto weapon{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSWeaponBase>( weaponServices->m_hActiveWeapon( ) ) };
	if ( !weapon )
		return nullptr;

	return weapon->m_pWeaponData( );
}