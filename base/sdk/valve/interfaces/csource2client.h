#pragma once
#include "../../../core/memory.h"
#include "../../vector.h"

class CSource2Client {
public:
	void GetViewAngles( Vector* angle ) {
		return Memory::CallVFunc<void, 72u>( this, 0, angle );
	}
};