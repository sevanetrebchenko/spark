
#include <glad/glad.h>
#include <platform/opengl/renderer/opengl_vertex_array_object.h>

namespace Spark::Graphics {

    struct OpenGLVertexArrayObject::OpenGLVertexArrayObjectData {
        OpenGLVertexArrayObjectData();

        NODISCARD GLenum ConvertShaderDataTypeToOpenGLDataType(ShaderDataType shaderDataType) const;

        unsigned _bufferID;
        unsigned _vboAttributeIndex;
        std::vector<IVertexBufferObject*> _vertexBuffers;
        IIndexBufferObject* _indexBuffer;
    };

    OpenGLVertexArrayObject::OpenGLVertexArrayObjectData::OpenGLVertexArrayObjectData() : _bufferID(0),
                                                                                          _vboAttributeIndex(0),
                                                                                          _indexBuffer(nullptr) {
        // Nothing to do here.
    }

    GLenum OpenGLVertexArrayObject::OpenGLVertexArrayObjectData::ConvertShaderDataTypeToOpenGLDataType(ShaderDataType shaderDataType) const {
        switch (shaderDataType) {
            case ShaderDataType::BOOL:
                return GL_BOOL;
            case ShaderDataType::INT:
                return GL_INT;
            case ShaderDataType::FLOAT:
            case ShaderDataType::VEC2:
            case ShaderDataType::VEC3:
            case ShaderDataType::VEC4:
            case ShaderDataType::MAT2:
            case ShaderDataType::MAT3:
            case ShaderDataType::MAT4:
                return GL_FLOAT;
            default:
                SP_ASSERT(false, ""); // TODO
                return 0;
        }
    }

    OpenGLVertexArrayObject::OpenGLVertexArrayObject() : _data(new OpenGLVertexArrayObjectData()) {
        glCreateVertexArrays(1, &_data->_bufferID);
    }

    OpenGLVertexArrayObject::~OpenGLVertexArrayObject() {
        glDeleteVertexArrays(1, &_data->_bufferID);
    }

    void OpenGLVertexArrayObject::Bind() const {
        glBindVertexArray(_data->_bufferID);
    }

    void OpenGLVertexArrayObject::Unbind() const {
        glBindVertexArray(0);
    }

    void OpenGLVertexArrayObject::AddVertexBuffer(IVertexBufferObject *vertexBuffer) {
        SP_ASSERT(!vertexBuffer->GetBufferLayout().GetBufferElements().empty(), "Provided vertex buffer has no buffer layout present.");

        const VertexBufferLayout& vertexBufferLayout = vertexBuffer->GetBufferLayout();
        for (auto& vertexBufferElement : vertexBufferLayout.GetBufferElements()) {
            unsigned elementCount = vertexBufferElement.GetComponentCount();
            switch (vertexBufferElement.GetShaderDataType()) {
                case ShaderDataType::BOOL:
                case ShaderDataType::INT:
                case ShaderDataType::FLOAT:
                case ShaderDataType::VEC2:
                case ShaderDataType::VEC3:
                case ShaderDataType::VEC4:
                    glEnableVertexAttribArray(_data->_vboAttributeIndex);
                    glVertexAttribPointer(_data->_vboAttributeIndex,
                                          vertexBufferElement.GetComponentCount(),
                                          _data->ConvertShaderDataTypeToOpenGLDataType(vertexBufferElement.GetShaderDataType()),
                                          GL_FALSE,
                                          vertexBufferLayout.GetStride(),
                                          (void*)vertexBufferElement.GetBufferOffset());
                    ++_data->_vboAttributeIndex;
                    break;
                case ShaderDataType::MAT2:
                case ShaderDataType::MAT3:
                case ShaderDataType::MAT4:
                    for (int i = 0; i < elementCount; ++i) {
                        glEnableVertexAttribArray(_data->_vboAttributeIndex);
                        glVertexAttribPointer(_data->_vboAttributeIndex,
                                              vertexBufferElement.GetComponentCount(),
                                              _data->ConvertShaderDataTypeToOpenGLDataType(vertexBufferElement.GetShaderDataType()),
                                              GL_FALSE,
                                              vertexBufferLayout.GetStride(),
                                              (void*)(vertexBufferElement.GetBufferOffset() + sizeof(float) * elementCount * i));
                        ++_data->_vboAttributeIndex;
                    }
                    break;
                default:
                    SP_ASSERT(false, ""); // TODO
            }
        }

        _data->_vertexBuffers.emplace_back(vertexBuffer);
    }

    void OpenGLVertexArrayObject::SetIndexBuffer(IIndexBufferObject *indexBuffer) {
        glBindVertexArray(_data->_bufferID);
        indexBuffer->Bind();
        _data->_indexBuffer = indexBuffer;
    }

    const std::vector<IVertexBufferObject*>& OpenGLVertexArrayObject::GetVertexBuffers() const {
        return _data->_vertexBuffers;
    }

    const IIndexBufferObject *OpenGLVertexArrayObject::GetIndexBuffer() const {
        return _data->_indexBuffer;
    }

}
