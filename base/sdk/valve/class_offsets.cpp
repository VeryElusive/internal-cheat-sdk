#include "entity.h"
#include "class_offsets.h"
#include "../../utils/schema.h"
#include "../../utils/math.h"
#include <string>

//e.g expansion: #define INIT_SCHEMA( table, var ) var = Schema::GetOffset( FNV1A::HashConst( "CBasePlayerController::m_hPawn" )

#ifdef _DEBUG
#define INIT_SCHEMA(table, var) var##_OFFSET = Schema::GetOffset( FNV1A::HashConst( CONCAT3(#table, "::", #var) ) ); printf("%s::%s -> 0x%X\n", #table, #var, var##_OFFSET);
#else
#define INIT_SCHEMA(table, var) var##_OFFSET = Schema::GetOffset( FNV1A::HashConst( CONCAT3(#table, "::", #var) ) )
#endif


void ClassOffsets::Init( ) {
	ENTER_REGION( "Schema" );

	CBasePlayerController::Initialise( );
	CGameSceneNode::Initialise( );
	CCollisionProperty::Initialise( );
	C_BaseEntity::Initialise( );
	CCSPlayerController::Initialise( );
	C_CSPlayerPawnBase::Initialise( );
	CCSWeaponBaseVData::Initialise( );
	C_BasePlayerPawn::Initialise( );
	CPlayer_WeaponServices::Initialise( );
	CPlayer_MovementServices::Initialise( );
	C_AttributeContainer::Initialise( );
	C_EconEntity::Initialise( );
	C_EconItemView::Initialise( );
	CEntityInstance::Initialise( );
	CEntityIdentity::Initialise( );
	C_BasePlayerWeapon::Initialise( );
	CSkeletonInstance::Initialise( );
	CCSPlayer_ItemServices::Initialise( );
	C_BaseModelEntity::Initialise( );
	C_CSWeaponBase::Initialise( );

	//CPlayer_CameraServices::Initialise( );

	const void* hTier0Lib{ Memory::GetModuleBaseHandle( TIER0_DLL ) };
	if ( hTier0Lib == nullptr )
		return;

	// other
	Math::RandomSeed = reinterpret_cast< decltype( Math::RandomSeed ) >( Memory::GetExportAddress( hTier0Lib, _( "RandomSeed" ) ) );
	Math::RandomFloat = reinterpret_cast< decltype( Math::RandomFloat ) >( Memory::GetExportAddress( hTier0Lib, _( "RandomFloat" ) ) );
	Math::RandomFloatExp = reinterpret_cast< decltype( Math::RandomFloatExp ) >( Memory::GetExportAddress( hTier0Lib, _( "RandomFloatExp" ) ) );
	Math::RandomInt = reinterpret_cast< decltype( Math::RandomInt ) >( Memory::GetExportAddress( hTier0Lib, _( "RandomInt" ) ) );
	Math::RandomGaussianFloat = reinterpret_cast< decltype( Math::RandomGaussianFloat ) >( Memory::GetExportAddress( hTier0Lib, _( "RandomGaussianFloat" ) ) );
}


void CBasePlayerController::Initialise( ) {
	INIT_SCHEMA( CBasePlayerController, m_hPawn );
}

void CCSPlayerController::Initialise( ) {
	INIT_SCHEMA( CCSPlayerController, m_bPawnIsAlive );
	INIT_SCHEMA( CCSPlayerController, m_sSanitizedPlayerName );
}

void CGameSceneNode::Initialise( ) {
	INIT_SCHEMA( CGameSceneNode, m_vecAbsOrigin );
	INIT_SCHEMA( CGameSceneNode, m_vecOrigin );
	INIT_SCHEMA( CGameSceneNode, m_nodeToWorld );
	INIT_SCHEMA( CGameSceneNode, m_bDormant );
}

void CCollisionProperty::Initialise( ) {
	INIT_SCHEMA( CCollisionProperty, m_vecMins );
	INIT_SCHEMA( CCollisionProperty, m_vecMaxs );
}

