
#ifndef SPARK_CORE_H
#define SPARK_CORE_H

#include "ecs/entities/entity_manager.h"
#include "ecs/systems/base_component_system_interface.h"
#include <spark_pch.h> // vector

namespace Spark {
    class Core {
        public:
            /**
             * Returns singleton instance of the engine Core. Instantiates singleton if necessary.
             * @return Pointer to singleton Core instance.
             */
            static Core* GetInstance();

            void Initialize();
            void Update();
            void Shutdown();

            Spark::ECS::Entities::EntityManager* GetEntityManager();

        private:
            Core();
            ~Core();

            static Core* _world; // Singleton

            class CoreData;
            CoreData* _data;
    };
}


#endif // SPARK_CORE_H
