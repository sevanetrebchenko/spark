
#ifndef SPARK_OPENGL_VERTEX_ARRAY_OBJECT_H
#define SPARK_OPENGL_VERTEX_ARRAY_OBJECT_H

#include <spark/graphics/renderer/vertex_array_object.h>

namespace Spark {
    namespace Graphics {

        class OpenGLVertexArrayObject : public IVertexArrayObject {
            public:
                OpenGLVertexArrayObject();
                ~OpenGLVertexArrayObject() override;

                void Bind() const override;
                void Unbind() const override;

                void AddVertexBuffer(IVertexBufferObject* vertexBuffer) override;
                void SetIndexBuffer(IIndexBufferObject* indexBuffer) override;

                _NODISCARD_ const std::vector<IVertexBufferObject*>& GetVertexBuffers() const override;
                _NODISCARD_ const IIndexBufferObject* GetIndexBuffer() const override;

            private:
                class OpenGLVertexArrayObjectData;
                OpenGLVertexArrayObjectData* _data;
        };

    }
}

#endif //SPARK_OPENGL_VERTEX_ARRAY_OBJECT_H
