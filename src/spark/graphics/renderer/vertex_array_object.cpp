
#include <spark/graphics/renderer/vertex_array_object.h>
#include <spark/graphics/renderer/renderer_api.h>
#include <platform/opengl/renderer/opengl_vertex_array_object.h>

namespace Spark::Graphics {

    IVertexArrayObject *IVertexArrayObject::Create() {
        switch (IRenderingAPI::GetAPI()) {
            case IRenderingAPI::API::NONE:
                break;
            case IRenderingAPI::API::OPENGL:
                return new OpenGLVertexArrayObject();
        }

        return nullptr;
    }
}
