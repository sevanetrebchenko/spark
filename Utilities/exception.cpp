
#include "exception.h"

namespace UtilityBox {
    namespace Exceptions {
        struct Exception::ExceptionData {
            ExceptionData();
            ~ExceptionData();
            unsigned _processingBufferSize;
            char* _processingBuffer;
        };

        Exception::ExceptionData::ExceptionData() : _processingBufferSize(64u) {
            _processingBuffer = new(std::nothrow) char[_processingBufferSize];
            // todo::assert
        }

        Exception::ExceptionData::~ExceptionData() {
            delete[] _processingBuffer;
        }

        Exception::Exception(const char *formatString, ...) : _data(std::make_unique<ExceptionData>()){
            std::va_list args;
            va_start(args, formatString);
            ProcessMessage(formatString, args);
            va_end(args);
        }

        void Exception::ProcessMessage(const char *formatString, std::va_list argList) {
            // calculate correct number of bytes to write
            unsigned currentBufferSize = _data->_processingBufferSize;

            // copy args list to not modify passed parameters
            std::va_list argsCopy;
            va_copy(argsCopy, argList);
            // If size of the buffer is zero, nothing is written and buffer may be a null pointer, however the return value (number of bytes that would be written not including the null terminator) is still calculated and returned.
            int writeResult = vsnprintf(nullptr, 0, formatString, argsCopy);

            // get buffer size of powers of 2 to match the size correctly
            // if buffer size is equal to write result, there will not be space for the null terminator for the string
            while (_data->_processingBufferSize <= writeResult) {
                _data->_processingBufferSize *= 2;
            }

            // reallocate buffer
            if (currentBufferSize != _data->_processingBufferSize) {
                delete [] _data->_processingBuffer;
                _data->_processingBuffer = new char[_data->_processingBufferSize];
            }

            // write data to buffer
            vsnprintf(_data->_processingBuffer, _data->_processingBufferSize, formatString, argList);
        }

        const char *Exception::what() const noexcept {
            return _data->_processingBuffer;
        }

        Exception::~Exception() noexcept {
            _data.reset();
        }
    }
}
