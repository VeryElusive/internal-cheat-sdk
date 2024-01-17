#pragma once

#define SHOW( cond ) [ ]( ) { return cond; }

ComboElement_t WeaponTypes[ ]{ ( "None" ), ( "Icon" ), ( "Text" ) };

ComboElement_t pitch[ 5 ]{ ( "Default" ), ( "Up" ), ( "Down" ), ( "Zero" ), ( "Random" ) };
ComboElement_t yaw[ 4 ]{ ( "Default" ), ( "Backward" ), ( "Left" ), ( "Right" ) };
ComboElement_t yawAdd[ 5 ]{ ( "None" ), ( "Jitter" ), ( "Rotate" ), ( "Spin" ), ( "Random" ) };

int selectedConfig{ };
std::string typedConfig{ };