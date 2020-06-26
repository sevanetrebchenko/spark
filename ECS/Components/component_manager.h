
#ifndef DATASTRUCTURES_COMPONENT_MANAGER_H
#define DATASTRUCTURES_COMPONENT_MANAGER_H

#include "../../Utilities/Memory/segmented_pool_allocator.h"
#include "base_component.h"

namespace ECS {
    template<class ComponentType>
    class ComponentManager final {
        public:
            ComponentManager();
            void Initialize();
            ~ComponentManager();

            _NODISCARD_ ComponentType* AddComponent();
            void DeleteComponent(ComponentType* component);

        private:
            UtilityBox::Memory::SegmentedPoolAllocator* _allocator;
    };
}


#endif //DATASTRUCTURES_COMPONENT_MANAGER_H
