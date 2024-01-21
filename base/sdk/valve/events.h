#pragma once
#include "../../core/memory.h"
#include "key_string.h"
#include "entity.h"
#include <string_view>

struct Events_t {
    // xref: client.dll & 4C 8B F0 8B CE ("cs_pre_restart")
    __forceinline const char* GetName( ) noexcept { 
        return Memory::CallVFunc< const char*, 1 >( this );
    }

    __forceinline bool GetBool( const std::string_view token_name ) noexcept {
        KeyString_t token( token_name.data( ), -1 );
        return Memory::CallVFunc< bool, 6u >( this, &token, false );
    }

    // xref: client.dll & 89 85 08 0B 00 00 ("damage_taken")
    __forceinline int GetIntFromHash( const std::string_view token_name ) noexcept {
        KeyString_t token( token_name.data( ), -1 );
        return Memory::CallVFunc< int, 7u >( this, &token, 0 );
    }

    // xref: client.dll & 4C 8B A8 80 00 00 00 ("killer")
    __forceinline CBasePlayerController* GetPlayerController( const std::string_view token_name ) noexcept {
        KeyString_t token( token_name.data( ), -1 );
        return Memory::CallVFunc< CBasePlayerController*, 16u >( this, &token );
    }

    __forceinline float GetFloat( const std::string_view token_name ) noexcept {
        KeyString_t token( token_name.data( ), -1 );
        return Memory::CallVFunc< float, 9u >( this, &token, 0.0f );
    }

    // xref: client.dll & 48 89 54 24 10 48 89 4C 24 08 55 53 56 57 41 55 41 56 41 57 48 8D AC 24 D0 DF FF FF ("weapon")
    __forceinline const char* GetString( const std::string_view token_name ) noexcept {
        KeyString_t token( token_name.data( ), -1 );
        return Memory::CallVFunc< const char*, 10u >( this, &token, nullptr );
    }

    //void* get_player_pawn_from_id( std::string_view event_name ) noexcept;

    /* output (name | int):
        team: 2
        dmg_health: 21
        entityid: 316
        defindex: 49
        radius: 1100
    */
    std::int64_t GetInt( std::string_view event_name ) noexcept {
        // client.dll; 48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 41 56 48 83 EC 30 48 8B 01 41 8B F0 4C 8B F1 41 B0 01 48 8D 4C 24 20 48 8B DA 48 8B 78
        return Displacement::EventGetInt( this, event_name.data( ) );
    }

    //void* GetPointerFromID( const std::string_view event_name ) noexcept;
};

// TODO: if you can be botheredd, do events like we use to in csgo
/*class IGameEventManager2
{
public:
    enum
    {
        SERVERSIDE = 0, // this is a server side listener, event logger etc
        CLIENTSIDE, // this is a client side listenet, HUD element etc
        CLIENTSTUB, // this is a serverside stub for a remote client listener (used by engine only)
        SERVERSIDE_OLD, // legacy support for old server event listeners
        CLIENTSIDE_OLD // legacy support for old client event listeners
    };

    virtual ~IGameEventManager2( ) { }

    /// load game event descriptions from a file e.g. "resource\gameevents.res"
    ///  @Returns: count of loaded game events
    int LoadEventsFromFile( const char* szFileName )
    {
        return CallVFunc<int, 1U>( this, szFileName );
    }

    // remove all and anything
    void Reset( )
    {
        CallVFunc<void, 2U>( this );
    }

    /// add a listener for a particular event
    ///  @Returns: true if listener was successfully added for given event or was already there, false otherwise
    bool AddListener( IGameEventListener2* pListener, const char* szName, bool bServerSide = false )
    {
        return CallVFunc<bool, 3U>( this, pListener, szName, bServerSide );
    }

    ///  @Returns: true if given listener is listens to given event, false otherwise
    bool FindListener( IGameEventListener2* pListener, const char* szName )
    {
        return CallVFunc<bool, 4U>( this, pListener, szName );
    }

    // remove a listener
    void RemoveListener( IGameEventListener2* pListener )
    {
        CallVFunc<void, 5U>( this, pListener );
    }
};*/