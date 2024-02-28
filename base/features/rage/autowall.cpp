#include "autowall.h"
#include "../../sdk/valve/interfaces/ienginecvar.h"

bool CPenetration::FireBullet( Vector start, Vector end, 
    const C_CSPlayerPawn* shooter, const C_CSPlayerPawn* target,
    const CCSWeaponBaseVData* weaponData, PenetrationData_t& penData,
    const float minDmgCap ) {
    TraceData_t traceData{ };
    traceData.m_pUnk = &traceData.m_arr;

    const auto direction{ end - start };
    const auto endPos{ direction * weaponData->m_flRange( ) };

    const TraceFilter_t filter( K_MAIN_PEN_MASK, shooter, 3 );
    CreateTrace( &traceData, start, endPos, filter, 4 );

    penData.m_flDamage = weaponData->m_nDamage( );

    HandleBulletPenetrationData_t handleBulletPenetrationData( static_cast< float >( weaponData->m_nDamage( ) ),
        weaponData->m_flPenetration( ), weaponData->m_flRange( ),
        weaponData->m_flRangeModifier( ), 4, false );

    if ( traceData.m_iNumUpdate > 0 ) {
        for ( int i{ }; i < traceData.m_iNumUpdate; i++ ) {
            auto* value{ reinterpret_cast< UpdateValue_t* const >(
                reinterpret_cast< std::uintptr_t >( traceData.m_pUpdateValue )
                + i * sizeof( UpdateValue_t ) ) };

            GameTrace_t trace{ };
            InitTraceInfo( &trace );
            GetTraceInfo(
                &traceData, &trace, 0.0f,
                reinterpret_cast< void* >(
                    reinterpret_cast< std::uintptr_t >( &traceData.m_arr[ 0 ] )
                    + sizeof( UnknownTraceMember_t ) * ( value->m_hHandleIdx & K_ENT_ENTRY_MASK ) ) );

            if ( trace.m_pHitEntity == target ) {
                if ( penData.m_flDamage > 0.f )
                    ScaleDamage( trace.m_pHitboxData->m_iHitgroup, target, weaponData, penData.m_flDamage );

                penData.m_iHitGroup = trace.m_pHitboxData->m_iHitgroup;
                penData.m_bPenetrated = i == 0;
                return true;
            }

            if ( HandleBulletPenetration( &traceData, &handleBulletPenetrationData, value, false ) ) 
                goto FAILED;

            penData.m_flDamage = handleBulletPenetrationData.m_flDamage;

            if ( penData.m_flDamage < minDmgCap )
                goto FAILED;
        }
    }

    FAILED:
    penData.m_flDamage = 0;
    return false;
}

bool IsEntityArmored( const C_CSPlayerPawn* entity, const int hitgroup ) {
    if ( hitgroup == HITGROUP_HEAD )
        return entity->m_bHasHelmet( );

    return entity->m_ArmorValue( ) > 0;
}

void CPenetration::ScaleDamage( const int hitgroup, const C_CSPlayerPawn* entity, const CCSWeaponBaseVData* weaponData, float& damage ) {
    // ida: server.dll; 80 78 42 00 74 08 F3 0F 59 35 ?? ?? ?? ?? 80 BE 04 0D 00 00 00
    CVAR( mp_damage_scale_ct_head );
    CVAR( mp_damage_scale_t_head );
    CVAR( mp_damage_scale_ct_body );
    CVAR( mp_damage_scale_t_body );

    const auto damage_scale_ct_head = mp_damage_scale_ct_head->value.fl,
        damage_scale_t_head = mp_damage_scale_t_head->value.fl,
        damage_scale_ct_body = mp_damage_scale_ct_body->value.fl,
        damage_scale_t_body = mp_damage_scale_t_body->value.fl;

    const bool is_ct = entity->m_iTeamNum( ) == 3, is_t = entity->m_iTeamNum( ) == 2;

    float head_damage_scale = is_ct ? damage_scale_ct_head : is_t ? damage_scale_t_head : 1.0f;
    const float body_damage_scale = is_ct ? damage_scale_ct_body : is_t ? damage_scale_t_body : 1.0f;

    // server.dll; F3 0F 10 35 ?? ?? ?? ?? 0F 29 7C 24 30 44 0F 29 44 24
    // xref: mp_heavybot_damage_reduction_scale
    if ( entity->HasHeavyArmor( ) )
        head_damage_scale *= 0.5f;

    switch ( hitgroup ) {
    case HITGROUP_HEAD:
        damage *= weaponData->m_flHeadshotMultiplier( ) * head_damage_scale;
        break;
    case HITGROUP_CHEST:
    case HITGROUP_LEFTARM:
    case HITGROUP_RIGHTARM:
    case HITGROUP_NECK:
        damage *= body_damage_scale;
        break;
    case HITGROUP_STOMACH:
        damage *= body_damage_scale * 1.25f;
        break;
    case HITGROUP_LEFTLEG:
    case HITGROUP_RIGHTLEG:
        damage *= body_damage_scale * 0.75f;
        break;
    default:
        break;
    }

    if ( !IsEntityArmored( entity, hitgroup ) )
        return;

    float heavy_armor_bonus = 1.0f, armor_bonus = 0.5f, armor_ratio = weaponData->m_flArmorRatio( ) * 0.5f;

    if ( entity->HasHeavyArmor( ) ) {
        heavy_armor_bonus = 0.25f;
        armor_bonus = 0.33f;
        armor_ratio *= 0.20f;
    }

    float damage_to_health = damage * armor_ratio;
    const float damage_to_armor = ( damage - damage_to_health ) * ( heavy_armor_bonus * armor_bonus );

    if ( damage_to_armor > static_cast< float >( entity->m_ArmorValue( ) ) ) {
        damage_to_health = damage - static_cast< float >( entity->m_ArmorValue( ) ) / armor_bonus;
    }

    damage = damage_to_health;
}