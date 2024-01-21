#pragma once
#include "entity_handle.h"
#include "interfaces/ischemasystem.h"
#include "../transform.h"
#include "../../core/displacement.h"
#include "skeleton.h"

#define VAR_AT_OFFSET( type, name, offset ) __forceinline type& name( ) const { \
		return *reinterpret_cast< type* >( ( reinterpret_cast< std::uintptr_t >( this ) + offset ) ); \
	} \

#define SCHEMA( type, name ) private: inline static uint32_t name##_OFFSET; \
		public: VAR_AT_OFFSET( type, name, name##_OFFSET )

class CCollisionProperty;
class CEntityInstance;

class CEntityIdentity {
public:
	VAR_AT_OFFSET( std::uint32_t, Index, 0x10 );

	SCHEMA( const char*, m_designerName );
	SCHEMA( std::uint32_t, m_flags );

	bool IsValid( ) {
		return Index( ) != INVALID_EHANDLE_INDEX;
	}

	int GetEntryIndex( ) {
		if ( !IsValid( ) )
			return ENT_ENTRY_MASK;

		return Index( ) & ENT_ENTRY_MASK;
	}

	int GetSerialNumber( ) {
		return Index( ) >> NUM_SERIAL_NUM_SHIFT_BITS;
	}

	CEntityInstance* pInstance; // 0x00

public:
	static void Initialise( );
};

// wow... what the fuck...
class CEntityInstance
{
public:
	SCHEMA( CEntityIdentity*, m_pEntity );

	void GetSchemaClassInfo( SchemaClassInfoData_t** pReturn ) {
		return Memory::CallVFunc<void, 34U>( this, pReturn );
	}

	CBaseHandle GetRefEHandle( ) {
		CEntityIdentity* identity{ m_pEntity( ) };
		if ( !identity )
			return CBaseHandle( );

		return CBaseHandle( identity->GetEntryIndex( ), identity->GetSerialNumber( ) - ( identity->m_flags( ) & 1 ) );
	}

public:
	static void Initialise( );
};

class CSkeletonInstance;
class C_CSPlayerPawn;

class CGameSceneNode {
public:
	SCHEMA( Vector, m_vecAbsOrigin );
	SCHEMA( Vector, m_vecOrigin );
	SCHEMA( CTransform, m_nodeToWorld );
	SCHEMA( bool, m_bDormant );

	VAR_AT_OFFSET( C_CSPlayerPawn*, m_pPawn, 0x30 );

	CSkeletonInstance* GetSkeletonInstance( ) {
		return Memory::CallVFunc< CSkeletonInstance*, 8 >( this );
	}

public:
	static void Initialise( );
};

class CSkeletonInstance : public CGameSceneNode {
public:
	SCHEMA( CModelState, m_modelState );
	SCHEMA( std::uint8_t, m_nHitboxSet );

public:
	static void Initialise( );
};

class CCollisionProperty
{
public:
	SCHEMA( Vector, m_vecMins );
	SCHEMA( Vector, m_vecMaxs );

public:
	static void Initialise( );
};

class CPlayer_WeaponServices
{
public:
	SCHEMA( CBaseHandle, m_hActiveWeapon );

public:
	static void Initialise( );
};

class C_BaseEntity : public CEntityInstance
{
public:
	SCHEMA( CGameSceneNode*, m_pGameSceneNode );
	SCHEMA( CCollisionProperty*, m_pCollision );
	SCHEMA( std::uint8_t, m_iTeamNum );
	SCHEMA( int, m_iHealth );
	SCHEMA( int, m_fFlags );
	SCHEMA( int, m_MoveType );
	SCHEMA( float, m_flWaterLevel );
	SCHEMA( float, m_flSimulationTime );
	SCHEMA( Vector, m_vecAbsVelocity );

	__forceinline bool Dormant( ) {
		if ( this->m_pGameSceneNode( ) )
			return m_pGameSceneNode( )->m_bDormant( );

		return true;
	}

	__forceinline Vector& GetAbsOrigin( ) const {
		if ( this->m_pGameSceneNode( ) )
			return m_pGameSceneNode( )->m_vecAbsOrigin( );

		return VecEmpty;
	}

public:
	static void Initialise( );
};

class C_BaseModelEntity : public C_BaseEntity
{
public:

};

class CEconItemDefinition {
public:
	char pad0[ 0x8 ];  // vtable
	void* m_pKVItem;
	uint16_t m_nDefIndex;
	CUtlVector<uint16_t> m_nAssociatedItemsDefIndexes;
	bool m_bEnabled;
	const char* m_szPrefab;
	uint8_t m_unMinItemLevel;
	uint8_t m_unMaxItemLevel;
	uint8_t m_nItemRarity;
	uint8_t m_nItemQuality;
	uint8_t m_nForcedItemQuality;
	uint8_t m_nDefaultDropItemQuality;
	uint8_t m_nDefaultDropQuantity;
	CUtlVector<void*> m_vecStaticAttributes;
	uint8_t m_nPopularitySeed;
	void* m_pPortraitsKV;
	const char* m_pszItemBaseName;
	bool m_bProperName;
	const char* m_pszItemTypeName;
	uint32_t m_unItemTypeID;
	const char* m_pszItemDesc;
};

class C_EconItemView {
public:
	CEconItemDefinition* GetStaticData( ) {
		return Memory::CallVFunc< CEconItemDefinition*, 13 >( this );
	}

	SCHEMA( std::uint16_t, m_iItemDefinitionIndex );
public:
	static void Initialise( );
};

class C_AttributeContainer {
public:
	SCHEMA( C_EconItemView*, m_Item );
public:
	static void Initialise( );
};

//C_CSWeaponBase : C_BasePlayerWeapon, C_EconEntity, [ C_BaseFlex, CBaseAnimGraph ], C_BaseModelEntity, C_BaseEntity

class C_EconEntity : public C_BaseModelEntity {
public:
	SCHEMA( C_AttributeContainer*, m_AttributeManager );

public:
	static void Initialise( );
};

class C_BasePlayerWeapon : public C_EconEntity {
public:
	SCHEMA( int32_t, m_iClip1 );
	SCHEMA( int32_t, m_iClip2 );

public:
	static void Initialise( );
};

class CCSWeaponBaseVData
{
public:
	SCHEMA( const char*, m_szName );
	VAR_AT_OFFSET( int32_t, m_iMaxClip1, 0x1fc );
	VAR_AT_OFFSET( int32_t, m_iMaxClip2, 0x200 );

public:
	static void Initialise( );
};

class C_CSWeaponBase : public C_BasePlayerWeapon {
public:
	// you can find this in cheat engine by first printin out the pointer of the weapon, then use the structure dissect feature, the class name is exposed.
	VAR_AT_OFFSET( CCSWeaponBaseVData*, m_pWeaponData, 0x360 );
	VAR_AT_OFFSET( std::uint32_t, Index, 0x10 );
};

//C_CSPlayerPawn : C_CSPlayerPawnBase, C_BasePlayerPawn, [ C_BaseCombatCharacter, C_BaseFlex, CBaseAnimGraph ], C_BaseModelEntity, C_BaseEntity, CEntityInstance
class C_BasePlayerPawn : public C_BaseModelEntity
{
public:
	SCHEMA( CPlayer_WeaponServices*, m_pWeaponServices );
	SCHEMA( CBaseHandle, m_hPawn );

public:
	static void Initialise( );
};

class C_CSPlayerPawnBase : public C_BasePlayerPawn
{
public:
	SCHEMA( C_CSWeaponBase*, m_pClippingWeapon );
	SCHEMA( int, m_iOldIDEntIndex );

public:
	static void Initialise( );
};

class C_CSPlayerPawn : public C_CSPlayerPawnBase
{
public:

};


class CBasePlayerController : public C_BaseEntity
{
public:
	SCHEMA( CBaseHandle, m_hPawn );

public:
	static void Initialise( );
};

class CCSPlayerController : public CBasePlayerController
{
public:
	SCHEMA( bool, m_bPawnIsAlive );
	SCHEMA( const char*, m_sSanitizedPlayerName );

public:
	static void Initialise( );
};

enum EItemDefinitionIndex : short
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SHIELD = 37,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFEGG = 41,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS = 69,
	WEAPON_BREACHCHARGE = 70,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE = 75,
	WEAPON_HAMMER = 76,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB = 81,
	WEAPON_DIVERSION = 82,
	WEAPON_FRAG_GRENADE = 83,
	WEAPON_SNOWBALL = 84,
	WEAPON_BUMPMINE = 85,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525,
	STUDDED_BLOODHOUND_GLOVES = 5027,
	T_GLOVES = 5028,
	CT_GLOVES = 5029,
	SPORTY_GLOVES = 5030,
	SLICK_GLOVES = 5031,
	LEATHER_HANDWRAPS = 5032,
	MOTORCYCLE_GLOVES = 5033,
	SPECIALIST_GLOVES = 5034,
	STUDDED_HYDRA_GLOVES = 5035,
};

