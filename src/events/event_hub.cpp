
#include <events/event_hub.h>

namespace Spark::Events {
    class EventHub::EventHubData {
        public:
            EventHubData();
            ~EventHubData();

            void AttachEventListener(IEventListener* eventListener);
            void DetachEventListener(IEventListener* eventListener);
            void Dispatch(Event* event);
            void Update();

        private:
            std::vector<IEventListener*> _eventListeners;
    };
    EventHub* EventHub::_instance = nullptr;


    EventHub::EventHubData::EventHubData() {

    }

    EventHub::EventHubData::~EventHubData() {

    }

    void EventHub::EventHubData::AttachEventListener(IEventListener *eventListener) {
        if (std::find(_eventListeners.cbegin(), _eventListeners.cend(), eventListener) == _eventListeners.cend()) {
            _eventListeners.emplace_back(eventListener);
        }
        else {
            std::cout << "already exists" << std::endl;
            // Already exists in map.
        }
    }

    void EventHub::EventHubData::DetachEventListener(IEventListener *eventListener) {
        if (!_eventListeners.empty()) {
            auto eventListenerLocation = std::find(_eventListeners.begin(), _eventListeners.end(), eventListener);
            if (eventListenerLocation != _eventListeners.end()) {
                // Switch event listener at the 'toDelete' location with the event listener at the end of the list.
                auto lastPosition = std::prev(_eventListeners.end());
                *eventListenerLocation = *lastPosition;

                // Remove the last element to keep data contiguous.
                _eventListeners.pop_back();
            }
            else {
                std::cout << "not found" << std::endl;
            }
        }
    }

    void EventHub::EventHubData::Dispatch(Event* event) {
        std::shared_ptr<Event*> sharedPointer = std::make_shared<Event*>(event);

        // Dispatch event to all the listeners.
        for (auto* eventListener : _eventListeners) {
            eventListener->OnEventReceived(sharedPointer);
        }
    }

    void EventHub::EventHubData::Update() {
        for (auto& eventListener : _eventListeners) {
            eventListener->OnUpdate();
        }
    }

    EventHub *EventHub::GetInstance() {
        if (!_instance) {
            _instance = new EventHub();
        }

        return _instance;
    }

    EventHub::EventHub() : _data(new EventHubData()) {
        // Nothing to do here.
    }

    EventHub::~EventHub() {
        delete _data;
    }

    void EventHub::AttachEventListener(IEventListener *eventListener) {
        _data->AttachEventListener(eventListener);
    }

    void EventHub::DetachEventListener(IEventListener *eventListener) {
        _data->DetachEventListener(eventListener);
    }

    void EventHub::Dispatch(Event *event) {
        _data->Dispatch(event);
    }

    void EventHub::OnUpdate() {
        _data->Update();
    }
}
