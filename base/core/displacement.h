#pragma once
#include "../sdk/matrix.h"
#include "../sdk/valve/trace.h"

#define VAR_AT_OFFSET( type, name, offset ) __forceinline type& name( ) const { \
		return *reinterpret_cast< type* >( ( reinterpret_cast< std::uintptr_t >( this ) + offset ) ); \
	} \

#define SCHEMA( type, name ) private: inline static uint32_t name##_OFFSET; \
		public: VAR_AT_OFFSET( type, name, name##_OFFSET )

using fnGetBaseEntity = void* ( __thiscall* ) ( void*, int );
using fnSetViewAngles = void ( __fastcall* ) ( void*, int, Vector* angle );
using fnGetBoneParent = int ( __fastcall* ) ( void*, int );
using fnFindVarMaterial = std::uintptr_t( __fastcall* ) ( const char*, int, int );
using fnEventGetInt = std::int64_t( __fastcall* ) ( void*, const char* );
using fnGetHitboxSet = std::int64_t( __fastcall* ) ( void*, unsigned int );
using fnTraceFilterInit = void( __fastcall* ) ( void*, const void*, std::uintptr_t, int, int );
using fnCreateTrace = void( __fastcall* ) ( TraceData_t*, Vector, Vector, TraceFilter_t, void*, void*, void*, void*, int );
using fnInitTraceInfo = void( __fastcall* ) ( GameTrace_t* );
using fnGetTraceInfo = void( __fastcall* ) ( TraceData_t*, GameTrace_t*, float, void* );
using fnHandleBulletPenetration = bool( __fastcall* ) ( TraceData_t*, HandleBulletPenetrationData_t*, UpdateValue_t*, void*, void*, void*, void*, void*, bool );
using fnGetBoneIndexFromHitbox = __int64( __fastcall* ) ( void*, unsigned int );

namespace Displacement {
	void Init( );

	inline fnGetBaseEntity GetBaseEntity;

	inline int( __stdcall* fnSetRelativeMouseMode )( int ) = nullptr;	
	
	inline fnSetViewAngles SetViewAngles;

	// both are same function args/type
	inline fnGetBoneParent GetBoneParent;
	inline fnGetBoneParent GetBoneFlags;

	inline fnGetHitboxSet GetHitboxSet;

	inline fnTraceFilterInit TraceFilterInit;

	inline fnCreateTrace CreateTrace;
	inline fnInitTraceInfo InitTraceInfo;
	inline fnGetTraceInfo GetTraceInfo;

	inline fnHandleBulletPenetration HandleBulletPenetration;

	inline fnGetBoneIndexFromHitbox GetBoneIndexFromHitbox;

	inline fnEventGetInt EventGetInt;

	inline fnFindVarMaterial FindVarMaterial;
}