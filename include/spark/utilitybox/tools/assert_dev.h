
#ifndef SPARK_ASSERT_DEV_H
#define SPARK_ASSERT_DEV_H

#include <spark/core/utility.h>

// Function used when more parameters are passed with the message (printf-style assert).
#define SP_ASSERT_VARIADIC(function, file, line, check, formatString, ...)                                                                           \
    do {                                                                                                                                             \
        if (!(check)) {                                                                                                                              \
            std::cerr << "Assert of condition: " << #check << " failed in function: " << (function) << ", " << (file) << ":" << (line) << std::endl; \
            std::cerr << "Assertion message: ";                                                                                                      \
            fprintf(stderr, (formatString), ##__VA_ARGS__);                                                                                          \
            std::cerr << std::endl;                                                                                                                  \
            std::cerr << "Aborting program execution." << std::endl;                                                                                 \
            SP_DEBUGBREAK();                                                                                                                         \
    }                                                                                                                                                \
    }                                                                                                                                                \
    while (false)

// Function used when only a message (string) is passed with the assert.
#define SP_ASSERT_FIXED(function, file, line, check, formatString)                                                                                   \
    do {                                                                                                                                             \
        if (!(check)) {                                                                                                                              \
            std::cerr << "Assert of condition: " << #check << " failed in function: " << (function) << ", " << (file) << ":" << (line) << std::endl; \
            std::cerr << "Assertion message: " << (formatString);                                                                                    \
            std::cerr << std::endl;                                                                                                                  \
            std::cerr << "Aborting program execution." << std::endl;                                                                                 \
            SP_DEBUGBREAK();                                                                                                                         \
        }                                                                                                                                            \
    }                                                                                                                                                \
    while (false)                                                                                                                                    \

// Call ASSERT_FIXED macro for when 0 arguments are passed, ASSERT_VARIADIC for argument numbers greater than 0.
#define MACRO_DECORATION() VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, \
                           VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, VARIADIC, FIXED

// Generate macro decoration based on the number of parameters.
#define APPEND_SUFFIX(SUFFIX) SP_ASSERT##_##SUFFIX
#define GET_FUNCTION_SIGNATURE_HELPER(ELEMENT) APPEND_SUFFIX(ELEMENT)
#define GET_FUNCTION_SIGNATURE(function, file, line, check, formatString, ...) GET_FUNCTION_SIGNATURE_HELPER(GET_NUM_ARGS_ELEMENT_FROM_TABLE(MACRO_DECORATION, ##__VA_ARGS__))(function, file, line, check, formatString, ##__VA_ARGS__)

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// Definition for ASSERT
#define SP_ASSERT(check, formatString, ...) GET_FUNCTION_SIGNATURE(__PRETTY_FUNCTION__, __FILENAME__, __LINE__, check, formatString, ##__VA_ARGS__)

#endif // SPARK_ASSERT_DEV_H
