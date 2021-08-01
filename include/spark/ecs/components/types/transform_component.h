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

                    NODISCARD glm::mat4 GetModelMatrix();

                    void SetParentTransform(TransformComponent* parent);

                    NODISCARD glm::vec3 GetLocalPosition() const;
                    NODISCARD glm::vec3 GetPosition() const;

                    NODISCARD glm::vec2 GetLocalScale() const;
                    NODISCARD glm::vec2 GetScale() const;

                    NODISCARD float GetLocalRotation() const;
                    NODISCARD float GetRotation() const;

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
