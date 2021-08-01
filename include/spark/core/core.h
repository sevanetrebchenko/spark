
#ifndef SPARK_CORE_H
#define SPARK_CORE_H

#include <spark/ecs/entities/entity_manager.h>
#include <spark/ecs/systems/base_component_system_interface.h>

namespace Spark {

    class Core {
        public:
            Core();
            ~Core();

            void RegisterSystem()

        private:
            std::vector<ECS::IBaseComponentSystem*> componentSystems_;
            ECS::EntityManager* entityManager_;
    };

}

#endif //SPARK_CORE_H
