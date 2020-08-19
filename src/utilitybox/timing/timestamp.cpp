
#include <spark_pch.h>                   // std::setfill, std::setw, std::floor
#include <utilitybox/timing/timestamp.h> // TimeStamp
#include <utilitybox/logger/logger.h>    // GetLoggingInitializationTime

namespace Spark::UtilityBox::Timing {
    //------------------------------------------------------------------------------------------------------------------
    // TIMESTAMP DATA
    //------------------------------------------------------------------------------------------------------------------
    struct TimeStamp::TimeStampData {
        /**
         * Construct the back-end for timestamp processing.
         * @param timestamp - Raw timestamp of the parent Timestamp's creation.
         */
        explicit TimeStampData(const std::chrono::time_point<std::chrono::high_resolution_clock>& timestamp);

        std::chrono::time_point<std::chrono::high_resolution_clock> _raw; // Raw timestamp object from std::chrono library
        unsigned _milliseconds; // Millisecond portion of the raw timestamp
        unsigned _seconds;      // Second portion of the raw timestamp
        unsigned _minutes;      // Minute portion of the raw timestamp
    };

    // Construct the back-end for timestamp processing.
    TimeStamp::TimeStampData::TimeStampData(const std::chrono::time_point<std::chrono::high_resolution_clock>& timestamp) : _raw(timestamp) {
        // Get the total timestamp time in seconds.
        double elapsedSeconds = (std::chrono::duration_cast<std::chrono::duration<double>>(_raw - Logger::LoggingHub::GetInstance()->GetInitializationTime())).count();

        // Get the fractional part of seconds
        double whole = std::floor(elapsedSeconds);
        double fraction = (elapsedSeconds - whole);

        // Get millisecond portion.
        _milliseconds = ((int)(fraction * 1000)) % 1000;

        // Get second portion.
        _seconds = ((int)whole) % 60;

        // Get minute portion.
        _minutes = ((int)(whole / 60.f));
    }


    //------------------------------------------------------------------------------------------------------------------
    // TIMESTAMP
    //------------------------------------------------------------------------------------------------------------------
    // Constructor for a timestamp. Uses std::chrono::high_resolution_clock::now() (time when constructor was called)
    // as the time record.
    TimeStamp::TimeStamp() : _data(new TimeStampData(std::chrono::high_resolution_clock::now())) {
        // Nothing to do here.
    }

    // Move constructor.
    TimeStamp::TimeStamp(TimeStamp&& other) noexcept {
        // Transfer data ownership to this.
        _data = other._data;
        _data->_raw = other._data->_raw;
        _data->_milliseconds = other._data->_milliseconds;
        _data->_seconds = other._data->_seconds;
        _data->_minutes = other._data->_minutes;

        // Set other to null.
        other._data = nullptr;
    }

    // Destructor.
    TimeStamp::~TimeStamp() {
        delete _data;
    }

    // Get only the millisecond portion of this timestamp.
    unsigned TimeStamp::GetMillis() const {
        return _data->_milliseconds;
    }

    // Get only the second portion of this timestamp.
    unsigned TimeStamp::GetSeconds() const {
        return _data->_seconds;
    }

    // Get only the minute portion of this timestamp.
    unsigned TimeStamp::GetMinutes() const {
        return _data->_minutes;
    }

    // Print this timestamp to the provided stream.
    std::ostream& operator<<(std::ostream& os, const TimeStamp& stamp) {
        // default format: 000m 00s 0000ms
        static std::stringstream format;

        // Append minutes into stream.
        format << std::setfill('0') << std::setw(3);
        format << stamp._data->_minutes << "m ";

        // Append seconds into stream.
        format << std::setfill('0') << std::setw(2);
        format << stamp._data->_seconds << "s ";

        // Append milliseconds into stream.
        format << std::setfill('0') << std::setw(4);
        format << stamp._data->_milliseconds << "ms ";

        os << format.str();
        format.str(std::string());
        return os;
    }
}
