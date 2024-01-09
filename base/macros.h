#pragma once

#ifdef _DEBUG
#define ASSERT(EXPRESSION) static_cast<void>(!!(EXPRESSION) || (__debugbreak(), 0))
#else
#define ASSERT(EXPRESSION) static_cast<void>(0)
#endif

#ifdef _DEBUG
#define CHECK_AND_FAIL( check ) if ( !check ) { printf( #check " failed!\n"); return false; }
#define ENTER_REGION( region ) printf( "\n\n-------------------------------------\n\t\t" region "\n\n");
#else
#define CHECK_AND_FAIL( check ) if ( !check ) return false;
#define ENTER_REGION( region )
#endif

#define CONCAT3(a, b, c) a ## b ## c