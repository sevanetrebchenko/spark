
#include <spark/utility.h>

namespace Spark {
    namespace ECS {

        EntityID GetEntityIDFromName(const std::string &entityName) {
            return STRINGHASH(entityName.c_str());
        }

    }

    namespace Internal {

        void ConvertToLowercase(std::string& string) {
            // Character by character traversal + conversion.
            for (char& character : string) {
                character = static_cast<char>(tolower(character));
            }
        }

        bool StringCompare(const std::string &first, const std::string &second) {
            return std::equal(first.begin(), first.end(), second.begin(), second.end(), [](char one, char two) {
                return tolower(static_cast<unsigned char>(one)) == tolower(static_cast<unsigned char>(two));
            });
        }

    }
}
