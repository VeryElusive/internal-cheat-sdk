#pragma once
#include "pch.h"

#include "utils/math.h"

#include "core/interfaces.h"

#include "core/config.h"
#include "core/memory.h"

#include "sdk/vector.h"
#include "sdk/color.h"


struct Context_t {
	float m_flFrameTime{ };
};

inline Context_t ctx;