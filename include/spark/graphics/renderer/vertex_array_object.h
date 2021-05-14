
#ifndef SPARK_VERTEX_ARRAY_OBJECT_H
#define SPARK_VERTEX_ARRAY_OBJECT_H

#include <spark/core/core.h>
#include <spark/graphics/renderer/vertex_buffer_object.h>
#include <spark/graphics/renderer/index_buffer_object.h>

namespace Spark {
    namespace Graphics {

        class IVertexArrayObject {
            public:
                static IVertexArrayObject* Create();
                virtual ~IVertexArrayObject() = default;

                virtual void Bind() const = 0;
                virtual void Unbind() const = 0;

                virtual void AddVertexBuffer(IVertexBufferObject* vertexBuffer) = 0;
                virtual void SetIndexBuffer(IIndexBufferObject* indexBuffer) = 0;

                _NODISCARD_ virtual const std::vector<IVertexBufferObject*>& GetVertexBuffers() const = 0;
                _NODISCARD_ virtual const IIndexBufferObject* GetIndexBuffer() const = 0;

        };

    }
}

#endif //SPARK_VERTEX_ARRAY_OBJECT_H
