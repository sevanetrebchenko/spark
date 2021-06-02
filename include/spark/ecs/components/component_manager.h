
#ifndef SPARK_COMPONENT_MANAGER_H
#define SPARK_COMPONENT_MANAGER_H

#include <spark/core/core.h>
#include <spark/ecs/components/component_manager_interface.h> // ComponentManagerInterface

namespace Spark {
    namespace ECS {
        namespace Components {

            template<class ComponentType>
            class ComponentManager : public ComponentManagerInterface {
                public:
                    ComponentManager();
                    ~ComponentManager() override;

                    _NODISCARD_ ComponentType* CreateComponent();
                    void DeleteComponent(ComponentType *component);

                private:
                    // Storage for ComponentManager data, back-end functionality, and helper functions.
                    class ComponentManagerData;
                    ComponentManagerData *_data;
            };

        }
    }
}

#include <spark/ecs/components/component_manager.tpp> // Template function definitions.

#endif // SPARK_COMPONENT_MANAGER_H

