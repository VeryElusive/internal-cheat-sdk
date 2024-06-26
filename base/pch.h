#pragma once
#ifndef PCH_H
#define PCH_H

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <d3d11.h>

#include "dependencies/imgui/imgui.h"
#include "dependencies/imgui/imgui_internal.h"
#include "dependencies/imgui/backends/imgui_impl_dx11.h"
#include "dependencies/imgui/backends/imgui_impl_win32.h"

#include "utils/xorstr.h"

#include <cstdint>
#include <vector>
#include <optional>
#include <string_view>
#include <Windows.h>
#include <algorithm>
#include <d3d11_1.h>

#endif //PCH_H