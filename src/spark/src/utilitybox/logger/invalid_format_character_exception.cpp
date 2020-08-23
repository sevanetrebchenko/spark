
#include <utilitybox/logger/invalid_format_character_exception.h>

namespace Spark::UtilityBox::Logger {

    InvalidFormatCharacterException::InvalidFormatCharacterException(const char* formatString, ...) : _formatString(formatString),
                                                                                                      _processingBufferSize(strlen(_formatString)),
                                                                                                      _processingBuffer(new char[_processingBufferSize]) {
        std::va_list argsList;
        va_start(argsList, formatString);
        ProcessMessage(formatString, argsList);
        va_end(argsList);
    }

    const char *InvalidFormatCharacterException::what() const noexcept {
        return _processingBuffer;
    }

    void InvalidFormatCharacterException::ProcessMessage(const char* formatString, std::va_list argsList) {
        unsigned currentBufferSize = _processingBufferSize;

        // Copy args list to not modify passed parameters (yet).
        std::va_list argsCopy;
        va_copy(argsCopy, argsList);

        // If size of the buffer is zero, nothing is written and buffer may be a null pointer, however the return
        // value (number of bytes that would be written not including the null terminator) is still calculated and returned.
        int writeResult = vsnprintf(nullptr, 0, formatString, argsCopy);

        // If buffer size is equal to write result, there will not be space for the null terminator for the string.
        // Multiple buffer size by two to adequately house string in a new buffer.
        while (_processingBufferSize <= writeResult) {
            _processingBufferSize *= 2;
        }

        // Reallocate buffer.
        if (currentBufferSize != _processingBufferSize) {
            delete [] _processingBuffer;
            _processingBuffer = new char[_processingBufferSize];
        }

        vsnprintf(_processingBuffer, _processingBufferSize, formatString, argsList);
    }
}
