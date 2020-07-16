
#include "../include/logger.h"        // LogMessage
#include "../include/adapter.h"       // Adapter
#include "../../Tools/assert_dev.h" // Asserts
#include <vector>          // std::vector
#include <sstream>         // std::stringstream
#include <iomanip>         // std::setw, std::setfill

namespace UtilityBox::Logger {
    //------------------------------------------------------------------------------------------------------------------
    // ADAPTER DATA
    //------------------------------------------------------------------------------------------------------------------
    class Adapter::AdapterData {
        public:
            /**
             * Constructor. Allocates necessary buffers for processing messages.
             */
            AdapterData();

            /**
             * Destructor. Safely deallocates all memory used by Adapter Data helper functions.
             */
            ~AdapterData();

            /**
             * Creates a block of text by formatting the given message. Wraps message contents to a hard character
             * limit. Should the message not fit on a single line, additional lines are aligned with the start of
             * the first line. Messages are hard wrapped at the lineLength limit - if a word were to cause the
             * length of the line to go over, it is placed on the next line below.
             *
             * [<-----------timestampLength---------->]                                       [<--lineLength]
             *                                         This example message is being processed
             *                                          on numerous lines.
             * @param message         - Message to format.
             * @param timestampLength - How many characters to leave blank should the formatted message span more
             *                          than one line.
             * @param lineLength      - Hard limit on the number of characters per line.
             * @return Vector of formatted strings.
             */
            std::vector<std::string> DefaultMessageFormat(const std::string& message, int timestampLength, int lineLength);

            /**
             * Allocates a sufficiently large buffer to store the calendar information and processes the buffer using
             * the given calendar format string.
             * Default calendar format: [DAY_OF_WEEK, 00, MONTH 0000 - 00:00:00]
             * @param formatString - Format for the calendar string to have.
             * @return String containing formatted calendar information.
             */
            std::string DefaultCalendarFormat(const std::string& formatString);

            /**
             * Formats the current number of the message being logged through this adapter.
             * Default log count format: [0000 0000]
             * @param logCounter - Number of the message being logged.
             * @return String containing formatted log count.
             */
            std::string DefaultLogCountFormat(const int& logCounter);

            /**
             * Formats a given message severity.
             * Default message severity format: [ SEVERITY ]
             * @param messageSeverity - Severity of the message.
             * @return String containing the formatted severity.
             */
            std::string DefaultSeverityFormat(const LogMessageSeverity& messageSeverity);

            /**
             * Formats a given timestamp.
             * Default timestamp format: [000m 00s 0000ms]
             * @param timestamp - Timestamp to format.
             * @return String containing the formatted timestamp.
             */
            std::string DefaultTimestampFormat(const Timing::TimeStamp& timestamp);

            /**
             * Formats a line of dashes of a given length.
             * @param lineLength - Desired line length.
             * @return String containing the line.
             */
            std::string DefaultLineFormat(const int& lineLength);

        #ifdef DEBUG_MESSAGES
            /**
             * Formats log message debug information.
             * Default format:   : supplied from (FUNCTION, FILE_NAME:LINE_NUMBER)
             * @param debugLogRecord - Log message debug information to format.
             * @return String containing formatted debug information.
             */
            std::string DefaultDebugInformationFormat(const LogMessage::DBG_LOG_RECORD& debugLogRecord);
        #endif

        private:
            /**
             * Attemps to reallocate the calendar buffer until the buffer can adequately fit the desired calendar
             * information, given by the formatString.
             * @param formatString - Desired calendar buffer format.
             */
            void ConstructCalendarBuffer(const std::string& formatString);

            std::vector<std::string> _processedMessages; // Finished processed message storage for formatting
                                                         // multi-line messages.

            std::stringstream _format;    // Stringstream to use when formatting various components.
            std::time_t _calendarTime;    // Gets filled with the current time when formatting calendar buffer.
            unsigned _calendarBufferSize; // Size of the calendar buffer.
            char* _calendarBuffer;        // Calendar buffer.
    };

    // Constructor. Allocates necessary buffers for processing messages.
    Adapter::AdapterData::AdapterData() : _calendarTime(std::time(nullptr)),
                                          _calendarBufferSize(64u),
                                          _calendarBuffer(new char[_calendarBufferSize]) /* Throws exception, caught elsewhere. */ {
        // Nothing to do here.
    }

