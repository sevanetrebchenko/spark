
#include "spark/logger/adapter/types/standard_output_adapter.h"

namespace Spark::Logger {

    StandardOutputAdapter::StandardOutputAdapter(std::ostream& outputStream, const AdapterConfiguration& configuration) : IAdapter(configuration),
                                                                                                                          output_(outputStream)
                                                                                                                          {
    }

    StandardOutputAdapter::~StandardOutputAdapter() = default;

    void StandardOutputAdapter::OutputMessage(const std::string &message) {
        output_ << message << std::endl;
    }

}