enum EFlags : int
{
	FL_ONGROUND = ( 1 << 0 ), // entity is at rest / on the ground
	FL_DUCKING = ( 1 << 1 ), // player is fully crouched/uncrouched
	FL_ANIMDUCKING = ( 1 << 2 ), // player is in the process of crouching or uncrouching but could be in transition
	FL_WATERJUMP = ( 1 << 3 ), // player is jumping out of water
	FL_ONTRAIN = ( 1 << 4 ), // player is controlling a train, so movement commands should be ignored on client during prediction
	FL_INRAIN = ( 1 << 5 ), // entity is standing in rain
	FL_FROZEN = ( 1 << 6 ), // player is frozen for 3rd-person camera
	FL_ATCONTROLS = ( 1 << 7 ), // player can't move, but keeps key inputs for controlling another entity
	FL_CLIENT = ( 1 << 8 ), // entity is a client (player)
	FL_FAKECLIENT = ( 1 << 9 ), // entity is a fake client, simulated server side; don't send network messages to them
	FL_INWATER = ( 1 << 10 ), // entity is in water
	FL_FLY = ( 1 << 11 ),
	FL_SWIM = ( 1 << 12 ),
	FL_CONVEYOR = ( 1 << 13 ),
	FL_NPC = ( 1 << 14 ),
	FL_GODMODE = ( 1 << 15 ),
	FL_NOTARGET = ( 1 << 16 ),
	FL_AIMTARGET = ( 1 << 17 ),
	FL_PARTIALGROUND = ( 1 << 18 ), // entity is standing on a place where not all corners are valid
	FL_STATICPROP = ( 1 << 19 ), // entity is a static property
	FL_GRAPHED = ( 1 << 20 ),
	FL_GRENADE = ( 1 << 21 ),
	FL_STEPMOVEMENT = ( 1 << 22 ),
	FL_DONTTOUCH = ( 1 << 23 ),
	FL_BASEVELOCITY = ( 1 << 24 ), // entity have applied base velocity this frame
	FL_WORLDBRUSH = ( 1 << 25 ), // entity is not moveable/removeable brush (part of the world, but represented as an entity for transparency or something)
	FL_OBJECT = ( 1 << 26 ),
	FL_KILLME = ( 1 << 27 ), // entity is marked for death and will be freed by the game
	FL_ONFIRE = ( 1 << 28 ),
	FL_DISSOLVING = ( 1 << 29 ),
	FL_TRANSRAGDOLL = ( 1 << 30 ), // entity is turning into client-side ragdoll
	FL_UNBLOCKABLE_BY_PLAYER = ( 1 << 31 )
};

