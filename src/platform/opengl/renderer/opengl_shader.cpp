
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <platform/opengl/renderer/opengl_shader.h>
#include <spark/utilitybox/logger/logging_interface.h>

namespace Spark::Graphics {
    //------------------------------------------------------------------------------------------------------------------
    // OPENGL SHADER DATA
    //------------------------------------------------------------------------------------------------------------------
    struct OpenGLShader::OpenGLShaderData : public UtilityBox::Logger::ILoggable {
        public:
            OpenGLShaderData(std::string  shaderName, const std::string& shaderData);
            OpenGLShaderData(std::string shaderName, const std::string& vertexSource, const std::string& fragmentSource);
            ~OpenGLShaderData();

            std::unordered_map<std::string, GLint> _uniformLocations;
            GLuint _shaderID;
            std::string _shaderName;

        private:
            bool IsPath(const std::string& filepath) const;
            std::string ReadFile(const std::string& filepath);
            GLenum ShaderTypeFromString(const std::string& shaderType) const;
            std::string ShaderTypeToString(GLenum shaderType) const;
            std::unordered_map<GLenum, std::string> RetrieveShaderComponents(const std::string& shaderSource);
            void CompileShader(const std::unordered_map<GLenum, std::string>& shaderComponents);
    };

    OpenGLShader::OpenGLShaderData::OpenGLShaderData(std::string shaderName, const std::string& shaderData) : UtilityBox::Logger::ILoggable(std::string("OpenGL Shader: ") + shaderName),
                                                                                                              _shaderName(std::move(shaderName)) {
        std::string shaderSource;
        // Read the file into a string if the shaderData is a path. Do nothing otherwise.
        if (IsPath(shaderData)) {
            shaderSource = ReadFile(shaderData);
        }
        else {
            shaderSource = shaderData;
        }

        CompileShader(RetrieveShaderComponents(shaderSource));
    }

    OpenGLShader::OpenGLShaderData::OpenGLShaderData(std::string shaderName, const std::string& vertexSource, const std::string& fragmentSource) : _shaderName(std::move(shaderName)) {
        // name, shader source (vertex, fragment)
        // name, shader path (vertex, fragment)
    }

