#pragma once

#define SHOW( cond ) [ ]( ) { return cond; }

ComboElement_t WeaponTypes[ ]{ ( "None" ), ( "Icon" ), ( "Text" ) };

int selectedConfig{ };
std::string typedConfig{ };