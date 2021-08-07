
#include "spark/utilitybox/logger/logger.h"

namespace Spark::UtilityBox {

    Logger::Logger() : processingBufferSize_(64u),
                       processingBuffer_(new char[processingBufferSize_]),
                       logCounter_(0),
                       asynchronousInterval_(std::chrono::milliseconds(500)),
                       distributeMessages_(true),
                       printingBufferLocation_(&buffer1_)
                       {
        distributingThread_ = std::move(std::thread(&Logger::DistributeMessages, this));
    }


    Logger::~Logger() {
        // Wait for both buffers to be completely empty.
        while (!buffer1_.empty() || !buffer2_.empty()) {
            std::this_thread::yield();
        }

        // Shutdown worker thread.
        distributeMessages_ = false;
        distributingThread_.join();

        for (IAdapter* adapter : adapters_) {
            delete adapter;
        }
    }

    void Logger::LogMessage(const std::string &filename, const std::string &function, int lineNumber, LogSeverity messageSeverity, TimeStamp timeStamp, const char *formatString, ...) {
        // Process variadic log message.
        std::va_list argsList;
        va_start(argsList, formatString);
        std::string message = ProcessFormatString(formatString, argsList);
        va_end(argsList);

        // Construct log record.
        (*printingBufferLocation_).emplace(LogRecord {filename, function, lineNumber, messageSeverity, timeStamp, message } );
    }

    void Logger::AddAdapter(IAdapter *adapter) {
        if (!adapter) {
            return;
        }

        // Make sure adapter doesn't already exist.
        for (IAdapter* temp : adapters_) {
            if (temp == adapter) {
                LogError("Failed to add adapter - adapter '%s' has already been registered with the logging system.", adapter->GetName().c_str());
                return;
            }
        }

        adapters_.emplace_back(adapter);
    }

    void Logger::RemoveAdapter(IAdapter *adapter) {
        // Make sure adapter doesn't already exist.
        if (!adapter) {
            return;
        }

        for (auto adapterIter = adapters_.begin(); adapterIter != adapters_.end(); ++adapterIter) {
            if (*adapterIter == adapter) {
                adapters_.erase(adapterIter);
                return;
            }
        }

        LogError("Failed to remove adapter - adapter '%s' has not been registered with the logging system.", adapter->GetName().c_str());
    }

    std::string Logger::ProcessFormatString(const char *formatString, std::va_list argsList) {
        unsigned currentBufferSize = processingBufferSize_;

        // Copy args list to not modify passed parameters (yet).
        std::va_list argsCopy;
        va_copy(argsCopy, argsList);

        // If size of the buffer is zero, nothing is written and buffer may be a null pointer, however the return
        // value (number of bytes that would be written not including the null terminator) is still calculated and returned.
        int writeResult = vsnprintf(nullptr, 0, formatString, argsCopy);

        // If buffer size is equal to write result, there will not be space for the null terminator for the string.
        // Multiple buffer size by two to adequately house string in a new buffer.
        while (processingBufferSize_ <= writeResult) {
            processingBufferSize_ *= 2;
        }

        if (currentBufferSize != processingBufferSize_) {
            delete [] processingBuffer_;
            processingBuffer_ = new char[processingBufferSize_];
        }

        vsnprintf(processingBuffer_, processingBufferSize_, formatString, argsList);

        return std::string(processingBuffer_); // No need to wipe, next buffer will be overwritten.
    }

    void Logger::DistributeMessages() {
        while (distributeMessages_) {
            PrintingBuffer& buffer = SwitchBuffers();

            while (!buffer.empty()) {
                static std::vector<std::string> messages;

                const LogRecord& data = buffer.front();

                for (IAdapter* adapter : adapters_) {
                    const AdapterConfiguration& adapterConfiguration = adapter->GetAdapterConfiguration();

                    if (data.messageSeverity >= adapterConfiguration.severityCutoff) {
                        messages = std::move(ProcessMessage(adapterConfiguration, data));
                        adapter->OutputMessages(messages);
                    }
                }

                buffer.pop();
            }

            std::this_thread::sleep_for(asynchronousInterval_);
        }
    }

    Logger::PrintingBuffer& Logger::SwitchBuffers() {
        bufferMutex_.lock();
        PrintingBuffer& buffer = *printingBufferLocation_;

        if (printingBufferLocation_ == &(buffer1_)) {
            printingBufferLocation_ = &buffer2_;
        }
        else {
            printingBufferLocation_ = &buffer1_;
        }

        bufferMutex_.unlock();

        return buffer;
    }


