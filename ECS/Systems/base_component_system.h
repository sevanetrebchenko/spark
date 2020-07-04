
#ifndef DATASTRUCTURES_BASE_COMPONENT_SYSTEM_H
#define DATASTRUCTURES_BASE_COMPONENT_SYSTEM_H

#include <tuple>
#include <vector>
#include "../../Utilities/Memory/segmented_pool_allocator.h"
#include "base_component_system_interface.h"

namespace ECS::Systems {
    template <class ...ComponentTypes>
    class BaseComponentSystem : public BaseComponentSystemInterface {
        public:
            explicit BaseComponentSystem(std::string&& systemName);
            ~BaseComponentSystem() = default;

            void Initialize() override;
            void Update(float dt) override;
            void Shutdown() override;

        protected:
            using BaseSystem = BaseComponentSystem<ComponentTypes...>;
            using ComponentTuple = std::tuple<ComponentTypes*...>; // Tuple of component pointers.

            template <typename ComponentType>
            ComponentType* GetComponent(unsigned index);

            template <typename ComponentType>
            ComponentType* GetComponent(const ComponentTuple& componentTuple);

            std::string _systemName;
            UtilityBox::Logger::LoggingSystem _loggingSystem { std::move(std::string(_systemName)) };
            std::vector<ComponentTuple> _filteredEntities;

        private:
            class BaseComponentSystemData;
            BaseComponentSystemData* _data = nullptr;
    };
}

#endif //DATASTRUCTURES_BASE_COMPONENT_SYSTEM_H

#include "base_component_system.tpp"
