#pragma once

#include "System/Logger.hpp"

// Windows specific defines.
#if defined(WIN32) && !defined(NDEBUG)
    #define _CRTDBG_MAP_ALLOC
    #define _CRTDBG_MAP_ALLOC_NEW
    #include <stdlib.h>
    #include <crtdbg.h>

    // Override new operator to store additional info about allocations.
    #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #define new DEBUG_NEW
#else
    #define DEBUG_NEW new
#endif

// Initializes low level debug systems.
void initializeDebug();

// Debug macros.
#define DEBUG_EXPAND_MACRO(x) x
#define DEBUG_STRINGIFY(expression) #expression
#define DEBUG_PRINT(format, ...) LOG_FATAL(format, ## __VA_ARGS__)

#ifdef _MSC_VER
    #define DEBUG_BREAK() __debugbreak();
#else
    #include <sys/signal.h>
    #define DEBUG_BREAK() raise(SIGTRAP);
#endif

// Assert macros.
// Will break only in debug builds.
// Use as a sanity check wherever possible.
#ifndef NDEBUG
    #define ASSERT_SIMPLE(expression) \
        if(expression) { } else \
        { \
            DEBUG_PRINT("Assertion failed: " DEBUG_STRINGIFY(expression)); \
            DEBUG_BREAK(); \
        }

    #define ASSERT_MESSAGE(expression, message, ...) \
        if(expression) { } else \
        { \
            DEBUG_PRINT("Assertion failed: " DEBUG_STRINGIFY(expression) " - " message, ## __VA_ARGS__); \
            DEBUG_BREAK(); \
        }
#else
    #define ASSERT_SIMPLE(expression)
    #define ASSERT_MESSAGE(expression, message, ...)
#endif

#define ASSERT_DEDUCE(arg1, arg2, arg3, arg4, arg5, arg6, arg7, ...) arg7
#define ASSERT_CHOOSER(...) DEBUG_EXPAND_MACRO(ASSERT_DEDUCE(__VA_ARGS__, ASSERT_MESSAGE, ASSERT_MESSAGE, ASSERT_MESSAGE, ASSERT_MESSAGE, ASSERT_MESSAGE, ASSERT_SIMPLE))
#define ASSERT(...) DEBUG_EXPAND_MACRO(ASSERT_CHOOSER(__VA_ARGS__)(__VA_ARGS__))

// Verify macro.
// Will break in all build configurations.
// Use to safeguard against errors we do not or cannot handle.
#define VERIFY_SIMPLE(expression) \
    if(expression) { } else \
    { \
        DEBUG_PRINT("Verification failed: " DEBUG_STRINGIFY(expression)) \
        DEBUG_BREAK(); \
    }

#define VERIFY_MESSAGE(expression, message, ...) \
    if(expression) { } else \
    { \
        DEBUG_PRINT("Verification failed: " DEBUG_STRINGIFY(expression) " - " message, ## __VA_ARGS__) \
        DEBUG_BREAK(); \
    }

#define VERIFY_DEDUCE(arg1, arg2, arg3, arg4, arg5, arg6, arg7, ...) arg7
#define VERIFY_CHOOSER(...) DEBUG_EXPAND_MACRO(VERIFY_DEDUCE(__VA_ARGS__, VERIFY_MESSAGE, VERIFY_MESSAGE, VERIFY_MESSAGE, VERIFY_MESSAGE, VERIFY_MESSAGE, VERIFY_SIMPLE))
#define VERIFY(...) DEBUG_EXPAND_MACRO(VERIFY_CHOOSER(__VA_ARGS__)(__VA_ARGS__))
