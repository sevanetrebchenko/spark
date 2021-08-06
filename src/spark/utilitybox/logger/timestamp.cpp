
#include "spark/utilitybox/logger/timestamp.h"

namespace Spark::UtilityBox {

    void TimeStamp::Init() {
        initTime_ = std::chrono::high_resolution_clock::now();
    }

    TimeStamp::TimeStamp() : raw_(std::chrono::high_resolution_clock::now()) {
        Calculate();
    }

    TimeStamp::TimeStamp(const timepoint &time) : raw_(time) {
        Calculate();
    }

    // Destructor.
    TimeStamp::~TimeStamp() {
    }

    // Get only the millisecond portion of this timestamp.
    unsigned TimeStamp::GetMillis() const {
        return milliseconds_;
    }

    // Get only the second portion of this timestamp.
    unsigned TimeStamp::GetSeconds() const {
        return seconds_;
    }

    // Get only the minute portion of this timestamp.
    unsigned TimeStamp::GetMinutes() const {
        return minutes_;
    }

        static std::stringstream format;

        // Append minutes into stream.
        format << std::setfill('0') << std::setw(3);
        format << stamp.minutes_ << "m ";

        // Append seconds into stream.
        format << std::setfill('0') << std::setw(2);
        format << stamp.seconds_ << "s ";

        // Append milliseconds into stream.
        format << std::setfill('0') << std::setw(4);
        format << stamp.milliseconds_ << "ms ";

        os << format.str();
        format.str(std::string());
        return os;
    }

    void TimeStamp::Calculate() {
        // Get the total timestamp time in seconds.
        double elapsedSeconds = (std::chrono::duration_cast<std::chrono::duration<double>>(raw_ - initTime_)).count();
        double whole = std::floor(elapsedSeconds);
        double fraction = (elapsedSeconds - whole);

        milliseconds_ = ((int)(fraction * 1000)) % 1000;
        seconds_ = ((int)whole) % 60;
        minutes_ = ((int)(whole / 60.f));
    }

}
