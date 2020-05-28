
#include "adapter.h"
#include "../exception.h"
#include "adapter_defines.h"
#include <utility>
#include <vector>
#include <sstream>
#include <iomanip>
#include "logger.h"


namespace UtilityBox {
    namespace Logger {
        class Adapter::AdapterData {
            public:
                AdapterData();
                ~AdapterData();
                std::vector<std::string> FormatMessage(const std::string& message, int timestampLength, unsigned lineLength);
                std::string FormatCalendarBuffer(std::string formatString);
                std::string FormatLogCounterBuffer(const unsigned& logCounter);
                std::string FormatMessageSeverityBuffer(const LogMessageSeverity& messageSeverity);
                std::string FormatMessageTimestampBuffer(const Timing::TimeStamp& timestamp);
                std::string FormatLine(const int& lineLength);
#ifdef DEBUG_MESSAGES
                std::string FormatDebugInformation(const LogMessage::DBG_LOG_RECORD& debugLogRecord);
#endif
            private:
                void UpdateCalendarBuffer();
                std::stringstream _format;

                // message
                std::vector<std::string> _formattedMessages;

                // calendar
                std::string _calendarFormat;
                std::time_t _calendarTime;
                unsigned _calendarBufferSize;
                char* _calendarBuffer;
        };

        Adapter::AdapterData::AdapterData() : _calendarTime(std::time(nullptr)), _calendarBufferSize(64u) {
            _calendarBuffer = new(std::nothrow) char[_calendarBufferSize];
            // todo: check
        }

        void Adapter::AdapterData:: UpdateCalendarBuffer() {
            while(!std::strftime(_calendarBuffer, _calendarBufferSize, _calendarFormat.c_str(), std::localtime(&_calendarTime))) {
                _calendarBufferSize *= 2;
                delete[] _calendarBuffer;
                _calendarBuffer = new (std::nothrow) char[_calendarBufferSize];
                // todo: check
            }
        }

        Adapter::AdapterData::~AdapterData() {
            delete[] _calendarBuffer;
        }

        std::vector<std::string> Adapter::AdapterData::FormatMessage(const std::string& message, int timestampLength, unsigned lineLength) {
            if (message.length() > lineLength) {
                // tokenize the string
                unsigned length = 0;
                bool firstLine = true;
                std::vector<std::string> tokens;
                std::stringstream parser(message);
                std::string storage;

                while (getline(parser, storage, ' ')) {
                    // delimiter gets rid of the space, we must append 1 to count the space as a character
                    if ((length + storage.length() + 1) > lineLength) {
                        if (!firstLine) {
                            for (int i = 0; i < timestampLength; ++i) {
                                _format << ' ';
                            }
                        }

                        for (auto &token : tokens) {
                            _format << token << ' ';
                        }

                        _formattedMessages.emplace_back(_format.str());
                        _format.str(std::string());
                        tokens.clear();
                        firstLine = false;
                        length = 0;
                    }

                    length += storage.length() + 1;
                    tokens.emplace_back(std::move(storage));
                }

                if (!tokens.empty()) {
                    for (int i = 0; i < timestampLength; ++i) {
                        _format << ' ';
                    }

                    for (auto &token : tokens) {
                        _format << token << ' ';
                    }

                    _formattedMessages.emplace_back(_format.str());
                    _format.str(std::string());
                }
            }
            else {
                _formattedMessages.emplace_back(message);
            }

            std::vector<std::string> toReturn = std::move(_formattedMessages);

            return toReturn;
        }

        std::string Adapter::AdapterData::FormatCalendarBuffer(std::string formatString) {
            if (_calendarFormat != formatString) {
                _calendarFormat = std::move(formatString);
                UpdateCalendarBuffer();
            }

            return _calendarBuffer;
        }

        std::string Adapter::AdapterData::FormatLogCounterBuffer(const unsigned &logCounter) {
            _format << '[';
            _format << std::setfill('0') << std::setw(4);
            _format << logCounter / 1000;

            _format << ' ';

            _format << std::setfill('0') << std::setw(4);
            _format << logCounter % 1000;
            _format << ']';

            std::string logCounterString = _format.str();
            _format.str(std::string());

            return logCounterString;
        }

       std::string Adapter::AdapterData::FormatMessageSeverityBuffer(const LogMessageSeverity &messageSeverity) {
            _format << '[';

            // [ DEBUG ]
            // [ WARNING ]
            // [ SEVERE ]
            // [ UNKNOWN ]
            switch (messageSeverity) {
                case LogMessageSeverity::DEBUG:
                    _format << " DEBUG ";
                    break;
                case LogMessageSeverity::WARNING:
                    _format << " WARNING ";
                    break;
                case LogMessageSeverity::SEVERE:
                    _format << " SEVERE ";
                    break;
                default:
                    _format << " UNKNOWN ";
                    break;
            }

            _format << ']';

           std::string messageSeverityString = _format.str();
           _format.str(std::string());

           return messageSeverityString;
        }

        std::string Adapter::AdapterData::FormatMessageTimestampBuffer(const Timing::TimeStamp &timestamp) {
            // timestamp format: [000m 00s 0000ms]
            _format << '[';

            // minutes
            _format << std::setfill('0') << std::setw(3);
            _format << timestamp.GetMinutes();
            _format << 'm';

            _format << ' ';

            // seconds
            _format << std::setfill('0') << std::setw(2);
            _format << timestamp.GetSeconds();
            _format << 's';

            _format << ' ';

            // milliseconds
            _format << std::setfill('0') << std::setw(4);
            _format << timestamp.GetMillis();
            _format << "ms";

            _format << ']';

            std::string timestampString = _format.str();
            _format.str(std::string());

            return timestampString;
        }

