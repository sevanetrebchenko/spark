
#ifndef SPARK_OPENGL_INDEX_BUFFER_OBJECT_H
#define SPARK_OPENGL_INDEX_BUFFER_OBJECT_H

#include <spark/graphics/renderer/index_buffer_object.h> // IIndexBufferObject

namespace Spark {
    namespace Graphics {

        class OpenGLIndexBufferObject : public IIndexBufferObject {
            public:
                OpenGLIndexBufferObject(unsigned *indices, unsigned indexCount);
                ~OpenGLIndexBufferObject() override;

                void Bind() const override;
                void Unbind() const override;

                NODISCARD unsigned GetIndexCount() const override;

            private:
                class OpenGLIndexBufferData;
                OpenGLIndexBufferData* _data;
        };

    }
}

#endif //SPARK_OPENGL_INDEX_BUFFER_OBJECT_H
