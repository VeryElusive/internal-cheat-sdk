#include "config.h"
#include <fstream>

void Configs::SaveCFG( std::string filePath ) {
	std::filesystem::path fsFilePath( filePath );
	if ( fsFilePath.extension( ) != ".havoc" )
		fsFilePath.replace_extension( ".havoc" );

	const auto file{ std::filesystem::path( m_fsPath / fsFilePath ) };
	std::ofstream outFile( file, std::ios::out | std::ios::trunc );

	for ( int i = 0; i < sizeof( Config_t ); i++ ) {
		const auto currentByte{ *reinterpret_cast< char* >( uintptr_t( &Configs::m_cConfig ) + i ) };
		for ( int x = 0; x < 8; x++ )
			outFile << ( int ) ( ( currentByte >> x ) & 1 );
	}

	outFile.close( );
}

void Configs::LoadCFG( std::string filePath ) {
	std::filesystem::path fsFilePath( filePath );
	if ( fsFilePath.extension( ) != ".havoc" )
		fsFilePath.replace_extension( ".havoc" );

	const auto file{ std::filesystem::path( m_fsPath / fsFilePath ) };
	std::ifstream inputFile( file, std::ios::in );

	if ( !inputFile.good( ) )
		return;

	std::string line;
	while ( inputFile ) {
		std::getline( inputFile, line );

		const auto settingsSize{ sizeof( Config_t ) };
		if ( line.size( ) > ( settingsSize * 8 ) ) {
			inputFile.close( );
			return;
		}
		for ( int i = 0; i < settingsSize; i++ ) {
			auto currentByte{ *reinterpret_cast< char* >( uintptr_t( &Configs::m_cConfig ) + i ) };
			for ( int x = 0; x < 8; x++ ) {
				if ( line.size( ) < ( i * 8 ) + x || line[ ( i * 8 ) + x ] == '1' )
					currentByte |= 1 << x;
				else
					currentByte &= ~( 1 << x );
			}
			*reinterpret_cast< char* >( uintptr_t( &Configs::m_cConfig ) + i ) = currentByte;
		}
	}

	inputFile.close( );
}

void Configs::Refresh( ) {
	m_vecFileNames.clear( );
	const auto filePath{ "C:\\Havoc-Assault-Cube\\Configs" };

	for ( const auto& it : std::filesystem::directory_iterator( filePath ) ) {
		if ( it.path( ).filename( ).extension( ) == ".havoc" ) {
			m_vecFileNames.push_back( it.path( ).filename( ).string( ).substr( 0, it.path( ).filename( ).string( ).size( ) - 6 ) );
		}
	}
}

void Configs::Remove( const std::size_t nIndex ) {
	std::filesystem::path fsFilePath( Configs::m_vecFileNames.at( nIndex ) );
	if ( fsFilePath.extension( ) != ".havoc" )
		fsFilePath.replace_extension( ".havoc" );

	const auto file{ std::filesystem::path( m_fsPath / fsFilePath ) };

	std::filesystem::remove( file );
}