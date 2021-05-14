
#include <spark/graphics/renderer/shader.h> // IShader
#include <spark/graphics/renderer/renderer_api.h>
#include <platform/opengl/renderer/opengl_shader.h> // OpenGLShader

namespace Spark::Graphics {

    IShader *IShader::Create(const std::string& shaderName, const std::string& shaderSource) {
        switch (IRenderingAPI::GetAPI()) {
            case IRenderingAPI::API::NONE:
                break;
            case IRenderingAPI::API::OPENGL:
                return new OpenGLShader(shaderName, shaderSource);
        }

        SP_ASSERT(false, "");
        return nullptr;
    }

    IShader *IShader::Create(const std::string& shaderName, const std::string& vertexSource, const std::string& fragmentSource) {
        switch (IRenderingAPI::GetAPI()) {
            case IRenderingAPI::API::NONE:
                break;
            case IRenderingAPI::API::OPENGL:
                return new OpenGLShader(shaderName, vertexSource, fragmentSource);
        }

        SP_ASSERT(false, "");
        return nullptr;
    }
}
