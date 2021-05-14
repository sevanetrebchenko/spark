
#include <spark/graphics/renderer/index_buffer_object.h>         // IIndexBufferObject
#include <spark/graphics/renderer/renderer_api.h>         // IRenderingAPI
#include <platform/opengl/renderer/opengl_index_buffer_object.h> // OpenGLIndexBufferObject

namespace Spark::Graphics {

    IIndexBufferObject *IIndexBufferObject::Create(unsigned *indices, unsigned indexCount) {
        switch (IRenderingAPI::GetAPI()) {
            case IRenderingAPI::API::NONE:
                break;
            case IRenderingAPI::API::OPENGL:
                return new OpenGLIndexBufferObject(indices, indexCount);
                break;
        }

        return nullptr;
    }
}
