#pragma once

// Windows specific defines.
#if defined(WIN32) && !defined(NDEBUG)
    #define _CRTDBG_MAP_ALLOC
    #define _CRTDBG_MAP_ALLOC_NEW
    #include <stdlib.h>
    #include <crtdbg.h>

    // Override new operator to store additional info about allocations.
    #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// Initializes low level debug systems.
void initializeDebug();
