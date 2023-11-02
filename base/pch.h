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
#pragma comment(lib, "d3d11.lib")

#include "dependencies/imgui/imgui.h"
#include "dependencies/imgui/imgui_internal.h"
#include "dependencies/imgui/backends/imgui_impl_dx11.h"
#include "dependencies/imgui/backends/imgui_impl_win32.h"

#include "utils/xorstr.h"

#endif //PCH_H