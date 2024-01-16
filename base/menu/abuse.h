#pragma once

#define SHOW( cond ) [ ]( ) { return cond; }

ComboElement_t WeaponTypes[ ]{ ( "None" ), ( "Icon" ), ( "Text" ) };

ComboElement_t pitch[ 5 ]{ _( "Default" ), _( "Up" ), _( "Down" ), _( "Zero" ), _( "Random" ) };
ComboElement_t yaw[ 4 ]{ _( "Default" ), _( "Backward" ), _( "Left" ), _( "Right" ) };
ComboElement_t yawAdd[ 5 ]{ _( "None" ), _( "Jitter" ), _( "Rotate" ), _( "Spin" ), _( "Random" ) };

int selectedConfig{ };
std::string typedConfig{ };