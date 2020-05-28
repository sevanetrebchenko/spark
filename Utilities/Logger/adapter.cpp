
#include "adapter.h"
#include "../exception.h"
#include <utility>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstring>
#include "logger.h"

namespace UtilityBox {
    namespace Logger {
        class Adapter::AdapterData {
            public:
                AdapterData();
                ~AdapterData();
                std::vector<std::string>& FormatMessage(const std::string& message, int timestampLength, unsigned lineLength);
                const char* FormatCalendarBuffer(std::string formatString);
                const char* FormatLogCounterBuffer(const unsigned& logCounter);
                const char* FormatMessageSeverityBuffer(const LogMessageSeverity& messageSeverity);
                const char* FormatMessageTimestampBuffer(const Timing::TimeStamp& timestamp);

            private:
                void UpdateCalendarBuffer();
                void CheckBufferSize(char** buffer, unsigned &bufferSize, unsigned size);

                std::stringstream _format;

                // message
                std::vector<std::string> _formattedMessages;

                // calendar
                std::string _calendarFormat;
                std::time_t _calendarTime;
                unsigned _calendarBufferSize;
                char* _calendarBuffer;

                // log counter
                unsigned _logCounterBufferSize;
                char* _logCounterBuffer;

                // message severity
                unsigned _messageSeverityBufferSize;
                char* _messageSeverityBuffer;

                // timestamp
                unsigned _timestampBufferSize;
                char* _timestampBuffer;
        };

        Adapter::AdapterData::AdapterData() : _calendarTime(std::time(nullptr)), _calendarBufferSize(64u),
                                                                                       _logCounterBufferSize(16u),
                                                                                       _messageSeverityBufferSize(16u),
                                                                                       _timestampBufferSize(32u) {
            _calendarBuffer = new(std::nothrow) char[_calendarBufferSize];
            _logCounterBuffer = new(std::nothrow) char[_logCounterBufferSize];
            _messageSeverityBuffer = new(std::nothrow) char[_messageSeverityBufferSize];
            _timestampBuffer = new(std::nothrow) char[_timestampBufferSize];
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
            delete[] _logCounterBuffer;
            delete[] _messageSeverityBuffer;
            delete[] _timestampBuffer;
        }

        std::vector<std::string>& Adapter::AdapterData::FormatMessage(const std::string& message, int timestampLength, unsigned lineLength) {
            if (message.length() > lineLength) {
                // tokenize the string
                unsigned length = 0;
                bool firstLine = true;
                std::vector<std::string> tokens;
                std::stringstream parser(message);
                std::string storage;

                while (getline(parser, storage, ' ')) {
                    // delimiter gets rid of the space, we must append 1 to count the space as a character
                    length += storage.length() + 1;
                    tokens.emplace_back(std::move(storage));

                    if (length > lineLength) {
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

            return _formattedMessages;
        }

        const char *Adapter::AdapterData::FormatCalendarBuffer(std::string formatString) {
            if (_calendarFormat != formatString) {
                _calendarFormat = std::move(formatString);
                UpdateCalendarBuffer();
            }

            return _calendarBuffer;
        }

        const char *Adapter::AdapterData::FormatLogCounterBuffer(const unsigned &logCounter) {
            _format << '[';
            _format << std::setfill('0') << std::setw(4);
            _format << logCounter / 1000;

            _format << ' ';

            _format << std::setfill('0') << std::setw(4);
            _format << logCounter % 1000;
            _format << ']';

            // leave room for terminating null character
            CheckBufferSize(&_logCounterBuffer, _logCounterBufferSize, _format.str().length() + 1);
            strcpy(_logCounterBuffer, _format.str().c_str());

            // clear buffer
            _format.str(std::string());

            return _logCounterBuffer;
        }

        const char *Adapter::AdapterData::FormatMessageSeverityBuffer(const LogMessageSeverity &messageSeverity) {
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

            // leave room for terminating null character
            CheckBufferSize(&_messageSeverityBuffer, _messageSeverityBufferSize, _format.str().length() + 1);
            strcpy(_messageSeverityBuffer, _format.str().c_str());

            // clear buffer
            _format.str(std::string());

            return _messageSeverityBuffer;
        }

        const char *Adapter::AdapterData::FormatMessageTimestampBuffer(const Timing::TimeStamp &timestamp) {
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

            // leave room for null character
            CheckBufferSize(&_timestampBuffer, _timestampBufferSize, _format.str().length() + 1);
            strcpy(_timestampBuffer, _format.str().c_str());

            // clear buffer
            _format.str(std::string());

            return _timestampBuffer;
        }

        void Adapter::AdapterData::CheckBufferSize(char** buffer, unsigned &bufferSize, unsigned size) {
            while (size > bufferSize) {
                bufferSize *= 2;
            }

            delete[] *buffer;
            *buffer = new(std::nothrow) char[bufferSize];
        }

        Adapter::Adapter(std::string &&name) : _adapterName(std::move(name)), _logCount(0), _config(), _data(std::move(std::make_unique<AdapterData>())) {
        }

        AdapterConfiguration &Adapter::GetConfiguration() {
            return _config;
        }

        std::vector<std::string>& Adapter::FormatMessage(const std::string& message, int timestampLength, unsigned lineLength) {
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
        }

        void FileAdapter::ProcessMessage(void *messageAddress) {
            // format:
            // [0000 0000] | DAY 00, MONTH 0000 | [ SEVERITY ]
            //      [000m 00s 0000ms] - Message goes here
            //          : supplied from (filename, function:000)

            // format message header
            const LogMessageSeverity& messageSeverity = LoggingHub::GetInstance().GetMessageSeverity(messageAddress);
            if (messageSeverity >= _config.severityCutoff) {
                _format << FormatLogCounter() << " | " << FormatCalendarInformation() << " | " << FormatSeverity(messageSeverity) << std::endl;
                _formattedMessages.emplace_back(_format.str());
                _format.str(std::string());

                // format message body
                const std::vector<LogMessage::LogRecord>& messageRecords = LoggingHub::GetInstance().GetLogRecords(messageAddress);
                for (auto& message : messageRecords) {
                    _format << '\t' << FormatTimestamp(message._timestamp) << " - " << message._message << std::endl;
#ifdef DEBUG_MESSAGES
                    _format << "\t\t" << ": supplied from (" << message._calleeInformation._fileName << ", " << message._calleeInformation._functionName << ':' << message._calleeInformation._lineNumber << ')' << std::endl;
#endif
                    _formattedMessages.emplace_back(_format.str());
                    _format.str(std::string());
                }
                ++_logCount;
            }
        }

        void FileAdapter::LogInitializationMessage() {
            std::string calendarDate = FormatCalendarInformation();
            _format << calendarDate << " : Logging file '" << _filename << "' opened successfully. " << "Adapter name: " << _adapterName << ".\n" << std::endl;
            _formattedMessages.emplace_back(_format.str());
            _format.str(std::string());
            ++_logCount;
        }
    }
}
