
#ifndef DATASTRUCTURES_COMPONENT_MANAGER_H
#define DATASTRUCTURES_COMPONENT_MANAGER_H

#include "../../../Utilities/Memory/segmented_pool_allocator.h"
#include "base_component.h"
#include "../../../Utilities/Logger/logging_system.h"
#include "component_manager_interface.h"

namespace ECS::Components {
    template <class ComponentType>
    class ComponentManager : public ComponentManagerInterface {
        public:
            static std::size_t Size();

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
