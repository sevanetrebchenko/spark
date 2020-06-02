
#include "adapter.h"
#include "../exception.h"
#include "../assert_dev.h"
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
                std::vector<std::string>&& FormatMessage(const std::string& message, int timestampLength, int lineLength);
                std::string FormatCalendarBuffer(std::string formatString);
                std::string FormatLogCounterBuffer(const unsigned& logCounter);
                std::string FormatMessageSeverityBuffer(const LogMessageSeverity& messageSeverity);
                std::string FormatMessageTimestampBuffer(const Timing::TimeStamp& timestamp);
                std::string FormatLine(const int& lineLength);
#ifdef DEBUG_MESSAGES
                std::string FormatDebugInformation(const LogMessage::DBG_LOG_RECORD& debugLogRecord);
#endif
            private:
                void UpdateCalendarBuffer(std::string&& formatString);
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
            ASSERT(ASSERT_LEVEL_FATAL, _calendarBuffer != nullptr, "Operation new failed to allocate adapter calendar processing buffer - program is out of memory.");
        }

        void Adapter::AdapterData:: UpdateCalendarBuffer(std::string&& formatString) {
            while(!std::strftime(_calendarBuffer, _calendarBufferSize, formatString.c_str(), std::localtime(&_calendarTime))) {
                _calendarBufferSize *= 2;
                delete[] _calendarBuffer;
                _calendarBuffer = new (std::nothrow) char[_calendarBufferSize];
                ASSERT(ASSERT_LEVEL_FATAL, _calendarBuffer != nullptr, "Operation new failed to re-allocate adapter calendar processing buffer - program is out of memory.");
            }
        }

        Adapter::AdapterData::~AdapterData() {
            delete[] _calendarBuffer;
        }

        std::vector<std::string>&& Adapter::AdapterData::FormatMessage(const std::string& message, int timestampLength, int lineLength) {
            if (message.length() > (lineLength - timestampLength)) {
                // tokenize the string
                unsigned length = 0;
                bool firstLine = true;
                std::vector<std::string> tokens;
                std::stringstream parser(message);
                std::string storage;

                while (getline(parser, storage, ' ')) {
                    // delimiter gets rid of the space, we must append 1 to count the space as a character
                    if ((length + storage.length() + 1) > (lineLength - timestampLength)) {

                        // first line has timestamp, don't append any extra characters
                        if (!firstLine) {
                            _format << std::setw(timestampLength) << std::setfill(' ') << "";
                        }

                        firstLine = false;

                        for (auto &token : tokens) {
                            _format << token << ' ';
                        }

                        _format << std::endl;
                        _formattedMessages.emplace_back(_format.str());
                        _format.str(std::string());
                        tokens.clear();
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

            return std::move(_formattedMessages);
        }

        std::string Adapter::AdapterData::FormatCalendarBuffer(std::string formatString) {
            UpdateCalendarBuffer(std::move(formatString));
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
            _format << ": supplied from (" << debugLogRecord._fileName << ", " << debugLogRecord._functionName << ':' << debugLogRecord._lineNumber << ')';
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
            return _data->FormatCalendarBuffer(_config.GetCalendarFormatString());
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

#ifdef DEBUG_MESSAGES
        std::string Adapter::FormatDebugInformation(const DBG_LOG_MESSAGE::DBG_LOG_RECORD &debugLogRecord) {
            return _data->FormatDebugInformation(debugLogRecord);
        }
#endif

        std::string Adapter::FormatLine(const int& lineLength) {
            return _data->FormatLine(lineLength);
        }

        Adapter::~Adapter() {
            _data.reset();
        }

        const std::string &Adapter::GetName() {
            return _adapterName;
        }

        void Adapter::ProcessMessage(void *messageAddress) {
            // make sure pointer was passed to a legitimate object to begin with
//            if (LoggingHub::GetInstance().VerifyDataPointer(messageAddress)) {
                const LogMessageSeverity& messageSeverity = LoggingHub::GetInstance().GetMessageSeverity(messageAddress);

                if (messageSeverity >= _config.GetMessageSeverityCutoff()) {
                    ++_logCount;

                    // format header
                    FormatHeader(messageAddress);

                    // format messages
                    FormatMessages(messageAddress);
                }
//            }
        }

        void Adapter::FormatHeader(void* messageAddress) {
            std::queue<HeaderFormatElement> headerFormat = _config.GetHeaderFormat();
            const LogMessageSeverity& messageSeverity = LoggingHub::GetInstance().GetMessageSeverity(messageAddress);
            const std::string& throughSystem = LoggingHub::GetInstance().GetThroughLoggingSystem(messageAddress);

            while (!headerFormat.empty()) {
                HeaderFormatElement& element = headerFormat.front();
                switch (element) {
                    // end message and place it into the formatted messages vector
                    case HeaderFormatElement::NEWLINE:
                        _format << std::endl;
                        break;

                    case HeaderFormatElement::TERMINATOR:
                        _formattedMessages.emplace_back(_format.str());
                        _format.str(std::string());
                        break;

                    // append the log count of the message through this adapter
                    case HeaderFormatElement::LOGCOUNT:
                        _format << FormatLogCounter();
                        break;

                    // append the calendar date with the specified calendar format string
                    case HeaderFormatElement::DATE:
                        _format << FormatCalendarInformation();
                        break;

                    // append the message's severity
                    case HeaderFormatElement::SEVERITY:
                        _format << FormatSeverity(messageSeverity);
                        break;

                    // append the logging system instance name through which this message was logged
                    case HeaderFormatElement::SYSTEM:
                        _format << "Logged through system: " << throughSystem;
                        break;

                    // append separating line (line of dashes specified by the configuration's message wrap limit field)
                    case HeaderFormatElement::SEPARATOR:
                        _format << FormatLine(_config.GetMessageWrapLimit());
                        break;

                    // insert a tab formatting character
                    case HeaderFormatElement::TAB:
                        _format << TAB_SPACE;
                        break;

                    // insert a bar formatting character
                    case HeaderFormatElement::BAR:
                        _format << " | ";
                        break;
                }

                headerFormat.pop();
            }
        }

        void Adapter::FormatMessages(void *messageAddress) {
            // format message
            const std::vector<LogMessage::LogRecord>& messageRecords = LoggingHub::GetInstance().GetLogRecords(messageAddress);
            for (auto& record : messageRecords) {
                // format on a per-message basis
                std::queue<MessageFormatElement> messageFormat = _config.GetMessageFormat();

                while (!messageFormat.empty()) {
                    MessageFormatElement& element = messageFormat.front();
                    switch (element) {
                        // end message and place it into the formatted messages vector
                        case MessageFormatElement::NEWLINE:
                            _format << std::endl;
                            break;

                        case MessageFormatElement::TERMINATOR:
                            _formattedMessages.emplace_back(_format.str());
                            _format.str(std::string());
                            break;

                        // append message timestamp
                        case MessageFormatElement::TIMESTAMP:
                            _format << FormatTimestamp(record._timestamp);
                            break;

                        // append message
                        case MessageFormatElement::MESSAGE:
                            // alignment so that messages get printed on the same line is the number of characters written
                            // before the start of the first line of the message
                            for (std::string& message : FormatMessage(record._message, _format.str().length(), _config.GetMessageWrapLimit())) {
//                                std::size_t tabPosition = message.find_first_of('\n');
//                                while (tabPosition != std::string::npos) {
//                                    message.insert(tabPosition + 1, TAB_SPACE);
//                                    tabPosition = message.find_first_of('\n', tabPosition + 1);
//                                }
                                _format << message;
                            }
                            break;

#ifdef DEBUG_MESSAGES
                        // append string detailing the debug information of the log message
                        case MessageFormatElement::DEBUGINFO:
                            _format << FormatDebugInformation(record._calleeInformation);
                            break;
#endif
                        // insert a tab formatting character
                        case MessageFormatElement::TAB:
                            _format << TAB_SPACE;
                            break;

                        // insert a dash formatting character
                        case MessageFormatElement::DASH:
                            _format << " - ";
                            break;

                        // insert a bar formatting character
                        case MessageFormatElement::BAR:
                            _format << " | ";
                            break;

                        case MessageFormatElement::ELLIPSIS:
                            _format << "...";
                            break;
                    }

                    messageFormat.pop();
                }
            }
        }

        void Adapter::OutputErrorMessage(std::queue<std::string>&& processedErrorMessages) {
            while (!processedErrorMessages.empty()) {
                _formattedMessages.emplace_back(processedErrorMessages.front());
                processedErrorMessages.pop();
            }
            OutputMessage();
        }
    }
}
