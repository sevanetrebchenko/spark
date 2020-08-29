
#include <spark/graphics/renderer/vertex_buffer_layout.h>

namespace Spark::Graphics {

    //------------------------------------------------------------------------------------------------------------------
    // BUFFER ELEMENT DATA
    //------------------------------------------------------------------------------------------------------------------
    struct VertexBufferElement::BufferElementData {
        BufferElementData(std::string elementName, ShaderDataType shaderDataType, unsigned elementSize, unsigned bufferOffset);

        std::string _elementName;
        ShaderDataType _shaderDataType;
        unsigned _elementSize;
        unsigned _bufferOffset;
    };

    VertexBufferElement::BufferElementData::BufferElementData(std::string elementName, ShaderDataType shaderDataType, unsigned int elementSize, unsigned int bufferOffset) : _elementName(std::move(elementName)),
                                                                                                                                                                             _shaderDataType(shaderDataType),
                                                                                                                                                                             _elementSize(elementSize),
                                                                                                                                                                             _bufferOffset(bufferOffset) {
        // Nothing to do here.
    }

    //------------------------------------------------------------------------------------------------------------------
    // BUFFER ELEMENT
    //------------------------------------------------------------------------------------------------------------------
    VertexBufferElement::VertexBufferElement(ShaderDataType shaderDataType, std::string elementName) : _data(new BufferElementData(std::move(elementName), shaderDataType, ShaderDataTypeSize(shaderDataType), 0)) {
    }

    unsigned VertexBufferElement::GetComponentCount() const {
        switch (_data->_shaderDataType) {
            case ShaderDataType::BOOL:
            case ShaderDataType::INT:
            case ShaderDataType::FLOAT:
                return 1;
            case ShaderDataType::INT2:
            case ShaderDataType::FLOAT2:
            case ShaderDataType::MAT2:
                return 2;
            case ShaderDataType::INT3:
            case ShaderDataType::FLOAT3:
            case ShaderDataType::MAT3:
                return 3;
            case ShaderDataType::INT4:
            case ShaderDataType::FLOAT4:
            case ShaderDataType::MAT4:
                return 4;
            default:
                SP_ASSERT(false, ""); // TODO
                return 0;
        }
    }

    const std::string &VertexBufferElement::GetElementName() const {
        return _data->_elementName;
    }

    void VertexBufferElement::SetElementName(std::string elementName) const {
        _data->_elementName = std::move(elementName);
    }

    const ShaderDataType &VertexBufferElement::GetShaderDataType() const {
        return _data->_shaderDataType;
    }

    void VertexBufferElement::SetShaderDataType(ShaderDataType shaderDataType) {
        _data->_shaderDataType = shaderDataType;
    }

    size_t VertexBufferElement::GetBufferOffset() const {
        return _data->_bufferOffset;
    }

    void VertexBufferElement::SetBufferOffset(unsigned int bufferOffset) {
        _data->_bufferOffset = bufferOffset;
    }

    //------------------------------------------------------------------------------------------------------------------
    // BUFFER LAYOUT DATA
    //------------------------------------------------------------------------------------------------------------------
    struct VertexBufferLayout::BufferLayoutData {
        BufferLayoutData();
        BufferLayoutData(const std::initializer_list<VertexBufferElement>& bufferElements);

        std::vector<VertexBufferElement> _elements;
        unsigned _stride;
    };

    VertexBufferLayout::BufferLayoutData::BufferLayoutData(const std::initializer_list<VertexBufferElement> &bufferElements) : _elements(bufferElements),
                                                                                                                               _stride(0) {
        unsigned currentOffset = 0;
        for (auto& bufferElement : _elements) {
            // Initialize offsets for buffer elements.
            bufferElement.SetBufferOffset(currentOffset);

            unsigned elementSize = ShaderDataTypeSize(bufferElement.GetShaderDataType());
            currentOffset += elementSize;
            _stride += elementSize;
        }
    }

    VertexBufferLayout::BufferLayoutData::BufferLayoutData() : _elements(), _stride(0) {
        // Nothing to do here.
    }

    //------------------------------------------------------------------------------------------------------------------
    // BUFFER LAYOUT
    //------------------------------------------------------------------------------------------------------------------
    VertexBufferLayout::VertexBufferLayout() : _data(new BufferLayoutData()){
        // Nothing to do here.
    }

    VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexBufferElement>& bufferElements) : _data(new BufferLayoutData(bufferElements)) {
        // Nothing to do here.
    }

    unsigned VertexBufferLayout::GetStride() const {
        return _data->_stride;
    }

    const std::vector<VertexBufferElement>& VertexBufferLayout::GetBufferElements() const {
        return _data->_elements;
    }

    std::vector<VertexBufferElement>::iterator VertexBufferLayout::begin() {
        return _data->_elements.begin();
    }

    std::vector<VertexBufferElement>::iterator VertexBufferLayout::end() {
        return _data->_elements.end();
    }

    std::vector<VertexBufferElement>::const_iterator VertexBufferLayout::cbegin() {
        return _data->_elements.cbegin();
    }

    std::vector<VertexBufferElement>::const_iterator VertexBufferLayout::cend() {
        return _data->_elements.cend();
    }

}
