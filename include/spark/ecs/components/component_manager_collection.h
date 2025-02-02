
#ifndef SPARK_COMPONENT_MANAGER_COLLECTION_H
#define SPARK_COMPONENT_MANAGER_COLLECTION_H

#include "spark/utility.h"
#include "spark/ecs/components/component_manager.h"
#include "spark/ecs/ecs_definitions.h"
#include "spark/tools/singleton.h"

namespace Spark {
    namespace ECS {

        template <class ...ComponentTypes>
        class ComponentManagerCollection : public Singleton<ComponentManagerCollection<ComponentTypes...>> {
            public:
                REGISTER_SINGLETON(ComponentManagerCollection<ComponentTypes...>);

                template <class ComponentType>
                NODISCARD ComponentManager<ComponentType>* GetComponentManager();

            protected:
                ComponentManagerCollection();
                ~ComponentManagerCollection() override;

            private:
                template <class ComponentType>
                void CreateComponentSystem();

                template <class ComponentType>
                void DestroyComponentSystem();

                std::unordered_map<ComponentTypeID, IComponentManager*> componentManagerMap_;
        };

    }
}

#include "spark/ecs/components/component_manager_collection.tpp" // Template function definitions.

#endif // SPARK_COMPONENT_MANAGER_COLLECTION_H