    // Attemps to reallocate the calendar buffer until the buffer can adequately fit the desired calendar information, given by the formatString.
    void Adapter::AdapterData::ConstructCalendarBuffer(const std::string& formatString) {
        // Return value is 0 and array contents are indeterminate if the buffer size is not large enough to hold the number of characters.
        // Continue to resize and reallocate the buffer (as strftime writes if it's large enough) until buffer holds valid contents.
        while(!std::strftime(_calendarBuffer, _calendarBufferSize, formatString.c_str(), std::localtime(&_calendarTime))) {
            _calendarBufferSize *= 2;
            delete[] _calendarBuffer;
            _calendarBuffer = new char[_calendarBufferSize]; // Throws exception, caught elsewhere.
        }
    }

    // Destructor. Safely deallocates all memory used by Adapter Data helper functions.
    Adapter::AdapterData::~AdapterData() {
        delete[] _calendarBuffer;
        _format.str(std::string());
        _processedMessages.clear();
    }

    // Creates a block of text by formatting the given message. Wraps message contents to a hard character limit.
    // Should the message not fit on a single line, additional lines are aligned with the start of the first line.
    std::vector<std::string> Adapter::AdapterData::DefaultMessageFormat(const std::string& message, int timestampLength, int lineLength) {
        // Message cannot fit on one line, tokenize.
        if (message.length() > (lineLength - timestampLength)) {
            unsigned length = 0;                // Current length of the built string.
            bool firstLine = true;              // First line needs no aligning spaces, subsequent lines do.
            std::vector<std::string> tokens;    // Token storage.
            std::stringstream parser(message);  // Used to parse the message into tokens.
            std::string storage;                // Current token.

            // Tokenize the string by spaces.
            while (getline(parser, storage, ' ')) {
                // Delimiter gets rid of the space, we must append 1 to correctly count the space as a character.
                if ((length + storage.length() + 1) > (lineLength - timestampLength)) {

                    // First line has preceeding information, don't append any extra characters to the beginning.
                    // Additional lines will need to be aligned.
                    if (!firstLine) {
                        _format << std::setw(timestampLength) << std::setfill(' ') << "";
                    }

                    firstLine = false;

                    // Include spaces between words.
                    for (auto &token : tokens) {
                        _format << token << ' ';
                    }

                    // Reset.
                    _format << std::endl;
                    _processedMessages.emplace_back(_format.str());
                    _format.str(std::string());
                    tokens.clear();
                    length = 0;
                }

                // String hasn't reached limit yet, append token to storage (bin string).
                length += storage.length() + 1;
                tokens.emplace_back(std::move(storage));
            }

            // Add all remaining tokens after the whole string has been parsed.
            if (!tokens.empty()) {
                for (int i = 0; i < timestampLength; ++i) {
                    _format << ' ';
                }

                for (auto &token : tokens) {
                    _format << token << ' ';
                }

                _processedMessages.emplace_back(_format.str());
                _format.str(std::string());
            }
        }

        // Message can fit on one line, no need to tokenize.
        else {
            _processedMessages.emplace_back(message);
        }

        return std::move(_processedMessages);
    }

    // Allocates a sufficiently large buffer to store the calendar information and processes the buffer using the given calendar format string.
    std::string Adapter::AdapterData::DefaultCalendarFormat(const std::string& formatString) {
        ConstructCalendarBuffer(formatString);
        return _calendarBuffer;
    }

