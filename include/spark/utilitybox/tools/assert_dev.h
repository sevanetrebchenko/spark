
#ifndef SPARK_ASSERT_DEV_H
#define SPARK_ASSERT_DEV_H

#include "spark/utilitybox/utility.h"

// Function used when more parameters are passed with the message (printf-style assert).
#define SP_ASSERT_VARIADIC(function, file, line, check, formatString, ...)                                                                           \
    do {                                                                                                                                             \
        if (!(check)) {                                                                                                                              \
            std::cerr << "Assert of condition: " << #check << " failed in function: " << (function) << ", " << (file) << ":" << (line) << std::endl; \
            std::cerr << "Assertion message: ";                                                                                                      \
            fprintf(stderr, (formatString), ##__VA_ARGS__);                                                                                          \
            std::cerr << std::endl;                                                                                                                  \
            std::cerr << "Aborting program execution." << std::endl;                                                                                 \
            exit(1);                                                                                                                                 \
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
            exit(1);                                                                                                                                 \
        }                                                                                                                                            \
    }                                                                                                                                                \
    while (false)                                                                                                                                    \

// Supports up to 256 variadic arguments.
#define ARG_TABLE( arg1,   arg2,   arg3,   arg4,   arg5,   arg6,   arg7,   arg8,   arg9,  arg10,  arg11,  arg12,  arg13,  arg14, \
                  arg15,  arg16,  arg17,  arg18,  arg19,  arg20,  arg21,  arg22,  arg23,  arg24,  arg25,  arg26,  arg27,  arg28, \
		          arg29,  arg30,  arg31,  arg32,  arg33,  arg34,  arg35,  arg36,  arg37,  arg38,  arg39,  arg40,  arg41,  arg42, \
		          arg43,  arg44,  arg45,  arg46,  arg47,  arg48,  arg49,  arg50,  arg51,  arg52,  arg53,  arg54,  arg55,  arg56, \
		          arg57,  arg58,  arg59,  arg60,  arg61,  arg62,  arg63,  arg64,  arg65,  arg66,  arg67,  arg68,  arg69,  arg70, \
		          arg71,  arg72,  arg73,  arg74,  arg75,  arg76,  arg77,  arg78,  arg79,  arg80,  arg81,  arg82,  arg83,  arg84, \
		          arg85,  arg86,  arg87,  arg88,  arg89,  arg90,  arg91,  arg92,  arg93,  arg94,  arg95,  arg96,  arg97,  arg98, \
		          arg99, arg100, arg101, arg102, arg103, arg104, arg105, arg106, arg107, arg108, arg109, arg110, arg111, arg112, \
		         arg113, arg114, arg115, arg116, arg117, arg118, arg119, arg120, arg121, arg122, arg123, arg124, arg125, arg126, \
		         arg127, arg128, arg129, arg130, arg131, arg132, arg133, arg134, arg135, arg136, arg137, arg138, arg139, arg140, \
		         arg141, arg142, arg143, arg144, arg145, arg146, arg147, arg148, arg149, arg150, arg151, arg152, arg153, arg154, \
		         arg155, arg156, arg157, arg158, arg159, arg160, arg161, arg162, arg163, arg164, arg165, arg166, arg167, arg168, \
		         arg169, arg170, arg171, arg172, arg173, arg174, arg175, arg176, arg177, arg178, arg179, arg180, arg181, arg182, \
		         arg183, arg184, arg185, arg186, arg187, arg188, arg189, arg190, arg191, arg192, arg193, arg194, arg195, arg196, \
		         arg197, arg198, arg199, arg200, arg201, arg202, arg203, arg204, arg205, arg206, arg207, arg208, arg209, arg210, \
		         arg211, arg212, arg213, arg214, arg215, arg216, arg217, arg218, arg219, arg220, arg221, arg222, arg223, arg224, \
		         arg225, arg226, arg227, arg228, arg229, arg230, arg231, arg232, arg233, arg234, arg235, arg236, arg237, arg238, \
		         arg239, arg240, arg241, arg242, arg243, arg244, arg245, arg246, arg247, arg248, arg249, arg250, arg251, arg252, \
		         arg253, arg254, arg255, arg256, arg257, RESULT, ...) RESULT

#define ARG_NUMBERS() 256, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, \
                      235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, \
                      214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, \
                      193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, \
                      172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, \
                      151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, \
                      130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, \
                      109, 108, 107, 106, 105, 104, 103, 102, 101, 100,  99,  98,  97,  96,  95,  94,  93,  92,  91,  90,  89, \
                       88,  87,  86,  85,  84,  83,  82,  81,  80,  79,  78,  77,  76,  75,  74,  73,  72,  71,  70,  69,  68, \
                       67,  66,  65,  64,  63,  62,  61,  60,  59,  58,  57,  56,  55,  54,  53,  52,  51,  50,  49,  48,  47, \
                       46,  45,  44,  43,  42,  41,  40,  39,  38,  37,  36,  35,  34,  33,  32,  31,  30,  29,  28,  27,  26, \
                       25,  24,  23,  22,  21,  20,  19,  18,  17,  16,  15,  14,  13,  12,  11,  10,   9,   8,   7,   6,   5, \
                        4,   3,   2,   1,   0

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

#define NUM_ARGS(...) ARG_TABLE(__VA_ARGS__)
#define GET_NUM_ARGS_ELEMENT_FROM_TABLE(TABLE, ...) NUM_ARGS(_, ##__VA_ARGS__, TABLE())

// Generate macro decoration based on the number of parameters.
#define APPEND_SUFFIX(SUFFIX) SP_ASSERT##_##SUFFIX
#define GET_FUNCTION_SIGNATURE_HELPER(ELEMENT) APPEND_SUFFIX(ELEMENT)
#define GET_FUNCTION_SIGNATURE(function, file, line, check, formatString, ...) GET_FUNCTION_SIGNATURE_HELPER(GET_NUM_ARGS_ELEMENT_FROM_TABLE(MACRO_DECORATION, ##__VA_ARGS__))(function, file, line, check, formatString, ##__VA_ARGS__)

// Definition for ASSERT
#define SP_ASSERT(check, formatString, ...) GET_FUNCTION_SIGNATURE(__PRETTY_FUNCTION__, __FILENAME__, __LINE__, check, formatString, ##__VA_ARGS__)

#endif // SPARK_ASSERT_DEV_H
