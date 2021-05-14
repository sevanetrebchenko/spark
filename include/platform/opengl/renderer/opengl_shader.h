
#ifndef SPARK_OPENGL_SHADER_H
#define SPARK_OPENGL_SHADER_H

#include <spark/graphics/renderer/shader.h>

namespace Spark {
    namespace Graphics {

        class OpenGLShader : public IShader {
            public:
                /**
                 * Compile a shader with the provided name using one file to hold all shaders. Function takes either a
                 * file path to the shader on the disk or the direct source written as a string literal. Different types
                 * of shaders must be separated with the '#type ...' header before anything else, taking in the type of
                 * shader.
                 * @param shaderName   - Name for the shader to have.
                 * @param shaderSource - Filepath or direct source of the shader.
                 */
                OpenGLShader(const std::string& shaderName, const std::string& shaderSource);

                /**
                 * Compile a shader with the provided name using separate files to hold all shader components. Function
                 * takes either a file path to the shader on the disk or the direct source written as a string literal.
                 * @param shaderName   - Name for the shader to have.
                 * @param shaderSource - Filepath or direct source of the shader.
                 */
                OpenGLShader(const std::string& shaderName, const std::string& vertexSource, const std::string& fragmentSource);
                ~OpenGLShader() override;

                void Bind() const override;
                void Unbind() const override;

                void SetInt(const std::string& shaderName, int value) override;
                void SetFloat(const std::string& shaderName, float value) override;
                void SetVec2(const std::string& shaderName, const glm::vec2& vector) override;
                void SetVec3(const std::string& shaderName, const glm::vec3& vector) override;
                void SetVec4(const std::string& shaderName, const glm::vec4& vector) override;
                void SetMat3(const std::string& shaderName, const glm::mat3& matrix) override;
                void SetMat4(const std::string& shaderName, const glm::mat4& matrix) override;

                _NODISCARD_ const std::string& GetName() const override;

            private:
                class OpenGLShaderData;
                OpenGLShaderData* _data;
        };

    }
}

#endif //SPARK_OPENGL_SHADER_H
