
#ifndef SPARK_ORTHOGRAPHIC_CAMERA_H
#define SPARK_ORTHOGRAPHIC_CAMERA_H

#include <glm/glm.hpp>
#include <spark/core/rename.h>

namespace Spark {
    namespace Graphics {

        class OrthographicCamera {
            public:
                explicit OrthographicCamera(float aspectRatio);
                ~OrthographicCamera();

                NODISCARD glm::mat4 GetCameraMatrix() const;

            private:
                class OrthographicCameraData;
                OrthographicCameraData* _data;
        };

    }
}

#endif //SPARK_ORTHOGRAPHIC_CAMERA_H
