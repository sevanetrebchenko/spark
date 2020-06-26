
#include "exception.h"

namespace UtilityBox::Exceptions {
    Exception::Exception(const char *formatString) : _message(formatString) {
        // Nothing to do here.
    }

    const char *Exception::what() const noexcept {
        return _message;
    }

    Exception::Exception(const Exception& exception) noexcept : _message(exception._message) {
    }
}
