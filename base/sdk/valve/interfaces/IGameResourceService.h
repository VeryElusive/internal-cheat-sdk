#pragma once
#include "../entity_handle.h"
#include "../../../core/displacement.h"

#define MAX_ENTITIES_IN_LIST 512
#define MAX_ENTITY_LISTS 64 // 0x3F
#define MAX_TOTAL_ENTITIES MAX_ENTITIES_IN_LIST* MAX_ENTITY_LISTS

class C_BaseEntity;

class CGameEntitySystem
{
public:
	/// GetClientEntity
	template <typename T = C_BaseEntity>
	T* Get( int nIndex ) {
		return reinterpret_cast< T* >( this->GetEntityByIndex( nIndex ) );
	}

	/// GetClientEntityFromHandle
	template <typename T = C_BaseEntity>
	T* Get( const CBaseHandle hHandle ) {
		if ( !hHandle.IsValid( ) )
			return nullptr;

		return reinterpret_cast< T* >( this->GetEntityByIndex( hHandle.GetEntryIndex( ) ) );
	}

	int GetHighestEntityIndex( ) {
		return *reinterpret_cast< int* >( reinterpret_cast< std::uintptr_t >( this ) + 0x1510 );
	}

private:
	void* GetEntityByIndex( int nIndex ) {
		//@ida: #STR: "(missing),", "(missing)", "Ent %3d: %s class %s name %s\n" | or find "cl_showents" cvar -> look for callback
		//	do { pEntity = GetBaseEntityByIndex(g_pGameEntitySystem, nCurrentIndex); ... }
		Displacement::GetBaseEntity( this, nIndex );
	}
};


class IGameResourceService
{
public:
	char pad0[ 0x58 ];
	CGameEntitySystem* m_pGameEntitySystem;
};