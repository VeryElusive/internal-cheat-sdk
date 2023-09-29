#pragma once
#include <cstdint>
#include <vector>
#include <optional>
#include <string_view>
#include <Windows.h>
#include <algorithm>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>

#include "core/config.h"
#include "core/memory.h"

#include "sdk/vector.h"
#include "sdk/player.h"

struct Context_t {
};

inline Context_t ctx;