    // Formats the current number of the message being logged through this adapter.
    std::string Adapter::AdapterData::DefaultLogCountFormat(const int& logCounter) {
        // Default log count format: [0000 0000]
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

    // Formats a given message severity.
   std::string Adapter::AdapterData::DefaultSeverityFormat(const LogMessageSeverity& messageSeverity) {
        _format << '[';

        // Default message severity format: [ SEVERITY ]
        // Potential options:
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

    // Formats a given timestamp.
    std::string Adapter::AdapterData::DefaultTimestampFormat(const Timing::TimeStamp& timestamp) {
        // Default timestamp format: [000m 00s 0000ms]
        _format << '[';

        // Format minutes.
        _format << std::setfill('0') << std::setw(3);
        _format << timestamp.GetMinutes();
        _format << 'm';

        _format << ' ';

        // Format seconds.
        _format << std::setfill('0') << std::setw(2);
        _format << timestamp.GetSeconds();
        _format << 's';

        _format << ' ';

        // Format milliseconds.
        _format << std::setfill('0') << std::setw(4);
        _format << timestamp.GetMillis();
        _format << "ms";

        _format << ']';

        std::string timestampString = _format.str();
        _format.str(std::string());

        return timestampString;
    }

    // Formats a line of dashes of a given length.
    std::string Adapter::AdapterData::DefaultLineFormat(const int& lineLength) {
        _format << std::setw(lineLength) << std::setfill('-') << "";

        std::string line = _format.str();
        _format.str(std::string());

        return line;
    }

#ifdef DEBUG_MESSAGES
    // Formats log message debug information.
    std::string Adapter::AdapterData::DefaultDebugInformationFormat(const DBG_LOG_MESSAGE::DBG_LOG_RECORD& debugLogRecord) {
        // Default format:   : supplied from (FUNCTION, FILE_NAME:LINE_NUMBER)
        _format << ": supplied from (" << debugLogRecord._fileName << ", " << debugLogRecord._functionName << ':' << debugLogRecord._lineNumber << ')';
        std::string debugInformationString = _format.str();
        _format.str(std::string());

        return debugInformationString;
    }
#endif


    //------------------------------------------------------------------------------------------------------------------
    // ADAPTER
    //------------------------------------------------------------------------------------------------------------------
    // Construct an adapter with default formatting with a given name.
    Adapter::Adapter(std::string&& name) : _adapterName(std::move(name)), _logCount(0), _config(), _data(new AdapterData()) {
        // Nothing to do here.
    }

    // Destructor.
    Adapter::~Adapter() {
        delete _data;
    }

    // Process a message given a void pointer provided by the LoggingHub and output formatted strings into the
    // formattedMessages queue to be outputted.
    void Adapter::ProcessMessage(void* messageAddress) {
        // Verify pointer was passed to a legitimate object to begin with.
        if (LoggingHub::GetInstance()->VerifyDataPointer(messageAddress)) {
            const LogMessageSeverity& messageSeverity = LoggingHub::GetInstance()->GetMessageSeverity(messageAddress);

            // Only log message if the adapter's cutoff allows it.
            if (messageSeverity >= _config.GetMessageSeverityCutoff()) {
                ConstructMessageHeader(messageAddress);
                ConstructMessageBody(messageAddress);
            }
        }
    }

    // Emplaces pre-processed error messages directly into the Adapter's store of formatted messages and directly outputs them using the OutputMessage function.
    void Adapter::OutputErrorMessage(std::queue<std::string> processedErrorMessages) {
        while (!processedErrorMessages.empty()) {
            _formattedMessages.emplace(processedErrorMessages.front());
            processedErrorMessages.pop();
        }
        OutputMessage();
    }

    // Get the configuration for this adapter.
    AdapterConfiguration& Adapter::GetConfiguration() {
        return _config;
    }

    // Get the name for this adapter.
    const std::string& Adapter::GetName() {
        return _adapterName;
    }

    // Get the current LogCount for this adapter.
    const int& Adapter::GetLogCount() const {
        return _logCount;
    }

    // Given the desired header format provided through the AdapterConfiguration and data accessed through the
    // LoggingHub with the messageAddress, define what is added to the output message with each header format element.
    void Adapter::ConstructMessageHeader(void* messageAddress) {
        // Header format is copied to use.
        std::queue<HeaderFormatElement> headerFormat = _config.GetHeaderFormat();
        const LogMessageSeverity& messageSeverity = LoggingHub::GetInstance()->GetMessageSeverity(messageAddress);
        const std::string& throughSystem = LoggingHub::GetInstance()->GetThroughLoggingSystem(messageAddress);

        while (!headerFormat.empty()) {
            HeaderFormatElement& element = headerFormat.front();

            switch (element) {
                // Terminate message and place it into the formatted messages vector.
                case HeaderFormatElement::NEWLINE:
                    _format << std::endl;
                    _formattedMessages.emplace(_format.str());
                    _format.str(std::string());
                    break;

                // Append the log count of the message.
                case HeaderFormatElement::LOGCOUNT:
                    _format << FormatLogCounter();
                    break;

                // Append calendar information.
                case HeaderFormatElement::DATE:
                    _format << FormatCalendarInformation();
                    break;

                // Append the message's severity.
                case HeaderFormatElement::SEVERITY:
                    _format << FormatSeverity(messageSeverity);
                    break;

                // Append the logging system instance name through which this message was logged.
                case HeaderFormatElement::SYSTEM:
                    _format << "Logged through system: " << throughSystem;
                    break;

                // Append dashed separating line.
                case HeaderFormatElement::SEPARATOR:
                    _format << FormatLine(_config.GetMessageWrapLimit());
                    break;

                // Append a tab formatting character.
                case HeaderFormatElement::TAB:
                    _format << TAB_SPACE;
                    break;

                // Append a dash formatting character.
                case HeaderFormatElement::DASH:
                    _format << " - ";
                    break;

                // Append a bar formatting character.
                case HeaderFormatElement::BAR:
                    _format << " | ";
                    break;

                // Append exception text.
                case HeaderFormatElement::EXCEPTION:
                    _format << "EXCEPTION";
                    break;
            }
            headerFormat.pop();
        }
    }

    // Given the desired message format provided through the AdapterConfiguration and data accessed through the
    // LoggingHub with the messageAddress, define what is added to the output message with each message format element.
    void Adapter::ConstructMessageBody(void* messageAddress) {
        const std::vector<LogMessage::LogRecord>& messageRecords = LoggingHub::GetInstance()->GetLogRecords(messageAddress);

        // Message format queue is applied on a per-message basis.
        for (auto& record : messageRecords) {
            // Message format is copied to use.
            std::queue<MessageFormatElement> messageFormat = _config.GetMessageFormat();

            while (!messageFormat.empty()) {
                MessageFormatElement &element = messageFormat.front();

                switch (element) {
                    // Terminate message and place it into the formatted messages vector.
                    case MessageFormatElement::NEWLINE:
                        _format << std::endl;
                        _formattedMessages.emplace(_format.str());
                        _format.str(std::string());
                        break;

                    // Append message timestamp.
                    case MessageFormatElement::TIMESTAMP:
                        _format << FormatTimestamp(record._timestamp);
                        break;

                    // Append message(s).
                    case MessageFormatElement::MESSAGE:
                        // Align multi-line messages so that additional messages are aligned to the start of the first message.
                        for (auto& processedMessage : FormatMessage(record._message, _format.str().length(), _config.GetMessageWrapLimit())) {
                            _format << processedMessage;
                        }
                        break;

                    // Append debug information of the log message.
                    case MessageFormatElement::DEBUGINFO:
#ifdef DEBUG_MESSAGES
                        _format << FormatDebugInformation(record._calleeInformation);
#endif
                        break;

                    // Append a tab formatting character.
                    case MessageFormatElement::TAB:
                        _format << TAB_SPACE;
                        break;

                    // Append a dash formatting character.
                    case MessageFormatElement::DASH:
                        _format << " - ";
                        break;

                    // Append a bar formatting character.
                    case MessageFormatElement::BAR:
                        _format << " | ";
                        break;
                }
                messageFormat.pop();
            }
        }
    }

    // Creates a block of text by formatting the given message. Wraps message contents to a hard character limit.
    // Should the message not fit on a single line, additional lines are aligned with the start of the first line.
    std::vector<std::string> Adapter::FormatMessage(const std::string& message, int characterStart, int characterEnd) {
        return _data->DefaultMessageFormat(message, characterStart, characterEnd);
    }

    // Formats a string to hold the calendar information based on the calendarFormat field in the adapter configuration.
    std::string Adapter::FormatCalendarInformation() {
        return _data->DefaultCalendarFormat(_config.GetCalendarFormatString());
    }

    // Format a counter representing the current message number through this adapter.
    std::string Adapter::FormatLogCounter() {
        return _data->DefaultLogCountFormat(_logCount);
    }

    // Format the severity of this message.
    std::string Adapter::FormatSeverity(const LogMessageSeverity& messageSeverity) {
        return _data->DefaultSeverityFormat(messageSeverity);
    }

    // Format the timestamp of this message.
    std::string Adapter::FormatTimestamp(const Timing::TimeStamp& timeStamp) {
        return _data->DefaultTimestampFormat(timeStamp);
    }

#ifdef DEBUG_MESSAGES
    // Format the debug information (file, function, and line number where Supply provided the message).
    std::string Adapter::FormatDebugInformation(const DBG_LOG_MESSAGE::DBG_LOG_RECORD &debugLogRecord) {
        return _data->DefaultDebugInformationFormat(debugLogRecord);
    }
#endif

    // Format a line of dashes of a given length.
    std::string Adapter::FormatLine(const int& lineLength) {
        return _data->DefaultLineFormat(lineLength);
    }
}
