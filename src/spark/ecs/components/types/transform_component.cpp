
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <spark/ecs/components/types/transform_component.h>

namespace Spark::ECS::Components {

    glm::mat4 TransformComponent::GetModelMatrix() {
        if (_isDirty) {
            if (_parentTransform) {
                _modelMatrix =
                _modelMatrix = glm::translate(glm::mat4(1.0f), _parentTransform->GetPosition());
                _modelMatrix = glm::rotate(_modelMatrix, _parentTransform->GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
            }

            _modelMatrix = glm::translate(_modelMatrix, _position);
            _modelMatrix = glm::rotate(_modelMatrix, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
            _modelMatrix = glm::scale(_modelMatrix, glm::vec3 {_scale, 1.0f});
        }

        return _modelMatrix;
    }
}
