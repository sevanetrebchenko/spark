
#include <glm/glm.hpp>
#include <spark/graphics/renderer/shader_data_type.h>

namespace Spark::Graphics {

    std::size_t ShaderDataTypeSize(ShaderDataType shaderDataType) {
        switch (shaderDataType) {
            case ShaderDataType::INT:
                return sizeof(int);
            case ShaderDataType::BOOL:
                return sizeof(bool);
            case ShaderDataType::FLOAT:
                return sizeof(float);
            case ShaderDataType::VEC2:
                return sizeof(glm::vec2);
            case ShaderDataType::VEC3:
                return sizeof(glm::vec3);
            case ShaderDataType::VEC4:
                return sizeof(glm::vec4);
            case ShaderDataType::MAT2:
                return sizeof(glm::mat2);
            case ShaderDataType::MAT3:
                return sizeof(glm::mat3);
            case ShaderDataType::MAT4:
                return sizeof(glm::mat4);
            default:
                SP_ASSERT(false, "ShaderDataType with ID: %i is not a valid shader data type.", shaderDataType);
                return 0;
        }
    }

}

