
#include "spark/utilitybox/logger/adapter/adapter.h"

namespace Spark::UtilityBox {

    IAdapter::IAdapter(const AdapterConfiguration& configuration) : configuration_(configuration) {
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
    }

}