    std::vector<std::string> Logger::ProcessMessage(const AdapterConfiguration& adapterConfiguration, const LogRecord& logRecord) {
        static std::vector<std::string> processedMessages;
        static std::stringstream format;

        int currentCharacterCount = 0;
        for (const char* traversalIndex = adapterConfiguration.globalFormattingString.c_str(); *traversalIndex != '\0'; ++traversalIndex) {
            // Get to the character after the next '%'.
            while (*traversalIndex != '%') {
                // Reached the end of the processing string.
                if (*traversalIndex == '\0') {
                    return std::move(processedMessages);
                }

                // Emplace finished sentence and clear formatting stringstream.
                if (*traversalIndex == '\n') {
                    format << std::endl;
                    processedMessages.emplace_back(format.str());
                    format.str(std::string(""));
                    currentCharacterCount = 0;
                }
                else if (*traversalIndex == '\t') {
                    format << *traversalIndex;
                    currentCharacterCount += 4;
                }
                else {
                    format << *traversalIndex;
                    ++currentCharacterCount;
                }

                ++traversalIndex;
            }
            ++traversalIndex;

            std::string appendedElement;

            switch (*traversalIndex) {
                // Timestamp
                case 't':
                    appendedElement = FormatTimestamp(logRecord.timeStamp, adapterConfiguration.timestampFormattingString);
                    format << appendedElement;
                    currentCharacterCount += appendedElement.length();
                    break;
                // Log count
                case 'c':
                    appendedElement = FormatLogCount();
                    format << appendedElement;
                    currentCharacterCount += appendedElement.length();
                    break;
                // Calendar date
                case 'd':
                    appendedElement = FormatCalendarInformation(adapterConfiguration.calendarFormatString);
                    format << appendedElement;
                    currentCharacterCount += appendedElement.length();
                    break;
                // Message severity.
                case 's':
                    appendedElement = FormatSeverity(logRecord.messageSeverity);
                    format << appendedElement;
                    currentCharacterCount += appendedElement.length();
                    break;
                // File, function, line number.
                case 'f':
                    appendedElement = FormatLocation(logRecord.filename, logRecord.function, logRecord.lineNumber, adapterConfiguration.locationFormatString);
                    format << appendedElement;
                    currentCharacterCount += appendedElement.length();
                    break;
                // Separating line.
                case 'x':
                    appendedElement = FormatSeparator(adapterConfiguration.separatorLength);
                    format << appendedElement;
                    currentCharacterCount += appendedElement.length();
                    break;
                // Message
                case 'm':
                    for (auto& string : FormatMessage(logRecord.message, currentCharacterCount, adapterConfiguration.wrapMessage, adapterConfiguration.wrapLimit)) {
                        format << string;
                    }
                    break;
                // Unknown character.
                default:
                    format << *traversalIndex;
            }
        }

        return std::move(processedMessages);
    }

    std::vector<std::string> Logger::FormatMessage(const std::string &message, int characterStart, bool wrap, int wrapLimit) {
        static std::vector<std::string> formattedStrings;
        static std::stringstream format;

        if (wrap) {
            int availableMessageSpace = wrapLimit - characterStart;

            // Message cannot fit on one line, tokenize.
            if (message.length() > (availableMessageSpace)) {
                unsigned length = 0;                // Current length of the built string.
                bool firstLine = true;              // First line needs no aligning spaces, subsequent lines do.
                std::vector<std::string> tokens;    // Token storage.
                std::stringstream parser(message);  // Used to parse the message into tokens.
                std::string storage;                // Current token.

                // Tokenize the string by spaces.
                while (getline(parser, storage, ' ')) {
                    // Delimiter gets rid of the space, we must append 1 to correctly count the space as a character.
                    if ((length + storage.length() + 1) > (availableMessageSpace)) {

                        // First line has preceding information, don't append any extra characters to the beginning.
                        // Additional lines will need to be aligned.
                        if (!firstLine) {
                            format << std::setw(characterStart) << std::setfill(' ') << "";
                        }

                        firstLine = false;

                        // Include spaces between words.
                        for (auto &token : tokens) {
                            format << token << ' ';
                        }

                        // Reset.
                        format << std::endl;
                        formattedStrings.emplace_back(format.str());
                        format.str(std::string());
                        tokens.clear();
                        length = 0;
                    }

                    // String hasn't reached limit yet, append token to storage (bin string).
                    length += storage.length() + 1;
                    tokens.emplace_back(std::move(storage));
                }

                // Add all remaining tokens after the whole string has been parsed.
                if (!tokens.empty()) {
                    format << std::setw(characterStart) << std::setfill(' ') << "";

                    for (auto &token : tokens) {
                        format << token << ' ';
                    }

                    formattedStrings.emplace_back(format.str());
                    format.str(std::string());
                }
            }

            // Message can fit on one line, no need to tokenize.
            else {
                formattedStrings.emplace_back(message);
            }
        }
        else {
            // Don't tokenize.
            formattedStrings.emplace_back(message);
        }

        return std::move(formattedStrings);
    }