        std::string Adapter::AdapterData::FormatLine(const int &lineLength) {
            _format << std::setw(lineLength) << std::setfill('-') << "";

            std::string line = _format.str();
            _format.str(std::string());

            return line;
        }

#ifdef DEBUG_MESSAGES
        std::string Adapter::AdapterData::FormatDebugInformation(const DBG_LOG_MESSAGE::DBG_LOG_RECORD &debugLogRecord) {
            _format << TAB_SPACE << TAB_SPACE << ": supplied from (" << debugLogRecord._fileName << ", " << debugLogRecord._functionName << ':' << debugLogRecord._lineNumber << ')';
            std::string debugInformationString = _format.str();
            _format.str(std::string());

            return debugInformationString;
        }
#endif

        Adapter::Adapter(std::string &&name) : _adapterName(std::move(name)), _logCount(0), _config(), _data(std::move(std::make_unique<AdapterData>())) {
        }

        AdapterConfiguration &Adapter::GetConfiguration() {
            return _config;
        }

        std::vector<std::string> Adapter::FormatMessage(const std::string& message, int timestampLength, unsigned lineLength) {
            return _data->FormatMessage(message, timestampLength, lineLength);
        }

        std::string Adapter::FormatCalendarInformation() {
            return _data->FormatCalendarBuffer(_config.calendarFormat);
        }

        std::string Adapter::FormatLogCounter() {
            return _data->FormatLogCounterBuffer(_logCount);
        }

        std::string Adapter::FormatSeverity(LogMessageSeverity messageSeverity) {
            return _data->FormatMessageSeverityBuffer(messageSeverity);
        }

        std::string Adapter::FormatTimestamp(const Timing::TimeStamp &timeStamp) {
            return _data->FormatMessageTimestampBuffer(timeStamp);
        }

        std::string Adapter::FormatDebugInformation(const DBG_LOG_MESSAGE::DBG_LOG_RECORD &debugLogRecord) {
            return _data->FormatDebugInformation(debugLogRecord);
        }

        std::string Adapter::FormatLine(const int& lineLength) {
            return _data->FormatLine(lineLength);
        }

        Adapter::~Adapter() {
            _data.reset();
        }

        const std::string &Adapter::GetName() {
            return _adapterName;
        }

        // File adapter begin
        FileAdapter::FileAdapter(std::string &&name, std::string &&filename) : Adapter(std::move(name)) {
            if (filename.empty()) {
                _filename = "temp-log.txt";
            }
            _logger.open(_filename, std::ios_base::out | std::ios_base::trunc);
            // assert opening of file
            LogInitializationMessage();
        }

        FileAdapter::~FileAdapter() {
            _formattedMessages.clear();

            if (_logger.is_open()) {
                _logger.close();
            }
        }

        void FileAdapter::OutputMessage() {
            for (auto& message : _formattedMessages) {
                _logger << message;
            }

            // add gaps between consecutive messages
            _logger << "\n";
            _formattedMessages.clear();
        }

        void FileAdapter::ProcessMessage(void *messageAddress) {
            // format:
            // [0000 0000] | DAY 00, MONTH 0000 | [ SEVERITY ]
            //      [000m 00s 0000ms] - Message goes here
            //          : supplied from (filename, function:000)

            // format message header
            const LogMessageSeverity& messageSeverity = LoggingHub::GetInstance().GetMessageSeverity(messageAddress);
            if (messageSeverity >= _config.severityCutoff) {
                const std::vector<LogMessage::LogRecord>& messageRecords = LoggingHub::GetInstance().GetLogRecords(messageAddress);
                // get timestamp length
                _format << TAB_SPACE << FormatTimestamp(messageRecords.at(0)._timestamp) << " - ";
                int timestampLength = _format.str().length();
                _format.str(std::string());

                _format << FormatLine(timestampLength + _config.messageWrapLimit) << std::endl;
                _formattedMessages.emplace_back(_format.str());
                _format.str(std::string());

                _format << FormatLogCounter() << " | " << FormatCalendarInformation() << " | " << FormatSeverity(messageSeverity) << std::endl;
                _formattedMessages.emplace_back(_format.str());
                _format.str(std::string());


                _format << TAB_SPACE << "Logged through system: " << LoggingHub::GetInstance().GetThroughLoggingSystem(messageAddress) << std::endl;
                _formattedMessages.emplace_back(_format.str());
                _format.str(std::string());

                _format << FormatLine(timestampLength + _config.messageWrapLimit) << std::endl;
                _formattedMessages.emplace_back(_format.str());
                _format.str(std::string());

                for (auto& logRecord : messageRecords) {
                    _format << TAB_SPACE << FormatTimestamp(logRecord._timestamp) << " - ";
                    for (std::string& message : FormatMessage(logRecord._message, timestampLength, _config.messageWrapLimit)) {
                        _format << message << std::endl;
                    }

#ifdef DEBUG_MESSAGES
                    _format << FormatDebugInformation(logRecord._calleeInformation) << std::endl;
#endif
                    _format << '\n';
                    _formattedMessages.emplace_back(_format.str());
                    _format.str(std::string());
                }

                ++_logCount;
            }
        }

        void FileAdapter::LogInitializationMessage() {
            std::string calendarDate = FormatCalendarInformation();
            _format << calendarDate << " : Logging file '" << _filename << "' opened successfully. \n" << "Adapter name: " << _adapterName << ".\n" << std::endl;
            _formattedMessages.emplace_back(_format.str());
            _format.str(std::string());
            ++_logCount;
        }
    }
}
