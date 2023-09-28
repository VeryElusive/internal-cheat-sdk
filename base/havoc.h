#pragma once
#include <cstdint>
#include <vector>
#include <optional>
#include <string_view>
#include <Windows.h>
#include <algorithm>

#include "core/config.h"
#include "core/memory.h"

#include "sdk/vector.h"
#include "sdk/player.h"

struct Context_t {
	HWND hwnd;
	float m_flElapsed{ };
	float m_flTime{ };
};

inline Context_t ctx;