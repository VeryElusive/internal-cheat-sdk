#include "utlmemory.h"
#include "../core/interfaces.h"

template <class T, class N>
void* CUtlMemory<T,N>::operator new( const std::size_t nSize )
{
	return Interfaces::MemAlloc->Alloc( nSize );
}

template <class T, class N>
void CUtlMemory<T, N>::operator delete( void* pMemory )
{
	Interfaces::MemAlloc->Free( pMemory );
}

template <class T, class N>
CUtlMemory<T, N>::CUtlMemory( const int nInitialGrowSize, const int nAllocationCount ) :
	pMemory( nullptr ), nAllocationCount( nAllocationCount ), nGrowSize( nInitialGrowSize )
{
	ASSERT( nInitialGrowSize >= 0 );

	if ( nAllocationCount > 0 )
		pMemory = static_cast< T* >( Interfaces::MemAlloc->Alloc( nAllocationCount * sizeof( T ) ) );
}

template <class T, class N>
void CUtlMemory<T, N>::Grow( const int nCount )
{
	if ( IsExternallyAllocated( ) )
		return;

	int nAllocationRequested = nAllocationCount + nCount;
	int nNewAllocationCount = 0;

	if ( nGrowSize )
		nAllocationCount = ( ( 1 + ( ( nAllocationRequested - 1 ) / nGrowSize ) ) * nGrowSize );
	else
	{
		if ( nAllocationCount == 0 )
			nAllocationCount = ( 31 + sizeof( T ) ) / sizeof( T );

		while ( nAllocationCount < nAllocationRequested )
			nAllocationCount <<= 1;
	}

	if ( static_cast< int >( static_cast< N >( nNewAllocationCount ) ) < nAllocationRequested )
	{
		if ( static_cast< int >( static_cast< N >( nNewAllocationCount ) ) == 0 && static_cast< int >( static_cast< N >( nNewAllocationCount - 1 ) ) >= nAllocationRequested )
			--nNewAllocationCount;
		else
		{
			if ( static_cast< int >( static_cast< N >( nAllocationRequested ) ) != nAllocationRequested )
				return;

			while ( static_cast< int >( static_cast< N >( nNewAllocationCount ) ) < nAllocationRequested )
				nNewAllocationCount = ( nNewAllocationCount + nAllocationRequested ) / 2;
		}
	}

	nAllocationCount = nNewAllocationCount;

	// @test: we can always call realloc, since it must allocate instead when passed null ptr
	if ( pMemory != nullptr )
		pMemory = static_cast< T* >( Interfaces::MemAlloc->ReAlloc( pMemory, nAllocationCount * sizeof( T ) ) );
	else
		pMemory = static_cast< T* >( Interfaces::MemAlloc->Alloc( nAllocationCount * sizeof( T ) ) );
}

template <class T, class N>
void CUtlMemory<T, N>::EnsureCapacity( const int nCapacity )
{
	if ( nAllocationCount >= nCapacity )
		return;

	if ( IsExternallyAllocated( ) )
	{
		// can't grow a buffer whose memory was externally allocated
		ASSERT( false );
		return;
	}

	nAllocationCount = nCapacity;

	// @test: we can always call realloc, since it must allocate instead when passed null ptr
	if ( pMemory != nullptr )
		pMemory = static_cast< T* >( Interfaces::MemAlloc->ReAlloc( pMemory, nAllocationCount * sizeof( T ) ) );
	else
		pMemory = static_cast< T* >( Interfaces::MemAlloc->Alloc( nAllocationCount * sizeof( T ) ) );
}

template <class T, class N>
void CUtlMemory<T, N>::ConvertToGrowableMemory( int nInitialGrowSize )
{
	if ( !IsExternallyAllocated( ) )
		return;

	nGrowSize = nInitialGrowSize;

	if ( nAllocationCount > 0 )
	{
		const int nByteCount = nAllocationCount * sizeof( T );
		T* pGrowableMemory = static_cast< T* >( Interfaces::MemAlloc->Alloc( nByteCount ) );
		std::memcpy( pGrowableMemory, pMemory, nByteCount );
		pMemory = pGrowableMemory;
	}
	else
		pMemory = nullptr;
}

template <class T, class N>
void CUtlMemory<T, N>::Purge( )
{
	if ( IsExternallyAllocated( ) )
		return;

	if ( pMemory != nullptr )
	{
		Interfaces::MemAlloc->Free( static_cast< void* >( pMemory ) );
		pMemory = nullptr;
	}

	nAllocationCount = 0;
}

template <class T, class N>
void CUtlMemory<T, N>::Purge( const int nElements )
{
	ASSERT( nElements >= 0 );

	if ( nElements > nAllocationCount )
	{
		// ensure this isn't a grow request in disguise
		ASSERT( nElements <= nAllocationCount );
		return;
	}

	if ( nElements == 0 )
	{
		Purge( );
		return;
	}

	if ( IsExternallyAllocated( ) || nElements == nAllocationCount )
		return;

	if ( pMemory == nullptr )
	{
		// allocation count is non zero, but memory is null
		ASSERT( false );
		return;
	}

	nAllocationCount = nElements;
	pMemory = static_cast< T* >( Interfaces::MemAlloc->ReAlloc( pMemory, nAllocationCount * sizeof( T ) ) );
}