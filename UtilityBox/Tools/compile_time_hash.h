
#ifndef DATASTRUCTURES_COMPILE_TIME_HASH_H
#define DATASTRUCTURES_COMPILE_TIME_HASH_H

#include <cstdint>

#define STRINGHASH(string) (UtilityBox::Hash::operator""_hash(string, sizeof(string) - 1))

namespace UtilityBox::Hash {
    // FNV-1a 32-bit hashing algorithm.
    constexpr inline std::uint32_t FNV1a_32bit(const char* string, std::size_t currentIndex) {
        if (currentIndex) {
            return (FNV1a_32bit(string, currentIndex - 1) ^ string[currentIndex]) * 16777619u;
        }
        else {
            return (2166136261u ^ string[currentIndex]) * 16777619u;
        }
    }

    constexpr inline std::uint32_t operator""_hash(const char* string, std::size_t stringLength) {
        return FNV1a_32bit(string, stringLength);
    }
}

#endif //DATASTRUCTURES_COMPILE_TIME_HASH_H
