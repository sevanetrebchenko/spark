
#include <spark/utilitybox/logger/adapter/adapter.h> // IAdapter

namespace Spark::UtilityBox::Logger {
    // Construct an adapter with default formatting with a given name.
    IAdapter::IAdapter(const char* adapterName) : _config(new AdapterConfiguration(adapterName)),
                                                  _logCount(0) {
        // Nothing to do here.
    }

    IAdapter::IAdapter(const std::string& adapterName) : _config(new AdapterConfiguration(adapterName.c_str())),
                                                         _logCount(0) {
        // Nothing to do here.
    }

    // Get the configuration for this adapter.
    AdapterConfiguration* IAdapter::GetConfiguration() const {
        return _config;
    }

    int IAdapter::GetLogCount() const {
        return _logCount;
    }



}
