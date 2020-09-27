
#ifndef SPARK_ORTHOGRAPHIC_CAMERA_COMPONENT_H
#define SPARK_ORTHOGRAPHIC_CAMERA_COMPONENT_H

#include <spark/ecs/components/types/base_component.h>
#include <glm/glm.hpp>

namespace Spark {
    namespace Graphics {

        class OrthographicCamera : public ECS::Components::BaseComponent {
            public:
                OrthographicCamera();
                ~OrthographicCamera();

            private:
                bool _isDirty;

                glm::mat4 _projectionMatrix;
                glm::mat4 _viewMatrix;
        };

    }
}

#endif //SPARK_ORTHOGRAPHIC_CAMERA_COMPONENT_H
