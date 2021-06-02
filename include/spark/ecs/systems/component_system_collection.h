
#ifndef SPARK_COMPONENT_SYSTEM_COLLECTION_H
#define SPARK_COMPONENT_SYSTEM_COLLECTION_H

#include <spark/spark_pch.h>
#include <spark/ecs/systems/base_component_system_interface.h>
#include <spark/ecs/systems/component_system_collection_interface.h>

namespace Spark {
    namespace ECS {
        namespace Systems {

            class ComponentSystemCollection : public IComponentSystemCollection {
                public:
                    ComponentSystemCollection();

                    void Initialize() override;
                    void Update(float dt) override;
                    void Shutdown() override;

                private:
                    std::vector<IBaseComponentSystem*> _systems;
            };

        }
    }
}

#endif //SPARK_COMPONENT_SYSTEM_COLLECTION_H
