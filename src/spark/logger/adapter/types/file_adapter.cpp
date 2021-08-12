
#include "spark/logger/adapter/types/file_adapter.h"

namespace Spark::Logger {

    FileAdapter::FileAdapter(const std::string &outputFilename, const AdapterConfiguration& configuration) : IAdapter(configuration),
                                                                                                             output_(outputFilename, std::ios::trunc)
                                                                                                             {
    }

    FileAdapter::~FileAdapter() {
        output_.close();
    }

    void FileAdapter::OutputMessage(const std::string &message) {
        output_ << message;
    }

}