    std::string Logger::FormatSeverity(const LogSeverity &messageSeverity) {
        static std::stringstream format;

        switch (messageSeverity) {
            case LogSeverity::DEBUG:
                format << "  DEBUG  ";
                break;
            case LogSeverity::WARNING:
                format << " WARNING ";
                break;
            case LogSeverity::ERROR:
                format << "  ERROR  ";
                break;
        }

        std::string formattedMessage = format.str();
        format.str(std::string("")); // Clear format.
        return std::move(formattedMessage);
    }

    std::string Logger::FormatTimestamp(const TimeStamp &timeStamp, const std::string& formatString) {
        static std::stringstream format;
        for (const char* traversalIndex = formatString.c_str(); *traversalIndex != '\0'; ++traversalIndex) {
            // Get to the character after the next '%'.
            while (*traversalIndex != '%') {
                // Reached the end of the string.
                if (*traversalIndex == '\0') {
                    std::string formattedMessage = format.str();
                    format.str(std::string("")); // Clear format.
                    return std::move(formattedMessage);
                }

                format << *traversalIndex++;
            }
            ++traversalIndex;

            switch (*traversalIndex) {
                // Minutes.
                case 'm':
                    format << std::setfill('0') << std::setw(3);
                    format << timeStamp.GetMinutes();
                    break;
                // Seconds.
                case 's':
                    format << std::setfill('0') << std::setw(2);
                    format << timeStamp.GetSeconds();
                    break;
                // Milliseconds.
                case 'l':
                    format << std::setfill('0') << std::setw(4);
                    format << timeStamp.GetMillis();
                    break;
                default:
                    format << *traversalIndex;
            }
        }

        std::string formattedMessage = format.str();
        format.str(std::string("")); // Clear format.
        return std::move(formattedMessage);
    }

    std::string Logger::FormatLocation(const std::string &file, const std::string &function, int lineNumber, const std::string& formatString) {
        static std::stringstream format;
        for (const char* traversalIndex = formatString.c_str(); *traversalIndex != '\0'; ++traversalIndex) {
            // Get to the character after the next '%'.
            while (*traversalIndex != '%') {
                // Reached the end of the string.
                if (*traversalIndex == '\0') {
                    std::string formattedMessage = format.str();
                    format.str(std::string("")); // Clear format.
                    return std::move(formattedMessage);
                }

                format << *traversalIndex++;
            }
            ++traversalIndex;

            switch (*traversalIndex) {
                // File.
                case 'f':
                    format << file;
                    break;
                // Function.
                case 'u':
                    format << function;
                    break;
                // Line number.
                case 'l':
                    format << lineNumber;
                    break;
                default:
                    format << *traversalIndex;
            }
        }

        std::string formattedMessage = format.str();
        format.str(std::string("")); // Clear format.
        return std::move(formattedMessage);
    }

    std::string Logger::FormatCalendarInformation(const std::string& formatString) {
        static std::time_t _calendarTime = std::time(nullptr);  // Gets filled with the current time when formatting calendar buffer.
        static unsigned _calendarBufferSize = 64u;                    // Size of the calendar buffer.
        static char* _calendarBuffer = new char[_calendarBufferSize]; // Calendar buffer.

        while(!std::strftime(_calendarBuffer, _calendarBufferSize, formatString.c_str(), std::localtime(&_calendarTime))) {
            _calendarBufferSize *= 2;
            delete[] _calendarBuffer;
            _calendarBuffer = new char[_calendarBufferSize];
        }

        // Reset to get the most updated calendar time.
        _calendarTime = std::time(nullptr);

        return _calendarBuffer;
    }

    std::string Logger::FormatLogCount() {
        static std::stringstream format;

        format << std::setfill('0') << std::setw(4);
        format << logCounter_ / 1000;

        format << ' ';

        format << std::setfill('0') << std::setw(4);
        format << logCounter_ % 1000;

        std::string formattedMessage = format.str();
        format.str(std::string("")); // Clear format.
        return std::move(formattedMessage);
    }

    std::string Logger::FormatSeparator(unsigned separatorLength) {
        static std::stringstream format;
        format << std::setw(separatorLength) << std::setfill('-') << "";

        std::string formattedMessage = format.str();
        format.str(std::string("")); // Clear format.
        return std::move(formattedMessage);
    }

}
