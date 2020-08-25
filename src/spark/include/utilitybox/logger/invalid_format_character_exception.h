
#ifndef SPARK_INVALID_FORMAT_CHARACTER_EXCEPTION_H
#define SPARK_INVALID_FORMAT_CHARACTER_EXCEPTION_H

#include <spark_pch.h>                             // std::exception, std::va_list
#include <utilitybox/tools/global_defines.h> // _NODISCARD_

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            class InvalidFormatCharacterException : std::exception {
                public:
                    explicit InvalidFormatCharacterException(const char* formatString, ...);
                    _NODISCARD_ const char* what() const noexcept override;

                private:
                    void ProcessMessage(const char* formatString, std::va_list argsList);
                    const char* _formatString;

                    unsigned _processingBufferSize;
                    char* _processingBuffer;
            };

        }
    }
}

#endif //SPARK_INVALID_FORMAT_CHARACTER_EXCEPTION_H
