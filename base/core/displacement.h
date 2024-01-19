#pragma once
#include "../sdk/matrix.h"

using fnGetBaseEntity = void* ( __thiscall* ) ( void*, int );
using fnSetViewAngles = void ( __fastcall* ) ( void*, int, Vector* angle );
using fnGetBoneParent = int ( __fastcall* ) ( void*, int );

namespace Displacement {
	void Init( );

	inline fnGetBaseEntity GetBaseEntity;

	inline int( __stdcall* fnSetRelativeMouseMode )( int ) = nullptr;	
	
	inline fnSetViewAngles SetViewAngles;

	inline fnGetBoneParent GetBoneParent;
	//inline ViewMatrix_t* ViewMatrix;
}