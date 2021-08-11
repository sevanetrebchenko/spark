
#include "spark/events/event_hub.h"
#include "spark/utilitybox/logger/logger.h"

namespace Spark::Events {

    void EventHub::AttachEventListener(IEventListener* toAdd) {
        if (!toAdd) {
            return;
        }

        // Ensure event listener has not already been added.
        for (IEventListener* eventListener : listeners_) {
            if (eventListener == toAdd) {
                LogWarning("Adding instance of event listener that has already been registered with the EventHub. Listener will not be added.");
                return;
            }
        }

        listeners_.emplace_back(toAdd);
    }

    void EventHub::DetachEventListener(IEventListener* toRemove) {
        if (!toRemove) {
            return;
        }

        for (auto listenerIter = listeners_.begin(); listenerIter != listeners_.end(); ++listenerIter) {
            if (*listenerIter == toRemove) {
                listeners_.erase(listenerIter);
                return;
            }
        }
    }

    void EventHub::Dispatch(const EventHandle& event) {
        for (IEventListener* eventListener : listeners_) {
            eventListener->OnEventReceived(event);
        }
    }

    void Spark::Events::EventHub::OnUpdate(float dt) {
        for (IEventListener* eventListener : listeners_) {
            eventListener->OnUpdate(dt); // Updated in the order listeners are inserted.
        }
    }

//    class EventHub::EventHubData : public UtilityBox::Logger::ILoggable {
//        public:
//            EventHubData();
//            ~EventHubData();
//
//            void AttachEventListener(IEventListener* eventListener);
//            void DetachEventListener(IEventListener* eventListener);
//            void Dispatch(IEvent* event);
//            void Update();
//
//        private:
//            std::vector<IEventListener*> _eventListeners;
//    };
//
//    EventHub* EventHub::_instance = nullptr;
//
//
//    EventHub::EventHubData::EventHubData() : UtilityBox::Logger::ILoggable("Event Hub") {
//
//    }
//
//    EventHub::EventHubData::~EventHubData() {
//
//    }
//
//    void EventHub::EventHubData::AttachEventListener(IEventListener *eventListener) {
//        // Construct string of all the event types.
//        static std::stringstream format;
//        const std::vector<std::string>& eventTypesAsStrings = eventListener->GetEventTypesAsStrings();
//
//        // Print all except the last one with a comma.
//        for (int i = 0; i < eventTypesAsStrings.size() - 1; ++i) {
//            format << eventTypesAsStrings[i] << ", ";
//        }
//
//        format << eventTypesAsStrings[eventTypesAsStrings.size() - 1];
//        std::string eventTypesString = format.str();
//        format.str(std::string()); // Clear format.
//
//        if (std::find(_eventListeners.cbegin(), _eventListeners.cend(), eventListener) == _eventListeners.cend()) {
//            _eventListeners.emplace_back(eventListener);
//            LogDebug("Attached event listener with name '%s' registered with the following types: %s.", eventListener->GetName(), eventTypesString.c_str());
//        }
//        else {
//            LogWarning("Event listener tracking types: %s already exists in the event hub with name: %s.", eventTypesString.c_str(), eventListener->GetName());
//        }
//    }
//
//    void EventHub::EventHubData::DetachEventListener(IEventListener *eventListener) {
//        if (!_eventListeners.empty()) {
//            auto eventListenerLocation = std::find(_eventListeners.begin(), _eventListeners.end(), eventListener);
//            if (eventListenerLocation != _eventListeners.end()) {
//                // Switch event listener at the 'toDelete' location with the event listener at the end of the list.
//                auto lastPosition = std::prev(_eventListeners.end());
//                *eventListenerLocation = *lastPosition;
//
//                // Remove the last element to keep data contiguous.
//                _eventListeners.pop_back();
//            }
//            else {
//                std::cout << "not found" << std::endl;
//            }
//        }
//    }
//
//    void EventHub::EventHubData::Dispatch(IEvent* event) {
//        LogDebug("Dispatching event of type: %s. Event description: %s", IEvent::ConvertEventTypeToString(event->GetEventType()).c_str(), event->ToString().c_str());
//        std::shared_ptr<IEvent*> sharedPointer = std::make_shared<IEvent*>(event);
//
//        // Dispatch event to all the listeners.
//        bool registeredListenerOfType = false;
//        for (auto* eventListener : _eventListeners) {
//            registeredListenerOfType |= eventListener->OnEventReceived(sharedPointer);
//        }
//
//        // No listeners are hooked up to this event.
//        if (!registeredListenerOfType) {
//            LogWarning("No event listener registered to receive events of type: '%s'.", IEvent::ConvertEventTypeToString(event->GetEventType()).c_str());
//        }
//    }
//
//    void EventHub::EventHubData::Update() {
//        for (auto& eventListener : _eventListeners) {
//            eventListener->OnUpdate();
//        }
//    }
//
//    EventHub *EventHub::GetInstance() {
//        if (!_instance) {
//            _instance = new EventHub();
//        }
//
//        return _instance;
//    }
//
//    EventHub::EventHub() : _data(new EventHubData()) {
//        // Nothing to do here.
//    }
//
//    EventHub::~EventHub() {
//        delete _data;
//    }
//
//    void EventHub::AttachEventListener(IEventListener *eventListener) {
//        _data->AttachEventListener(eventListener);
//    }
//
//    void EventHub::DetachEventListener(IEventListener *eventListener) {
//        _data->DetachEventListener(eventListener);
//    }
//
//    void EventHub::Dispatch(IEvent *event) {
//        _data->Dispatch(event);
//    }
//
//    void EventHub::OnUpdate() {
//        _data->Update();
//    }
}
