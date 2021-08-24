
#ifndef SPARK_ECS_DEFINITIONS_H
#define SPARK_ECS_DEFINITIONS_H

#include "spark/utility.h"

namespace Spark {
    namespace ECS {

        class IComponent; // Forward declaration.

        typedef std::uint32_t ComponentTypeID;
        typedef std::uint32_t EntityID;
        typedef std::unordered_map<ComponentTypeID, IComponent*> EntityComponentMap;

        #define INVALID_ID (EntityID)-1;
        #define REGISTER_COMPONENT(ClassName)                                           \
        public:                                                                         \
            static constexpr ::Spark::ECS::ComponentTypeID ID = STRINGHASH(#ClassName); \
            static constexpr const char* Name = #ClassName;                             \
        private:

    }
}

#endif //SPARK_ECS_DEFINITIONS_H
