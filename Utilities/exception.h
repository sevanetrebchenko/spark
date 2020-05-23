#ifndef DATASTRUCTURES_EXCEPTION_H
#define DATASTRUCTURES_EXCEPTION_H

#define _NODISCARD_ [[nodiscard]]

#include <exception>
#include <string>
#include <memory>
#include <cstdarg>

namespace UtilityBox::Exceptions {
    class Exception : public std::exception {
        public:
            explicit Exception(const char* formatString, ...);
            ~Exception() noexcept override;
            _NODISCARD_ const char* what() const noexcept override;

        private:
            void ProcessMessage(const char* formatString, std::va_list argList);

            struct ExceptionData;
            std::unique_ptr<ExceptionData> _data;
    };
}


#endif //DATASTRUCTURES_EXCEPTION_H
