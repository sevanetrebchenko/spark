
#include <glad/glad.h>
#include <platform/opengl/renderer/opengl_vertex_buffer_object.h>

namespace Spark::Graphics {

    struct OpenGLVertexBufferObject::OpenGLVertexBufferData {
        OpenGLVertexBufferData();

        unsigned _bufferID;
        VertexBufferLayout _bufferLayout;
    };

    OpenGLVertexBufferObject::OpenGLVertexBufferData::OpenGLVertexBufferData() : _bufferID(0) {
        // Nothing to do here.
    }

    OpenGLVertexBufferObject::OpenGLVertexBufferObject(unsigned int dataSize) : _data(new OpenGLVertexBufferData()) {
        glCreateBuffers(1, &_data->_bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, _data->_bufferID);
        glBufferData(GL_ARRAY_BUFFER, dataSize, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBufferObject::OpenGLVertexBufferObject(float *vertices, unsigned int dataSize) : _data(new OpenGLVertexBufferData()) {
        glCreateBuffers(1, &_data->_bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, _data->_bufferID);
        glBufferData(GL_ARRAY_BUFFER, dataSize, vertices, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBufferObject::~OpenGLVertexBufferObject() {
        glDeleteBuffers(1, &_data->_bufferID);
        delete _data;
    }

    void OpenGLVertexBufferObject::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, _data->_bufferID);
    }

    void OpenGLVertexBufferObject::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBufferObject::SetData(const void *dataBase, unsigned int dataSize) {
        glBindBuffer(GL_ARRAY_BUFFER, _data->_bufferID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, dataBase);
    }

    const VertexBufferLayout &OpenGLVertexBufferObject::GetBufferLayout() const {
        return _data->_bufferLayout;
    }

    void OpenGLVertexBufferObject::SetBufferLayout(const VertexBufferLayout& bufferLayout) {
        _data->_bufferLayout = bufferLayout;
    }
}
