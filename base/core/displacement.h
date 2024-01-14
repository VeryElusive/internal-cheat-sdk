#pragma once
#include "../sdk/matrix.h"

using fnGetBaseEntity = void* ( __thiscall* ) ( void*, int );

namespace Displacement {
	void Init( );

	inline fnGetBaseEntity GetBaseEntity;
	//inline ViewMatrix_t* ViewMatrix;
}