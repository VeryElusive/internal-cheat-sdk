#pragma once
#include "../sdk/matrix.h"

using fnGetBaseEntity = void* ( __thiscall* ) ( void*, int );
using fnSetViewAngles = void ( __fastcall* ) ( void*, int, Vector* angle );
using fnGetBoneParent = int ( __fastcall* ) ( void*, int );
using fnFindVarMaterial = std::uintptr_t( __fastcall* ) ( const char*, int, int );
using fnEventGetInt = std::int64_t( __fastcall* ) ( void*, const char* );

namespace Displacement {
	void Init( );

	inline fnGetBaseEntity GetBaseEntity;

	inline int( __stdcall* fnSetRelativeMouseMode )( int ) = nullptr;	
	
	inline fnSetViewAngles SetViewAngles;

	// both are same function args/type
	inline fnGetBoneParent GetBoneParent;
	inline fnGetBoneParent GetBoneFlags;

	inline fnEventGetInt EventGetInt;

	inline fnFindVarMaterial FindVarMaterial;
}