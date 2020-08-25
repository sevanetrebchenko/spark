
#ifndef SPARK_CORE_H
#define SPARK_CORE_H

#include <spark_pch.h>
#include <core/platform_detection.h>
#include <utilitybox/tools/global_defines.h> // _NODISCARD_, BIT_SHIFT

#ifndef NDEBUG
    #define SP_DEBUG
#endif

// Enable debug builds.
#ifdef SP_DEBUG
    // Enable debug breaks.
    #if defined(SP_PLATFORM_LINUX)
        #include <csignal>
        #define SP_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Debug breaks are not suppported on the current platform."
    #endif

    #define SP_ENABLE_ASSERTS
#else
    // Does nothing.
    #define SP_DEBUGBREAK()
#endif

// Enable asserts
#ifdef SP_ENABLE_ASSERTS
    #include <utilitybox/tools/assert_dev.h>
#else
    // Does nothing.
    #define SP_ASSERT(check, formatString, ...)
#endif

#endif // SPARK_CORE_H