void C_BaseEntity::Initialise( ) {
	INIT_SCHEMA( C_BaseEntity, m_pGameSceneNode );
	INIT_SCHEMA( C_BaseEntity, m_pCollision );
	INIT_SCHEMA( C_BaseEntity, m_iTeamNum );
	INIT_SCHEMA( C_BaseEntity, m_iHealth );
	INIT_SCHEMA( C_BaseEntity, m_fFlags );
	INIT_SCHEMA( C_BaseEntity, m_vecAbsVelocity );
	INIT_SCHEMA( C_BaseEntity, m_MoveType );
	INIT_SCHEMA( C_BaseEntity, m_flWaterLevel );
	INIT_SCHEMA( C_BaseEntity, m_flSimulationTime );
}

void C_CSPlayerPawnBase::Initialise( ) {
	INIT_SCHEMA( C_CSPlayerPawnBase, m_pClippingWeapon );
	INIT_SCHEMA( C_CSPlayerPawnBase, m_iOldIDEntIndex );
	INIT_SCHEMA( C_CSPlayerPawnBase, m_ArmorValue );
	INIT_SCHEMA( C_CSPlayerPawnBase, m_bWaitForNoAttack );
	INIT_SCHEMA( C_CSPlayerPawnBase, m_bIsDefusing );
}

void CCSWeaponBaseVData::Initialise( ) {
	INIT_SCHEMA( CCSWeaponBaseVData, m_szName );
}

void C_BasePlayerPawn::Initialise( ) {
	INIT_SCHEMA( C_BasePlayerPawn, m_pWeaponServices );
	INIT_SCHEMA( C_BasePlayerPawn, m_pMovementServices );
	//INIT_SCHEMA( C_BasePlayerPawn, m_hPawn );
	INIT_SCHEMA( C_BasePlayerPawn, m_pItemServices );
	INIT_SCHEMA( C_BasePlayerPawn, m_vOldOrigin );
}

void CPlayer_WeaponServices::Initialise( ) {
	INIT_SCHEMA( CPlayer_WeaponServices, m_hActiveWeapon );
}

void CPlayer_MovementServices::Initialise( ) {
	INIT_SCHEMA( CPlayer_MovementServices, m_flMaxSpeed );
	INIT_SCHEMA( CPlayer_MovementServices, m_flSurfaceFriction );
}

void C_AttributeContainer::Initialise( ) {
	INIT_SCHEMA( C_AttributeContainer, m_Item );
}

void C_EconEntity::Initialise( ) {
	INIT_SCHEMA( C_EconEntity, m_AttributeManager );
}

void C_EconItemView::Initialise( ) {
	INIT_SCHEMA( C_EconItemView, m_iItemDefinitionIndex );
}

void CEntityInstance::Initialise( ) {
	INIT_SCHEMA( CEntityInstance, m_pEntity );
}

void CEntityIdentity::Initialise( ) {
	INIT_SCHEMA( CEntityIdentity, m_designerName );
	INIT_SCHEMA( CEntityIdentity, m_flags );
}

void C_BasePlayerWeapon::Initialise( ) {
	INIT_SCHEMA( C_BasePlayerWeapon, m_iClip1 );
	INIT_SCHEMA( C_BasePlayerWeapon, m_iClip2 );
	INIT_SCHEMA( C_BasePlayerWeapon, m_nNextPrimaryAttackTick );
	INIT_SCHEMA( C_BasePlayerWeapon, m_flNextPrimaryAttackTickRatio );
	INIT_SCHEMA( C_BasePlayerWeapon, m_nNextSecondaryAttackTick );
	INIT_SCHEMA( C_BasePlayerWeapon, m_flNextSecondaryAttackTickRatio );
}

void CSkeletonInstance::Initialise( ) {
	INIT_SCHEMA( CSkeletonInstance, m_modelState );
	INIT_SCHEMA( CSkeletonInstance, m_nHitboxSet );
}

void CCSPlayer_ItemServices::Initialise( ) {
	INIT_SCHEMA( CCSPlayer_ItemServices, m_bHasHeavyArmor );
}

/*void CPlayer_CameraServices::Initialise( ) {
	INIT_SCHEMA( CPlayer_ItemServices, m_ArmorValue );
}*/

void C_BaseModelEntity::Initialise( ) {
	INIT_SCHEMA( C_BaseModelEntity, m_vecViewOffset );
}

void C_CSWeaponBase::Initialise( ) {
	INIT_SCHEMA( C_CSWeaponBase, m_bInReload );
}