
#ifndef SPARK_TRANSFORM_H
#define SPARK_TRANSFORM_H

#include "spark/core/utility.h"
#include "spark/ecs/components/types/base_component.h"
#include "spark/ecs/utility.h"

namespace Spark {
    namespace ECS {

        struct Transform : public IComponent {
            Transform() = default;
            ~Transform() override;
            void OnImGuiRender() override;

            bool _enabled = true;

            REGISTER_COMPONENT(Transform);
        };

    }
}

#endif //SPARK_TRANSFORM_H
