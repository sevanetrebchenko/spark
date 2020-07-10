
#ifndef DATASTRUCTURES_COMPONENT_MANAGER_H
#define DATASTRUCTURES_COMPONENT_MANAGER_H

#include "../../../Utilities/Memory/segmented_pool_allocator.h"
#include "base_component.h"
#include "../../../Utilities/Logger/logging_system.h"

namespace ECS::Components {

    // Virtual abstract interface so that component managers can be polymorphic at runtime.
    class ComponentManagerInterface {
        public:
            virtual ~ComponentManagerInterface() = default;
    };

    template <class ComponentType>
    class ComponentManager : public ComponentManagerInterface {
        public:
            ComponentManager();
            void Initialize();

            ~ComponentManager() override;

            _NODISCARD_ ComponentType* CreateComponent();
            void DeleteComponent(ComponentType* component);

        private:
            class ComponentManagerData;
            ComponentManagerData* _data;
    };
}

#endif //DATASTRUCTURES_COMPONENT_MANAGER_H

#include "../src/component_manager.tpp"
