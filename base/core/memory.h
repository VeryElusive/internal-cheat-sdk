#pragma once
#include "../havoc.h"

#define CLIENT_DLL _( L"client.dll" )
#define ENGINE2_DLL _( L"engine2.dll" )


#pragma region memory_definitions
#pragma warning(push)
#pragma warning(disable: 6255) // '_alloca' indicates failure by raising a stack overflow exception. consider using '_malloca' instead
#define MEM_STACKALLOC(SIZE) _malloca(SIZE)
#pragma warning(pop)
#define MEM_STACKFREE(MEMORY) static_cast<void>(0)

struct UNICODE_STRING {
	uint16_t	Length;
	uint16_t	MaximumLength;
	wchar_t* Buffer;
};

struct PEB_LDR_DATA {
	uint32_t		Length;
	uint8_t			Initialized;
	uintptr_t		SsHandle;
	LIST_ENTRY		InLoadOrderModuleList;
	LIST_ENTRY		InMemoryOrderModuleList;
	LIST_ENTRY		InInitializationOrderModuleList;
	uintptr_t		EntryInProgress;
	uint8_t			ShutdownInProgress;
	uintptr_t		ShutdownThreadId;
};

struct _PEB {
	uint8_t							InheritedAddressSpace;
	uint8_t							ReadImageFileExecOptions;
	uint8_t							BeingDebugged;
	uint8_t							BitField;
	uintptr_t						Mutant;
	void*						ImageBaseAddress;
	PEB_LDR_DATA* Ldr;
	uintptr_t		                ProcessParameters;
	uintptr_t						SubSystemData;
	uintptr_t						ProcessHeap;
	uintptr_t			            FastPebLock;
	uintptr_t						AtlThunkSListPtr;
	uintptr_t						IFEOKey;
	uintptr_t						CrossProcessFlags;
	union {
		uintptr_t						KernelCallbackTable;
		uintptr_t						UserSharedInfoPtr;
	};
	uint32_t						SystemReserved;
	uint32_t						AtlThunkSListPtr32;
	uintptr_t						ApiSetMap;
};

struct LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY				InLoadOrderLinks;
	LIST_ENTRY				InMemoryOrderLinks;
	LIST_ENTRY				InInitializationOrderLinks;
	void*				DllBase;
	uintptr_t				EntryPoint;
	uint32_t				SizeOfImage;
	UNICODE_STRING			FullDllName;
	UNICODE_STRING			BaseDllName;
};

// lookup table for fast hex integer to string conversion in range [00 .. FF]
constexpr char _TWO_DIGITS_HEX_LUT[ ] =
"000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F"
"202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F"
"404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F"
"606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E7F"
"808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9F"
"A0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
"C0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
"E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

namespace Memory {
	void* GetModuleBaseHandle( const wchar_t* wszModuleName );
	std::size_t PatternToBytes( const char* szPattern, std::uint8_t* pOutByteBuffer, char* szOutMaskBuffer );
	std::uint8_t* FindPattern( const wchar_t* wszModuleName, const char* szPattern );
	std::uint8_t* FindPattern( const wchar_t* wszModuleName, const char* szBytePattern, const char* szByteMask );
	std::uint8_t* FindPatternEx( const std::uint8_t* pRegionStart, const std::size_t nRegionSize, const std::uint8_t* arrByteBuffer, const std::size_t nByteCount, const char* szByteMask );
	std::size_t BytesToPattern( const std::uint8_t* pByteBuffer, const std::size_t nByteCount, char* szOutBuffer );
	void* GetExportAddress( const void* hModuleBase, const char* szProcedureName );

	__forceinline std::uint8_t* ResolveRelativeAddress( std::uint8_t* nAddressBytes, std::uint32_t nRVAOffset, std::uint32_t nRIPOffset )
	{
		std::uint32_t nRVA = *reinterpret_cast< std::uint32_t* >( nAddressBytes + nRVAOffset );
		std::uint64_t nRIP = reinterpret_cast< std::uint64_t >( nAddressBytes ) + nRIPOffset;

		return reinterpret_cast< std::uint8_t* >( nRVA + nRIP );
	}

	template <typename T, std::size_t nIndex, class CBaseClass, typename... Args_t>
	__forceinline T CallVFunc( CBaseClass* thisptr, Args_t... argList ) {
		using VirtualFn_t = T( __thiscall* )( const void*, decltype( argList )... );
		return ( *reinterpret_cast< VirtualFn_t* const* >( reinterpret_cast< std::uintptr_t >( thisptr ) ) )[ nIndex ]( thisptr, argList... );
	}
}