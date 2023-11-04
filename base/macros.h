#pragma once

#ifdef _DEBUG
#define ASSERT(EXPRESSION) static_cast<void>(!!(EXPRESSION) || (__debugbreak(), 0))
#else
#define ASSERT(EXPRESSION) static_cast<void>(0)
#endif