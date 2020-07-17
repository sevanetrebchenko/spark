
#ifndef SPARK_COMPILE_TIME_HASH_H
#define SPARK_COMPILE_TIME_HASH_H

#include "../../spark_pch.h" // std::uint32_t

namespace Spark {
    namespace UtilityBox {
        namespace Hash {
            /**
             * FNV-1a 32-bit hashing algorithm.
             * @param string       - String to hash.
             * @param currentIndex - Current character index of the string while hashing.
             * @return
             */
            constexpr inline std::uint32_t FNV1a_32bit(const char *string, std::size_t currentIndex) {
                if (currentIndex) {
                    return (FNV1a_32bit(string, currentIndex - 1u) ^ static_cast<unsigned char>(string[currentIndex])) * 16777619u;
                }
                else {
                    return (2166136261u ^ static_cast<unsigned char>(string[currentIndex])) * 16777619u;
                }
            }

        } // namespace Hash
    } // namespace UtilityBox
} // namespace Spark

#define STRINGHASH(string) (Spark::UtilityBox::Hash::FNV1a_32bit(string, sizeof(string) - 1))

#endif // SPARK_COMPILE_TIME_HASH_H
