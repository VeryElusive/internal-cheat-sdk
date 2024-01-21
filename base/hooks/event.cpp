#include "../core/hooks.h"
#include "../sdk/fnv1a.h"
#include "../havoc.h"

void __fastcall Hooks::hkHandleEvents( void* rcx, Events_t* event ) {
	const auto og{ HandleEvents.Original<decltype( &hkHandleEvents )>( ) };

    const char* eventName{ event->GetName( ) };

    // TODO: make a better option for deathmatch. __THIS WILL CRASH!__
    switch ( FNV1A::Hash( eventName ) ) {
    case FNV1A::HashConst( "round_end" ):
    case FNV1A::HashConst( "round_start" ):
        /*for ( auto it{ ctx.m_mapPlayerEntries.begin( ) }; it != ctx.m_mapPlayerEntries.end( ); ) {
            auto& entry{ *it };
            if ( !entry.second.m_pPawn 
                || !Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( entry.first ) ) {
                it = ctx.m_mapPlayerEntries.erase( it );
                continue;
            }

            ++it;
        }*/
        break;
    default:
        break;
    }

	og( rcx, event );
}