    OpenGLShader::OpenGLShaderData::~OpenGLShaderData() {
        glDeleteProgram(_shaderID);
        _shaderID = 0;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::OpenGLShaderData::RetrieveShaderComponents(const std::string& shaderSource) {
        std::unordered_map<GLenum, std::string> shaderComponents;
        const char* typeToken = "#type";
        std::size_t typeTokenLength = strlen(typeToken);

        // Find '#type' keyword in the shader file.
        std::size_t typeTokenBeginIndex = shaderSource.find(typeToken, 0);

        while (typeTokenBeginIndex != std::string::npos) {
            // Get to the end of the line that has the '#type' token.
            std::size_t endOfTypeTokenLine = shaderSource.find_first_of('\n', typeTokenBeginIndex); // Find newline. // TODO: cross platform
            SP_ASSERT(endOfTypeTokenLine != std::string::npos, "Syntax error: No provided '#type' specification for shader source with name: '%s'.", _shaderName.c_str());

            // Get type of shader after '#type' keyword.
            std::size_t shaderTypeBeginIndex = typeTokenBeginIndex + typeTokenLength + 1; // Start of the shader type name after '#type' keyword.
            std::string shaderTypeString = shaderSource.substr(shaderTypeBeginIndex, endOfTypeTokenLine - shaderTypeBeginIndex);
            ConvertToLowercase(shaderTypeString);
            GLenum shaderType = ShaderTypeFromString(shaderTypeString);
            SP_ASSERT(shaderType != GL_INVALID_VALUE, "Syntax error: Invalid shader type keyword provided after '#type' specification in shader source with name: '%s'.", _shaderName.c_str());

            // Get shader source code.
            std::size_t nextLinePos = shaderSource.find_first_not_of('\n', endOfTypeTokenLine); // Start of shader code after shader type declaration line.
            SP_ASSERT(nextLinePos != std::string::npos, "Syntax error: No provided shader source code for shader with name: '%s'.", _shaderName.c_str());
            typeTokenBeginIndex = shaderSource.find(typeToken, nextLinePos); // Start of the line with the type declaration of the next shader type.

            shaderComponents[shaderType] = (typeTokenBeginIndex == std::string::npos) ? shaderSource.substr(nextLinePos) : shaderSource.substr(nextLinePos, typeTokenBeginIndex - nextLinePos);
        }

        return std::move(shaderComponents);
    }

    GLenum OpenGLShader::OpenGLShaderData::ShaderTypeFromString(const std::string& shaderType) const {
        if (shaderType == "vertex") {
            return GL_VERTEX_SHADER;

        }
        if (shaderType == "fragment") {
            return GL_FRAGMENT_SHADER;
        }

        return GL_INVALID_VALUE;
    }

    bool OpenGLShader::OpenGLShaderData::IsPath(const std::string &filepath) const {
        std::ifstream fileStream(filepath.c_str());
        return fileStream.good();
    }

    std::string OpenGLShader::OpenGLShaderData::ReadFile(const std::string &filepath) {
        static std::stringstream fileBuffer;
        std::ifstream fileReader;

        // Open the file.
        fileReader.open(filepath);
        if (fileReader.is_open()) {
            fileBuffer << fileReader.rdbuf();
        }
        else {
            // Could not open file
        }

        std::string fileContents = fileBuffer.str();
        fileBuffer.str(std::string()); // Clear buffer contents.
        return std::move(fileContents);
    }

    void OpenGLShader::OpenGLShaderData::CompileShader(const std::unordered_map<GLenum, std::string>& shaderComponents) {
        GLuint shaderProgram = glCreateProgram();
        unsigned numShaderComponents = shaderComponents.size();
        GLuint* shaders = new GLenum[numShaderComponents];
        unsigned currentShaderIndex = 0;

        //--------------------------------------------------------------------------------------------------------------
        // SHADER COMPONENT COMPILING
        //--------------------------------------------------------------------------------------------------------------
        for (auto& shaderComponent : shaderComponents) {
            GLenum shaderType = shaderComponent.first;
            const GLchar* shaderSource = reinterpret_cast<const GLchar*>(shaderComponent.second.c_str());

            // Create shader from source.
            GLuint shader = glCreateShader(shaderType);
            glShaderSource(shader, 1, &shaderSource, nullptr); // If length is NULL, each string is assumed to be null terminated.
            glCompileShader(shader);

            // Compile shader source code.
            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (!isCompiled) {
                // Shader failed to compile - get error information from OpenGL.
                GLint errorMessageLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorMessageLength);

                std::vector<GLchar> errorMessageBuffer;
                errorMessageBuffer.resize(errorMessageLength);
                glGetShaderInfoLog(shader, errorMessageLength, &errorMessageLength, &errorMessageBuffer[0]);
                std::string errorMessage(errorMessageBuffer.begin(), errorMessageBuffer.end());

                glDeleteShader(shader);
                SP_ASSERT(false, "Shader %s failed to compile %s component. Provided error information: %s.", _shaderName.c_str(), ShaderTypeToString(shaderType).c_str(), errorMessage.c_str());
            }

            // Shader successfully compiled.
            glAttachShader(shaderProgram, shader);
            shaders[currentShaderIndex++] = shader;
        }

        //--------------------------------------------------------------------------------------------------------------
        // SHADER PROGRAM LINKING
        //--------------------------------------------------------------------------------------------------------------
        glLinkProgram(shaderProgram);

        GLint isLinked = 0;
        glGetShaderiv(shaderProgram, GL_LINK_STATUS, &isLinked);
        if (!isLinked) {
            // Shader failed to link - get error information from OpenGL.
            GLint errorMessageLength = 0;
            glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &errorMessageLength);

            std::vector<GLchar> errorMessageBuffer;
            errorMessageBuffer.resize(errorMessageLength);
            glGetShaderInfoLog(shaderProgram, errorMessageLength, &errorMessageLength, &errorMessageBuffer[0]);
            std::string errorMessage(errorMessageBuffer.begin(), errorMessageBuffer.end());

            // Program is unnecessary at this point.
            glDeleteProgram(shaderProgram);

            // Delete shader components.
            for (int i = 0; i < numShaderComponents; ++i) {
                glDeleteShader(shaders[i]);
            }

            SP_ASSERT(false, "Shader %s failed to link. Provided error information: %s.", _shaderName.c_str(), errorMessage.c_str());
        }

        // Shader is successfully initialized.
        _shaderID = shaderProgram;

