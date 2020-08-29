
#include <glad/glad.h>
#include <platform/opengl/renderer/opengl_index_buffer_object.h> // OpenGLIndexBufferObject

namespace Spark::Graphics {

    struct OpenGLIndexBufferObject::OpenGLIndexBufferData {
        unsigned _bufferID;
        unsigned _indexCount;
    };

    OpenGLIndexBufferObject::OpenGLIndexBufferObject(unsigned int *indices, unsigned int indexCount) : _data(new OpenGLIndexBufferData()) {
        _data->_indexCount = indexCount;

        glCreateBuffers(1, &_data->_bufferID);

        // Binding with GL_ARRAY_BUFFER instead of GL_ELEMENT_ARRAY_BUFFER allows the data to be loaded regardless of the state of the vertex array object.
        glBindBuffer(GL_ARRAY_BUFFER, _data->_bufferID);
        glBufferData(GL_ARRAY_BUFFER, indexCount * sizeof(unsigned), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBufferObject::~OpenGLIndexBufferObject() {
        glDeleteBuffers(1, &_data->_bufferID);
    }

    void OpenGLIndexBufferObject::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, _data->_bufferID);
    }

    void OpenGLIndexBufferObject::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    unsigned OpenGLIndexBufferObject::GetIndexCount() const {
        return _data->_indexCount;
    }
}
