
#ifndef SPARK_COMPONENT_MANAGER_H
#define SPARK_COMPONENT_MANAGER_H

#include <spark/core/core.h>
#include <spark/ecs/components/component_manager_interface.h> // IComponentManager
#include <spark/utilitybox/memory/segmented_pool_allocator.h> // SegmentedPoolAllocator

namespace Spark {
    namespace ECS {
        namespace Components {

            template<class ComponentType>
            class ComponentManager : public IComponentManager, public UtilityBox::Logger::ILoggable {
                public:
                    ComponentManager();
                    ~ComponentManager() override;

                    _NODISCARD_ ComponentType* CreateComponent();
                    void DeleteComponent(ComponentType *component);

                private:
                    UtilityBox::Memory::SegmentedPoolAllocator _allocator { sizeof(ComponentType) }; // Memory allocator.
            };

        }
    }
}

#include <spark/ecs/components/component_manager.tpp> // Template function definitions.

#endif // SPARK_COMPONENT_MANAGER_H

