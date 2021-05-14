
#include <spark/graphics/renderer/vertex_buffer_object.h>
#include <spark/graphics/renderer/renderer_api.h>
#include <platform/opengl/renderer/opengl_vertex_buffer_object.h>

namespace Spark::Graphics {

    IVertexBufferObject* IVertexBufferObject::Create(unsigned int dataSize) {
        switch (IRenderingAPI::GetAPI()) {
            case IRenderingAPI::API::NONE:
                break;
            case IRenderingAPI::API::OPENGL:
                return new OpenGLVertexBufferObject(dataSize);
        }

        return nullptr;
    }

    IVertexBufferObject *IVertexBufferObject::Create(float *vertices, unsigned int dataSize) {
        switch (IRenderingAPI::GetAPI()) {
            case IRenderingAPI::API::NONE:
                break;
            case IRenderingAPI::API::OPENGL:
                return new OpenGLVertexBufferObject(vertices, dataSize);
        }

        return nullptr;
    }

}