enum EMoveType : int
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY, // no gravity, but still collides with stuff
	MOVETYPE_FLYGRAVITY, // flies through the air and is affected by gravity
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH, // no clip to world, push and crush
	MOVETYPE_NOCLIP, // no gravity, no collisions, still do velocity/absvelocity
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER, // observer movement, depends on player's observer mode
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};

enum EBoneFlags : uint32_t {
	FLAG_NO_BONE_FLAGS = 0x0,
	FLAG_BONEFLEXDRIVER = 0x4,
	FLAG_CLOTH = 0x8,
	FLAG_PHYSICS = 0x10,
	FLAG_ATTACHMENT = 0x20,
	FLAG_ANIMATION = 0x40,
	FLAG_MESH = 0x80,
	FLAG_HITBOX = 0x100,
	FLAG_BONE_USED_BY_VERTEX_LOD0 = 0x400,
	FLAG_BONE_USED_BY_VERTEX_LOD1 = 0x800,
	FLAG_BONE_USED_BY_VERTEX_LOD2 = 0x1000,
	FLAG_BONE_USED_BY_VERTEX_LOD3 = 0x2000,
	FLAG_BONE_USED_BY_VERTEX_LOD4 = 0x4000,
	FLAG_BONE_USED_BY_VERTEX_LOD5 = 0x8000,
	FLAG_BONE_USED_BY_VERTEX_LOD6 = 0x10000,
	FLAG_BONE_USED_BY_VERTEX_LOD7 = 0x20000,
	FLAG_BONE_MERGE_READ = 0x40000,
	FLAG_BONE_MERGE_WRITE = 0x80000,
	FLAG_ALL_BONE_FLAGS = 0xfffff,
	BLEND_PREALIGNED = 0x100000,
	FLAG_RIGIDLENGTH = 0x200000,
	FLAG_PROCEDURAL = 0x400000,
};