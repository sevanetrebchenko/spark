
#ifndef SPARK_ECS_UTILITY_TPP
#define SPARK_ECS_UTILITY_TPP

namespace Spark::ECS::Internal {

    template <typename ComponentType>
    std::string CommaSeparatedList() {
        return std::string(ComponentType::Name);
    }

    template <typename ComponentType1, typename ComponentType2, typename... AdditionalComponentTypes>
    std::string CommaSeparatedList() {
        return CommaSeparatedList<ComponentType1>() + ", " + CommaSeparatedList<ComponentType2, AdditionalComponentTypes...>();
    }

}

#endif // SPARK_ECS_UTILITY_TPP