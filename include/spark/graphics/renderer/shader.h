
#ifndef SPARK_SHADER_H
#define SPARK_SHADER_H

#include <spark/core/rename.h>
#include <glm/glm.hpp>

namespace Spark {
    namespace Graphics {

        class IShader {
            public:
                static IShader* Create(const std::string& shaderName, const std::string& shaderSource);
                static IShader* Create(const std::string& shaderName, const std::string& vertexSource, const std::string& fragmentSource);
                virtual ~IShader() = default;

                virtual void Bind() const = 0;
                virtual void Unbind() const = 0;

                virtual void SetInt(const std::string& shaderName, int value) = 0;
                virtual void SetFloat(const std::string& shaderName, float value) = 0;
                virtual void SetVec2(const std::string& shaderName, const glm::vec2& vector) = 0;
                virtual void SetVec3(const std::string& shaderName, const glm::vec3& vector) = 0;
                virtual void SetVec4(const std::string& shaderName, const glm::vec4& vector) = 0;
                virtual void SetMat3(const std::string& shaderName, const glm::mat3& matrix) = 0;
                virtual void SetMat4(const std::string& shaderName, const glm::mat4& matrix) = 0;

                NODISCARD virtual const std::string& GetName() const = 0;
        };

    }
}

#endif //SPARK_SHADER_H
