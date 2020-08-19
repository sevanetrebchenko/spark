
#include <utilitybox/logger/adapter_config.h> // AdapterConfiguration

namespace Spark::UtilityBox::Logger {
    //------------------------------------------------------------------------------------------------------------------
    // ADAPTER CONFIGURATION DATA
    //------------------------------------------------------------------------------------------------------------------
    class AdapterConfiguration::AdapterConfigurationData {
        public:
            explicit AdapterConfigurationData(const char* adapterName);
            ~AdapterConfigurationData() = default;

            _NODISCARD_ const char* GetAdapterName() const;
            _NODISCARD_ int GetMessageWrapLimit() const;
            _NODISCARD_ LogMessageSeverity GetMessageSeverityCutoff() const;
            _NODISCARD_ const char* GetFormattingString() const;
            _NODISCARD_ const char* GetCalendarFormatString() const;
            _NODISCARD_ const char* GetTimestampFormattingString() const;

            void SetAdapterName(const char* adapterName);
            void SetMessageWrapLimit(int newLimit);
            void SetMessageSeverityCutoff(LogMessageSeverity newCutoff);
            void SetFormattingString(const char* formattingString);
            void SetCalendarFormatString(const char* newFormat);
            void SetTimestampFormattingString(const char* timestampFormat);

        private:
            int _messageWrapLimit;
            LogMessageSeverity _severityCutoff;
            const char* _adapterName;
            const char* _formattingString;
            const char* _timestampFormat;
            const char* _calendarFormat; // Default calendar format: [DAY_OF_WEEK, 00, MONTH 0000 - 00:00:00].
    };

    // Constructor. Establishes default formats for headers and messages.
    AdapterConfiguration::AdapterConfigurationData::AdapterConfigurationData(const char* adapterName) : _messageWrapLimit(100),
                                                                                                        _severityCutoff(LogMessageSeverity::DEBUG),
                                                                                                        _adapterName(adapterName),
                                                                                                        _formattingString(""),
                                                                                                        _timestampFormat("%m %s %l"),
                                                                                                        _calendarFormat("[%A %d, %B %Y - %H:%M:%S]")
    {
        // Nothing to do here.
    }

    const char *AdapterConfiguration::AdapterConfigurationData::GetAdapterName() const {
        return _adapterName;
    }

    int AdapterConfiguration::AdapterConfigurationData::GetMessageWrapLimit() const {
        return _messageWrapLimit;
    }

    LogMessageSeverity AdapterConfiguration::AdapterConfigurationData::GetMessageSeverityCutoff() const {
        return _severityCutoff;
    }

    const char *AdapterConfiguration::AdapterConfigurationData::GetFormattingString() const {
        return _formattingString;
    }

    const char *AdapterConfiguration::AdapterConfigurationData::GetCalendarFormatString() const {
        return _calendarFormat;
    }

    const char *AdapterConfiguration::AdapterConfigurationData::GetTimestampFormattingString() const {
        return _timestampFormat;
    }

    void AdapterConfiguration::AdapterConfigurationData::SetAdapterName(const char *adapterName) {
        _adapterName = adapterName;
    }

    void AdapterConfiguration::AdapterConfigurationData::SetMessageWrapLimit(int newLimit) {
        // Don't allow negative limits.
        if (newLimit > 0) {
            _messageWrapLimit = newLimit;
        }
    }

    void AdapterConfiguration::AdapterConfigurationData::SetMessageSeverityCutoff(LogMessageSeverity newCutoff) {
        _severityCutoff = newCutoff;
    }

    void AdapterConfiguration::AdapterConfigurationData::SetFormattingString(const char *formattingString) {
        _formattingString = formattingString;
    }

    void AdapterConfiguration::AdapterConfigurationData::SetCalendarFormatString(const char *newFormat) {
        _calendarFormat = newFormat;
    }

    void AdapterConfiguration::AdapterConfigurationData::SetTimestampFormattingString(const char *timestampFormat) {
        _timestampFormat = timestampFormat;
    }


    //------------------------------------------------------------------------------------------------------------------
    // ADAPTER CONFIGURATION
    //------------------------------------------------------------------------------------------------------------------
    // Establishes default formats for headers and messages.
    AdapterConfiguration::AdapterConfiguration(const char* adapterName) : _data(new AdapterConfigurationData(adapterName)) {
        // Nothing to do here.
    }

    // Cleans all header and message formats.
    AdapterConfiguration::~AdapterConfiguration() {
        delete _data;
    }

    const char *AdapterConfiguration::GetAdapterName() const {
        return _data->GetAdapterName();
    }

    int AdapterConfiguration::GetMessageWrapLimit() const {
        return _data->GetMessageWrapLimit();
    }

    LogMessageSeverity AdapterConfiguration::GetMessageSeverityCutoff() const {
        return _data->GetMessageSeverityCutoff();
    }

    const char *AdapterConfiguration::GetFormattingString() const {
        return _data->GetFormattingString();
    }

    const char* AdapterConfiguration::GetCalendarFormatString() const {
        return _data->GetCalendarFormatString();
    }

    const char *AdapterConfiguration::GetTimestampFormattingString() const {
        return _data->GetTimestampFormattingString();
    }

    void AdapterConfiguration::SetAdapterName(const char *adapterName) {
        _data->SetAdapterName(adapterName);
    }

    void AdapterConfiguration::SetMessageWrapLimit(int newLimit) {
        _data->SetMessageWrapLimit(newLimit);
    }

    void AdapterConfiguration::SetMessageSeverityCutoff(LogMessageSeverity newCutoff) {
        _data->SetMessageSeverityCutoff(newCutoff);
    }

    void AdapterConfiguration::SetFormattingString(const char *formattingString) {
        _data->SetFormattingString(formattingString);
    }

    void AdapterConfiguration::SetCalendarFormatString(const char* newFormat) {
        _data->SetCalendarFormatString(newFormat);
    }

    void AdapterConfiguration::SetTimestamppFormattingString(const char *timestampFormat) {
        _data->SetTimestampFormattingString(timestampFormat);
    }

}
