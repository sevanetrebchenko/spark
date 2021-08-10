
#include "spark/events/types/application_events.h"

namespace Spark::Events {

    // WindowResizeEvent.
    WindowResizeEvent::WindowResizeEvent(int previousWidth, int previousHeight, int width, int height) : previousWidth_(previousWidth),
                                                                                                         previousHeight_(previousHeight),
                                                                                                         width_(width),
                                                                                                         height_(height)
                                                                                                         {
    }

    int WindowResizeEvent::GetWidth() const {
        return width_;
    }

    int WindowResizeEvent::GetHeight() const {
        return height_;
    }

    int WindowResizeEvent::GetPreviousWidth() const {
        return previousWidth_;
    }

    int WindowResizeEvent::GetPreviousHeight() const {
        return previousHeight_;
    }


    // WindowMinimizedEvent.
    WindowMinimizedEvent::WindowMinimizedEvent(bool minimized) : minimized_(minimized) {
    }

    bool WindowMinimizedEvent::GetMinimized() const {
        return minimized_;
    }

}
