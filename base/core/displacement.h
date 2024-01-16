#pragma once
#include "../sdk/matrix.h"

using fnGetBaseEntity = void* ( __thiscall* ) ( void*, int );
using fnSetViewAngles = void ( __fastcall* ) ( void*, int, Vector* angle );

namespace Displacement {
	void Init( );

	inline fnGetBaseEntity GetBaseEntity;

	inline int( __stdcall* fnSetRelativeMouseMode )( int ) = nullptr;	
	
	inline fnSetViewAngles SetViewAngles;
	//inline ViewMatrix_t* ViewMatrix;
}