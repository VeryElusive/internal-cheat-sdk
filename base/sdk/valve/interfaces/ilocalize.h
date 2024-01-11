#pragma once
#include "../../../core/memory.h"

class ILocalize {
public:
    const char* FindSafe( const char* tokenName ) {
        return Memory::CallVFunc< const char*, 17u >( this, tokenName );
    }
};