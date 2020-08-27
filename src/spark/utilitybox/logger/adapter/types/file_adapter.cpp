
#include <spark/utilitybox/logger/adapter/types/file_adapter.h> // FileAdapter
#include <codecvt>

namespace Spark::UtilityBox::Logger {

    FileAdapter::FileAdapter(const char *fileName) : IAdapter(std::string("File Adapter: ") + std::string(fileName)),
                                                     _fileName(fileName) {
        //GetConfiguration()->SetFormattingString("[%c] | [%s] | %d | - [ %t ] - %m\n");
        GetConfiguration()->SetFormattingString("[%n] | [%s] | %d | - [ %t ] - %m\n");
        GetConfiguration()->SetTimestampFormattingString("%mm %ss %lms");
        GetConfiguration()->SetMessageWrapLimit(200);
    }

    void FileAdapter::Initialize() {
        _file.open(_fileName, std::ios::out | std::ios::trunc);
        if (!_file.is_open()) {
            static std::stringstream format;
            format << "File: " << _fileName << " failed to open. Adapter will not be attached.";
            std::string errorMessage = format.str();
            format.str(std::string());

            throw std::runtime_error(errorMessage.c_str());
        }
        else {
            unsigned char smarker[3];
            smarker[0] = 0xEF;
            smarker[1] = 0xBB;
            smarker[2] = 0xBF;
            _file << smarker;
        }
    }

    FileAdapter::~FileAdapter() {
        _file.flush();
        _file.close();
    }

    // Print all messages in the formatted messages buffer to the stream. Separates concurrent messages with a newline to aid in clarity.
    void FileAdapter::OutputMessage(std::vector<std::string> messages) {
        ++_logCount;

        for (auto& message : messages) {
            _file << message;
        }

        _file.flush();
    }
}


