#include "Precompiled.hpp"
#include "Common/Debug.hpp"

void initializeDebug()
{
    // Enable debug memory allocator and memory leak detection.
    #if defined(WIN32) && !defined(NDEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    #endif
}
