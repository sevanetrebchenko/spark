
#include <spark/ecs/utility.h>

namespace Spark::ECS {

    bool StringCompare(const std::string &first, const std::string &second) {
        return std::equal(first.begin(), first.end(), second.begin(), second.end(), [](char one, char two) {
            return tolower(one) == tolower(two);
        });
    }

    EntityID GetEntityIDFromName(const std::string &entityName) {
        return STRINGHASH(entityName.c_str());
    }

}