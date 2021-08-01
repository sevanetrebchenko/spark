
#ifndef SPARK_UTILITY_H
#define SPARK_UTILITY_H

namespace Spark {
    namespace Internal {

        // Generates comma separated list of given components without trailing comma.
        template <typename ComponentType>
        std::string CommaSeparatedList() {
            return std::string(ComponentType::Name);
        }

        template <typename ComponentType1, typename ComponentType2, typename... AdditionalComponentTypes>
        std::string CommaSeparatedList() {
            return CommaSeparatedList<ComponentType1>() + ", " + CommaSeparatedList<ComponentType2, AdditionalComponentTypes...>();
        }

    }
}

#endif //SPARK_UTILITY_H
