#pragma once

using fnGetBaseEntity = void* ( __thiscall* ) ( void*, int );

namespace Displacement {
	void Init( );

	inline fnGetBaseEntity GetBaseEntity;
}