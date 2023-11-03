#include "memory.h"
#include "../utils/utils.h"

std::size_t Memory::PatternToBytes( const char* szPattern, std::uint8_t* pOutByteBuffer, char* szOutMaskBuffer )
{
	std::uint8_t* pCurrentByte = pOutByteBuffer;

	while ( *szPattern != '\0' )
	{
		// check is a wildcard
		if ( *szPattern == '?' )
		{
			++szPattern;
#ifdef CS_PARANOID
			CS_ASSERT( *szPattern == '\0' || *szPattern == ' ' || *szPattern == '?' ); // we're expect that next character either terminating null, whitespace or part of double wildcard (note that it's required if your pattern written without whitespaces)
#endif

			// ignore that
			* pCurrentByte++ = 0U;
			*szOutMaskBuffer++ = '?';
		}
		// check is not space
		else if ( *szPattern != ' ' )
		{
			// convert two consistent numbers in a row to byte value
			std::uint8_t uByte = static_cast< std::uint8_t >( Utils::CharToHexInt( *szPattern ) << 4 );

			++szPattern;
#ifdef CS_PARANOID
			CS_ASSERT( *szPattern != '\0' && *szPattern != '?' && *szPattern != ' ' ); // we're expect that byte always represented by two numbers in a row
#endif

			uByte |= static_cast< std::uint8_t >( Utils::CharToHexInt( *szPattern ) );

			*pCurrentByte++ = uByte;
			*szOutMaskBuffer++ = 'x';
		}

		++szPattern;
	}

	// zero terminate both buffers
	*pCurrentByte = 0U;
	*szOutMaskBuffer = '\0';

	return pCurrentByte - pOutByteBuffer;
}
std::uint8_t* Memory::FindPattern( const wchar_t* wszModuleName, const char* szPattern )
{
	// convert pattern string to byte array
	const std::size_t nApproximateBufferSize = ( strlen( szPattern ) >> 1U ) + 1U;
	std::uint8_t* arrByteBuffer = static_cast< std::uint8_t* >( MEM_STACKALLOC( nApproximateBufferSize ) );
	char* szMaskBuffer = static_cast< char* >( MEM_STACKALLOC( nApproximateBufferSize ) );
	PatternToBytes( szPattern, arrByteBuffer, szMaskBuffer );

	// @test: use search with straight in-place conversion? do not think it will be faster, cuz of bunch of new checks that gonna be performed for each iteration
	return FindPattern( wszModuleName, reinterpret_cast< const char* >( arrByteBuffer ), szMaskBuffer );
}

void* Memory::GetModuleBaseHandle( const wchar_t* wszModuleName )
{
	const _PEB* pPEB = reinterpret_cast< _PEB* >( __readgsqword( 0x60 ) );

	if ( wszModuleName == nullptr )
		return pPEB->ImageBaseAddress;

	void* pModuleBase = nullptr;
	for ( LIST_ENTRY* pListEntry = pPEB->Ldr->InMemoryOrderModuleList.Flink; pListEntry != &pPEB->Ldr->InMemoryOrderModuleList; pListEntry = pListEntry->Flink )
	{
		const LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD( pListEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks );

		if ( pEntry->FullDllName.Buffer != nullptr && lstrcmpW( wszModuleName, pEntry->BaseDllName.Buffer ) == 0 )
		{
			pModuleBase = pEntry->DllBase;
			break;
		}
	}

	return pModuleBase;
}

std::uint8_t* Memory::FindPattern( const wchar_t* wszModuleName, const char* szBytePattern, const char* szByteMask )
{
	const void* hModuleBase = GetModuleBaseHandle( wszModuleName );

	if ( hModuleBase == nullptr )
		return nullptr;

	const auto pBaseAddress = static_cast< const std::uint8_t* >( hModuleBase );

	const auto pIDH = static_cast< const IMAGE_DOS_HEADER* >( hModuleBase );
	if ( pIDH->e_magic != IMAGE_DOS_SIGNATURE )
		return nullptr;

	const auto pINH = reinterpret_cast< const IMAGE_NT_HEADERS* >( pBaseAddress + pIDH->e_lfanew );
	if ( pINH->Signature != IMAGE_NT_SIGNATURE )
		return nullptr;

	const std::uint8_t* arrByteBuffer = reinterpret_cast< const std::uint8_t* >( szBytePattern );
	const std::size_t nByteCount = strlen( szByteMask );

	std::uint8_t* pFoundAddress = nullptr;

	pFoundAddress = FindPatternEx( pBaseAddress, pINH->OptionalHeader.SizeOfImage, arrByteBuffer, nByteCount, szByteMask );

	if ( pFoundAddress == nullptr ) {
		char* szPattern = static_cast< char* >( MEM_STACKALLOC( ( nByteCount << 1U ) + nByteCount ) );
		[[maybe_unused]] const std::size_t nConvertedPatternLength = BytesToPattern( arrByteBuffer, nByteCount, szPattern );

		MEM_STACKFREE( szPattern );
	}

	return pFoundAddress;
}

std::uint8_t* Memory::FindPatternEx( const std::uint8_t* pRegionStart, const std::size_t nRegionSize, const std::uint8_t* arrByteBuffer, const std::size_t nByteCount, const char* szByteMask )
{
	std::uint8_t* pCurrentAddress = const_cast< std::uint8_t* >( pRegionStart );
	const std::uint8_t* pRegionEnd = pRegionStart + nRegionSize - nByteCount;
	const bool bIsMaskUsed = ( szByteMask != nullptr );

	while ( pCurrentAddress < pRegionEnd )
	{
		// check the first byte before entering the loop, otherwise if there two consecutive bytes of first byte in the buffer, we may skip both and fail the search
		if ( ( bIsMaskUsed && *szByteMask == '?' ) || *pCurrentAddress == *arrByteBuffer )
		{
			if ( nByteCount == 1 )
				return pCurrentAddress;

			// compare the least byte sequence and continue on wildcard or skip forward on first mismatched byte
			std::size_t nComparedBytes = 0U;
			while ( ( bIsMaskUsed && szByteMask[ nComparedBytes + 1U ] == '?' ) || pCurrentAddress[ nComparedBytes + 1U ] == arrByteBuffer[ nComparedBytes + 1U ] )
			{
				// check does byte sequence match
				if ( ++nComparedBytes == nByteCount - 1U )
					return pCurrentAddress;
			}

			// skip non suitable bytes
			pCurrentAddress += nComparedBytes;
		}

		++pCurrentAddress;
		}

	return nullptr;
}

std::size_t Memory::BytesToPattern( const std::uint8_t* pByteBuffer, const std::size_t nByteCount, char* szOutBuffer )
{
	char* szCurrentPattern = szOutBuffer;

	for ( std::size_t i = 0U; i < nByteCount; i++ )
	{
		// manually convert byte to chars
		const char* szHexByte = &_TWO_DIGITS_HEX_LUT[ pByteBuffer[ i ] * 2U ];
		*szCurrentPattern++ = szHexByte[ 0 ];
		*szCurrentPattern++ = szHexByte[ 1 ];
		*szCurrentPattern++ = ' ';
	}
	*--szCurrentPattern = '\0';

	return szCurrentPattern - szOutBuffer;
}