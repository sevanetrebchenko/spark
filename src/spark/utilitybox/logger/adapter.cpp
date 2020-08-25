
#include <spark/utilitybox/logger/adapter/adapter.h> // Adapter

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
