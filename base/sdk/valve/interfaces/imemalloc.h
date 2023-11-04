#pragma once
// used: mem::CallVFunc
#include "../../../core/memory.h"
#pragma warning(push)
#pragma warning(disable : 4191)

class IMemAlloc
{
public:
	void* Alloc( std::size_t nSize )
	{
		return Memory::CallVFunc<void*, 1>( this, nSize );
	}

	void* ReAlloc( const void* pMemory, std::size_t nSize )
	{
		return Memory::CallVFunc<void*, 2>( this, pMemory, nSize );
	}

	void Free( const void* pMemory )
	{
		return Memory::CallVFunc<void, 3>( this, pMemory );
	}

	std::size_t GetSize( const void* pMemory )
	{
		return Memory::CallVFunc<std::size_t, 21>( this, pMemory );
	}
};

#pragma warning(pop)