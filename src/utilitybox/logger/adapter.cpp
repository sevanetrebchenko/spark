
#include <spark_pch.h>                 // std::setw, std::setfill, std::vector, std::stringstream, std::string
#include <utilitybox/logger/logger.h>  // LogMessage
#include <utilitybox/logger/adapter.h> // Adapter

namespace Spark::UtilityBox::Logger {
    // Construct an adapter with default formatting with a given name.
    Adapter::Adapter(const char* adapterName) : _config(new AdapterConfiguration(adapterName)),
                                                _logCount(0) {
        // Nothing to do here.
    }

    // Destructor.
    Adapter::~Adapter() {
    }

    // Get the configuration for this adapter.
    AdapterConfiguration* Adapter::GetConfiguration() const {
        return _config;
    }

    int Adapter::GetLogCount() const {
        return _logCount;
    }

}
