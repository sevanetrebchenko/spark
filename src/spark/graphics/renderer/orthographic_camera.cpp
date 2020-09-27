
#include <spark/graphics/renderer/orthographic_camera.h>
#include <glm/ext.hpp>

namespace Spark::Graphics {

    struct OrthographicCamera::OrthographicCameraData {

        void RecalculateCameraMatrix();


    };

    void OrthographicCamera::OrthographicCameraData::RecalculateCameraMatrix() {
        // position of the camera eye
        const glm::vec3& cameraEyePosition {0, 0, 0};
        // camera looks along the -z axis towards z = 0
        glm::vec3 cameraLookDirection = glm::vec3{ cameraEyePosition.x, cameraEyePosition.y, 0.0f };
        glm::vec3 cameraDirection = glm::normalize(cameraEyePosition - cameraLookDirection);

        // global up vector of the camera points along the y axis
        glm::vec3 globalUpVector = glm::vec3{ 0.0f, 1.0f, 0.0f };

        // camera right vector
        glm::vec3 cameraRight = glm::normalize(glm::cross(globalUpVector, cameraDirection));

        // camera up vector
        glm::vec cameraUp = glm::cross(cameraDirection, cameraRight);

        // view matrix of the camera
        // camera -> world transformation
        glm::mat4 viewMatrix = glm::mat4(
                glm::vec4 {cameraRight, 0.0f},
                glm::vec4 {cameraUp, 0.0f},
                glm::vec4 {cameraDirection, 0.0f},
                glm::vec4 {cameraEyePosition, 1.0f}
        );

        // invert the matrix to get the world to camera transformation
        // world -> camera
        viewMatrix = glm::inverse(viewMatrix);

        // camera -> NDC matrix
        glm::mat4 cameraMatrix = glm::ortho(
                this->currentCameraDimensions.x / 2,
                -this->currentCameraDimensions.x / 2,
                -this->currentCameraDimensions.y / 2,
                this->currentCameraDimensions.y / 2,
                0.0f, 1000.0f
        );
    }

    OrthographicCamera::OrthographicCamera(float aspectRatio) : _data(new OrthographicCameraData()) {
        // Nothing to do here.
    }

    glm::mat4 OrthographicCamera::GetCameraMatrix() const {
        if (_data->_isDirty) {
            _data->RecalculateCameraMatrix();
        }

        return _data->_projectionMatrix * _data->_viewMatrix;
    }
}