
#ifndef SPARK_CONFIGURATION_H
#define SPARK_CONFIGURATION_H

//// Platform detection.
//// Windows.
//#ifdef _WIN32
//    #ifdef _WIN64
//        /* Windows x64 */
//        #define SP_PLATFORM_WINDOWS
//    #else
//        /* Windows x86 */
//        #error "Spark is not currently supported on Windows x86 builds."
//    #endif
//
//// Apple / MacOS
//#elif defined(__APPLE__) || defined(__MACH__)
//    #include <TargetConditionals.h>
//	// TARGET_OS_MAC exists on all the Apple platforms. They need to be checked in order to ensure Spark is running
//	// on a Mac and not a different Apple platform.
//	#if TARGET_IPHONE_SIMULATOR == 1
//		#error "Spark is not supported on IOS Simulator."
//	#elif TARGET_OS_IPHONE == 1
//		#define SP_PLATFORM_IOS
//		#error "Spark is not supported on IOS."
//	#elif TARGET_OS_MAC == 1
//		#define SP_PLATFORM_MACOS
//		#error "Spark is not currently supported on MacOS."
//	#else
//		#error "Unknown Apple platform."
//    #endif
//
//// Android.
//// Android platforms have Linux defined, so they must be checked before.
//#elif defined(__ANDROID__)
//    #define SP_PLATFORM_ANDROID
//        #error "Spark is not currently supported on MacOS."
//
//// Linux.
//#elif defined(__linux__)
//    #define SP_PLATFORM_LINUX
//
//// Unknown platform.
//#else
//	#error "Unknown platform!"
//#endif // End of platform detection.

#endif // SPARK_CONFIGURATION_H
