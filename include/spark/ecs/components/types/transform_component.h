#ifndef SPARK_TRANSFORM_COMPONENT_H
#define SPARK_TRANSFORM_COMPONENT_H

#include <glm/glm.hpp>
#include <spark/ecs/components/types/base_component.h> // BaseComponent

namespace Spark {
    namespace ECS {
        namespace Components {

            struct TransformComponent : public BaseComponent {
                public:
                    static constexpr unsigned ID = STRINGHASH("TransformComponent");
                    static constexpr const char* Name = "Transform";

                    TransformComponent();
                    void OnImGuiRender() override;

                    _NODISCARD_ glm::mat4 GetModelMatrix();

                    void SetParentTransform(TransformComponent* parent);

                    _NODISCARD_ glm::vec3 GetLocalPosition() const;
                    _NODISCARD_ glm::vec3 GetPosition() const;

                    _NODISCARD_ glm::vec2 GetLocalScale() const;
                    _NODISCARD_ glm::vec2 GetScale() const;

                    _NODISCARD_ float GetLocalRotation() const;
                    _NODISCARD_ float GetRotation() const;

                private:
                    TransformComponent* _parentTransform;
                    bool _isDirty;

                    float _rotation; // Rotation stored in radians.
                    glm::vec2 _scale;
                    glm::vec3 _position;

                    glm::mat4 _modelMatrix;
            };

        }
    }
}

#endif //SPARK_TRANSFORM_COMPONENT_H
