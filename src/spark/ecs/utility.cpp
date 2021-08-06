
#include <spark/ecs/utility.h>

namespace Spark::ECS {

    EntityID GetEntityIDFromName(const std::string &entityName) {
        return STRINGHASH(entityName.c_str());
    }

}