
#include "spark/utilitybox/logger/timestamp.h"

namespace Spark::UtilityBox {

    TimeStamp::timepoint TimeStamp::initTime_ = std::chrono::high_resolution_clock::now(); // TODO

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
