#pragma once
#include "../sdk/matrix.h"

using fnGetBaseEntity = void* ( __thiscall* ) ( void*, int );

namespace Displacement {
	void Init( );

	inline fnGetBaseEntity GetBaseEntity;
	inline int( __stdcall* fnSetRelativeMouseMode )( int ) = nullptr;
	//inline ViewMatrix_t* ViewMatrix;
}