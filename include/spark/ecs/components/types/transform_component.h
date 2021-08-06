
#ifndef SPARK_TRANSFORM_COMPONENT_H
#define SPARK_TRANSFORM_COMPONENT_H

#include "spark/core/utility.h"
#include "spark/ecs/components/types/base_component.h"

namespace Spark {
    namespace ECS {

        struct Transform : public BaseComponent {
            ~Transform() override;
            void OnImGuiRender() override;

            bool _enabled = true;

            REGISTER_COMPONENT(Transform)
        };

    }
}

#endif //SPARK_TRANSFORM_COMPONENT_H
