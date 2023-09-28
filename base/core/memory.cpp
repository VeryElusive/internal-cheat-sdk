#include "memory.h"

std::vector<std::optional<std::uint8_t>> Memory::PatternToBytes( std::string pattern ) {
	std::vector<std::optional<std::uint8_t>> ret{ };

	for ( auto it{ pattern.begin( ) }; it != pattern.end( ); ++it ) {
		if ( *it == '?' )
			// this is a wildcard
			ret.push_back( std::nullopt );
		else if ( *it != ' ' ) {
			// convert current 4 bits to hex
			std::uint8_t byte{ static_cast< std::uint8_t >(
				( ( *it >= 'A' ? ( ( ( *it - 'A' ) & ( ~( 'a' ^ 'A' ) ) ) + 10 )
					: ( *it <= '9' ? *it - '0' : 0 ) )
					| 0xF0 ) << 4 ) };


			// convert next 4 bits to hex and assign to byte
			if ( ++it; *it != ' ' )
				byte |= static_cast< std::uint8_t >(
					*it >= 'A' ?
					( ( ( *it - 'A' ) & ( ~( 'a' ^ 'A' ) ) ) + 10 )
					: ( *it <= '9' ? *it - '0' : 0 ) );

			ret.push_back( byte );
		}
	}
	return ret;
}

std::uintptr_t Memory::GetBaseAddress( ) {
	HANDLE processSnapshot{ CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ) };
	if ( processSnapshot == INVALID_HANDLE_VALUE )
		return { };

	HANDLE process{ };
	PROCESSENTRY32 processInfo{ };
	processInfo.dwSize = sizeof( processInfo );

	if ( Process32First( processSnapshot, &processInfo ) ) {
		do {
			if ( !lstrcmp( processInfo.szExeFile, "ac_client.exe" ) ) {
				process = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processInfo.th32ProcessID );
				break;
			}
		} while ( Process32Next( processSnapshot, &processInfo ) );
	}
	CloseHandle( processSnapshot );

	if ( !process )
		return { };

	// credits: https://github.com/PierreCiholas/GetBaseAddress/blob/master/main.cpp
	HMODULE mod[ 1024 ]{ };// im not actually sure of this size
	DWORD tmp{ };
	if ( !EnumProcessModules( process, mod, sizeof( mod ), &tmp ) )
		return { }; // Impossible to read modules

	TCHAR modName[ MAX_PATH ]{ };
	if ( !GetModuleFileNameEx( process, mod[ 0 ], modName, sizeof( modName ) / sizeof( TCHAR ) ) )
		return { }; // Impossible to get module info

	return reinterpret_cast< std::uintptr_t >( mod[ 0 ] ); // base address
}

std::uintptr_t Memory::FindPattern( std::string pattern ) {
	const std::vector<std::optional<std::uint8_t>> bytesToSearch{ PatternToBytes( pattern ) };

	if ( !ACModuleHandle )
		ACModuleHandle = GetBaseAddress( );

	const auto moduleBytes{ reinterpret_cast< std::uint8_t* >( ACModuleHandle ) };
	if ( !moduleBytes )
		return{ };

	for ( std::uintptr_t i{ }; i < IMAGE_SIZE - bytesToSearch.size( ); ++i ) {
		bool found{ true };

		for ( std::uintptr_t j{ }; j < bytesToSearch.size( ); ++j ) {
			if ( bytesToSearch.at( j ).has_value( )
				&& moduleBytes[ i + j ] != bytesToSearch.at( j ).value( ) ) {
				// this set didn't match our bytes
				found = false;
				break;
			}
		}

		// return valid address
		if ( found )
			return reinterpret_cast< std::uintptr_t >( &moduleBytes[ i ] );
	}

	return { };
}