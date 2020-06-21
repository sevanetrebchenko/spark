#ifndef DATASTRUCTURES_EXCEPTION_H
#define DATASTRUCTURES_EXCEPTION_H

#include "global_defines.h"
#include <exception>
#include <string>
#include <memory>
#include <cstdarg>

namespace UtilityBox {
    namespace Exceptions {
        class Exception : public std::exception {
            public:
                explicit Exception(const char* formatString);
                Exception(const Exception& exception) noexcept;
                _NODISCARD_ const char* what() const noexcept override;

            private:
                const char* _message;
        };
    }
}


#endif //DATASTRUCTURES_EXCEPTION_H
