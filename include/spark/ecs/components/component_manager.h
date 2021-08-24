
#ifndef SPARK_COMPONENT_MANAGER_H
#define SPARK_COMPONENT_MANAGER_H

#include "spark/utility.h"
#include "spark/ecs/components/component_manager_interface.h"
#include "spark/ecs/ecs_definitions.h"

namespace Spark {
    namespace ECS {

        template <class ComponentType>
        class ComponentManager : public IComponentManager {
            public:
                ComponentManager();
                ~ComponentManager() override;

                NODISCARD ComponentType* CreateComponent();
                void DeleteComponent(ComponentType *component);

            private:
                std::forward_list<ComponentType> components_;
        };

    }
}

#include "spark/ecs/components/component_manager.tpp"

#endif // SPARK_COMPONENT_MANAGER_H

