
#ifndef SPARK_OPENGL_VERTEX_BUFFER_OBJECT_H
#define SPARK_OPENGL_VERTEX_BUFFER_OBJECT_H

#include <spark/graphics/renderer/vertex_buffer_object.h> // IVertexBufferObject

namespace Spark {
    namespace Graphics {

        class OpenGLVertexBufferObject : public IVertexBufferObject {
            public:
                explicit OpenGLVertexBufferObject(unsigned dataSize);
                OpenGLVertexBufferObject(float* vertices, unsigned dataSize);
                ~OpenGLVertexBufferObject() override;

                void Bind() const override;
                void Unbind() const override;

                void SetData(const void* dataBase, unsigned dataSize) override;

                NODISCARD const VertexBufferLayout& GetBufferLayout() const override;
                void SetBufferLayout(const VertexBufferLayout& layout) override;

            private:
                class OpenGLVertexBufferData;
                OpenGLVertexBufferData* _data;
        };

    }
}

#endif //SPARK_OPENGL_VERTEX_BUFFER_OBJECT_H
