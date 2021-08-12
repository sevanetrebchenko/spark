
#include "spark/logger/adapter/adapter.h"

namespace Spark::Logger {

    IAdapter::IAdapter(AdapterConfiguration configuration) : logCount_(0),
                                                             configuration_(std::move(configuration)) {
    }

    const AdapterConfiguration& IAdapter::GetAdapterConfiguration() const {
        return configuration_;
    }

    const std::string &IAdapter::GetName() const {
        return configuration_.name;
    }

    void IAdapter::OutputMessages(const std::vector<std::string> &messages) {
        for (const std::string& message : messages) {
            OutputMessage(message);
        }

        ++logCount_;
    }

    int IAdapter::GetLogCount() const {
        return logCount_;
    }

}