        // Shader components are no longer necessary.
        for (int i = 0; i < numShaderComponents; ++i) {
            GLuint shaderComponentID = shaders[i];
            glDetachShader(shaderProgram, shaderComponentID);
            glDeleteShader(shaderComponentID);
        }
    }

    std::string OpenGLShader::OpenGLShaderData::ShaderTypeToString(GLenum shaderType) const {
        switch(shaderType) {
            case GL_FRAGMENT_SHADER:
                return "FRAGMENT";
            case GL_VERTEX_SHADER:
                return "VERTEX";
            default:
                SP_ASSERT(false, "Unknown/Not handled shader type: %i provided to ShaderTypeToString.", shaderType);
                return "";
        }
    }


    //------------------------------------------------------------------------------------------------------------------
    // OPENGL SHADER
    //------------------------------------------------------------------------------------------------------------------
    OpenGLShader::OpenGLShader(const std::string& shaderName, const std::string& shaderSource) : _data(new OpenGLShaderData(shaderName, shaderSource)) {
        // Nothing to do here.
    }

    OpenGLShader::OpenGLShader(const std::string& shaderName, const std::string& vertexSource, const std::string& fragmentSource) : _data(new OpenGLShaderData(shaderName, vertexSource, fragmentSource)) {
        // Nothing to do here.
    }

    OpenGLShader::~OpenGLShader() {
        delete _data;
    }

    void OpenGLShader::Bind() const {
        glUseProgram(_data->_shaderID);
    }

    void OpenGLShader::Unbind() const {
        glUseProgram(NULL);
    }

    void OpenGLShader::SetInt(const std::string& name, int value) {
        auto uniformLocation = _data->_uniformLocations.find(name);

        // Location not found.
        if (uniformLocation == _data->_uniformLocations.end()) {
            // Find location first.
            GLint location = glGetUniformLocation(_data->_shaderID, name.c_str());
            glUniform1i(location, value);

            _data->_uniformLocations.emplace(name, location);
        }
        else {
            glUniform1i(uniformLocation->second, value);
        }
    }

    void OpenGLShader::SetFloat(const std::string& name, float value) {
        auto uniformLocation = _data->_uniformLocations.find(name);

        // Location not found.
        if (uniformLocation == _data->_uniformLocations.end()) {
            // Find location first.
            GLint location = glGetUniformLocation(_data->_shaderID, name.c_str());
            glUniform1f(location, value);

            _data->_uniformLocations.emplace(name, location);
        }
        else {
            glUniform1f(uniformLocation->second, value);
        }
    }

    void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& vector) {
        auto uniformLocation = _data->_uniformLocations.find(name);

        // Location not found.
        if (uniformLocation == _data->_uniformLocations.end()) {
            // Find location first.
            GLint location = glGetUniformLocation(_data->_shaderID, name.c_str());
            glUniform2fv(location, 1, glm::value_ptr(vector));

            _data->_uniformLocations.emplace(name, location);
        }
        else {
            glUniform2fv(uniformLocation->second, 1, glm::value_ptr(vector));
        }
    }

    void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& vector) {
        auto uniformLocation = _data->_uniformLocations.find(name);

        // Location not found.
        if (uniformLocation == _data->_uniformLocations.end()) {
            // Find location first.
            GLint location = glGetUniformLocation(_data->_shaderID, name.c_str());
            glUniform3fv(location, 1, glm::value_ptr(vector));

            _data->_uniformLocations.emplace(name, location);
        }
        else {
            glUniform3fv(uniformLocation->second, 1, glm::value_ptr(vector));
        }
    }

    void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& vector) {
        auto uniformLocation = _data->_uniformLocations.find(name);

        // Location not found.
        if (uniformLocation == _data->_uniformLocations.end()) {
            // Find location first.
            GLint location = glGetUniformLocation(_data->_shaderID, name.c_str());
            glUniform4fv(location, 1, glm::value_ptr(vector));

            _data->_uniformLocations.emplace(name, location);
        }
        else {
            glUniform4fv(uniformLocation->second, 1, glm::value_ptr(vector));
        }
    }

    void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix) {
        auto uniformLocation = _data->_uniformLocations.find(name);

        // Location not found.
        if (uniformLocation == _data->_uniformLocations.end()) {
            // Find location first.
            GLint location = glGetUniformLocation(_data->_shaderID, name.c_str());
            glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));

            _data->_uniformLocations.emplace(name, location);
        }
        else {
            glUniformMatrix3fv(uniformLocation->second, 1, GL_FALSE, glm::value_ptr(matrix));
        }
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix) {
        auto uniformLocation = _data->_uniformLocations.find(name);

        // Location not found.
        if (uniformLocation == _data->_uniformLocations.end()) {
            // Find location first.
            GLint location = glGetUniformLocation(_data->_shaderID, name.c_str());
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));

            _data->_uniformLocations.emplace(name, location);
        }
        else {
            glUniformMatrix4fv(uniformLocation->second, 1, GL_FALSE, glm::value_ptr(matrix));
        }
    }

    const std::string& OpenGLShader::GetName() const {
        return _data->_shaderName;
    }

}
