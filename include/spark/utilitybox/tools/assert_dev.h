
#ifndef SPARK_ASSERT_DEV_H
#define SPARK_ASSERT_DEV_H

#include <spark_pch.h> // std::cerr, fprintf

// Function used when more parameters are passed with the message (printf-style assert).
#define ASSERT_VARIADIC(function, file, line, check, formatString, ...)                                                                        \
    do {                                                                                                                                       \
        if (!(check)) {                                                                                                                        \
            std::cerr << "Assert of condition: " << #check << " failed in function: " << function << ", " << file << ":" << line << std::endl; \
            std::cerr << "Assertion message: ";                                                                                                \
            fprintf(stderr, formatString, ##__VA_ARGS__);                                                                                      \
            std::cerr << std::endl;                                                                                                            \
            std::cerr << "Aborting program execution." << std::endl;                                                                           \
            exit(0);                                                                                                                           \
        }                                                                                                                                      \
    }                                                                                                                                          \
    while (false)                                                                                                                              \

// Function used when only a message (string) is passed with the assert.
#define ASSERT_FIXED(function, file, line, check, formatString)                                                                                \
    do {                                                                                                                                       \
        if (!(check)) {                                                                                                                        \
            std::cerr << "Assert of condition: " << #check << " failed in function: " << function << ", " << file << ":" << line << std::endl; \
            std::cerr << "Assertion message: " << formatString;                                                                                \
            std::cerr << std::endl;                                                                                                            \
            std::cerr << "Aborting program execution." << std::endl;                                                                           \
            exit(0);                                                                                                                           \
        }                                                                                                                                      \
    }                                                                                                                                          \
    while (false)                                                                                                                              \

// The argument look-up table returns the element in MACRO_DECORATION that falls in the RESULT position.
#define ARG_TABLE(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10,         \
                 arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, \
                 arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, \
                 arg31, arg32, arg33, arg34, arg35, arg36, arg37, arg38, arg39, arg40, \
                 arg41, arg42, arg43, arg44, arg45, arg46, arg47, arg48, arg49, arg50, \
                 arg51, arg52, arg53, arg54, arg55, arg56, arg57, arg58, arg59, arg60, \
                 arg61, arg62, arg63, RESULT, ...) RESULT                              \


// Template for decoration. Numbers represent how many arguments are passed into the function.
// Useful if you want to call different functions for specific numbers of arguments.
// Note: There are 64 arguments in the loop-up table (above) and 63 positions below - this is to
//       correctly account for 0 arguments.
//#define MACRO_DECORATION()                                 \
//         62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51,   \
//         50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39,   \
//         38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27,   \
//         26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15,   \
//         14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,   \
//         2,  1,  0                                         \

// Call ASSERT_FIXED macro for when 0 arguments are passed, ASSERT_VARIADIC for argument numbers greater than 0.
#define MACRO_DECORATION()                                                                                                         \
         VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC,   \
         VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC,   \
         VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC,   \
         VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC,   \
         VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC,   \
         VARIADIC, VARIADIC, FIXED                                                                                                 \

// Get the number of passed arguments from the look-up table.
#define NUM_ARGS(...) ARG_TABLE(__VA_ARGS__)

// Wrapper function to combine the above look-up tables. __VA_ARGS__ is offset by 1 so that 0 arguments are passed correctly.
#define GET_NUM_ARGS(...) NUM_ARGS(_, ##__VA_ARGS__, MACRO_DECORATION())

// Generate macro decoration based on the number of parameters.
#define _APPEND_SUFFIX(_FUNCTION_BASE, _SUFFIX) _FUNCTION_BASE##_##_SUFFIX
#define _ASSERT_TYPE(_FUNCTION_BASE, _SUFFIX) _APPEND_SUFFIX(_FUNCTION_BASE, _SUFFIX)
#define _GET_FUNCTION_SIGNATURE(_FUNCTION_BASE, function, file, line, check, formatString, ...) _ASSERT_TYPE(_FUNCTION_BASE, GET_NUM_ARGS(__VA_ARGS__))(function, file, line, check, formatString, ##__VA_ARGS__)

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// Definition for ASSERT
#define ASSERT(check, formatString, ...) _GET_FUNCTION_SIGNATURE(ASSERT, __PRETTY_FUNCTION__, __FILENAME__, __LINE__, check, formatString, ##__VA_ARGS__)


#endif // SPARK_ASSERT_DEV_H
