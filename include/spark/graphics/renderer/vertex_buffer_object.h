
#ifndef SPARK_VERTEX_BUFFER_OBJECT_H
#define SPARK_VERTEX_BUFFER_OBJECT_H

#include <spark/core/rename.h>
#include <spark/graphics/renderer/vertex_buffer_layout.h> // VertexBufferLayout, VertexBufferElement

namespace Spark {
    namespace Graphics {

        class IVertexBufferObject {
            public:
                static IVertexBufferObject* Create(unsigned size);
                static IVertexBufferObject* Create(float* vertices, unsigned size);
                virtual ~IVertexBufferObject() = default;

                virtual void Bind() const = 0;
                virtual void Unbind() const = 0;

                virtual void SetData(const void* dataBase, unsigned dataSize) = 0;

                NODISCARD virtual const VertexBufferLayout& GetBufferLayout() const = 0;
                virtual void SetBufferLayout(const VertexBufferLayout& layout) = 0;
        };

    }
}

#endif // SPARK_VERTEX_BUFFER_OBJECT_H
