#include <iostream>
#include "Utilities/Logger/logger.h"
#include "Utilities/Logger/adapter.h"
#include "Utilities/assert_dev.h"
#include "Utilities/Memory/segmented_pool_allocator.h"
#include "Utilities/Memory/contiguous_pool_allocator.h"
#include "Utilities/DataStructures/Array/array.h"
#include "ECS/Components/component_manager.h"
#include "ECS/Components/base_component.h"
#include "ECS/Components/component_manager_collection.h"
#include "ECS/Systems/base_component_system.h"

using namespace UtilityBox::Logger;

class myAdapter : public Adapter {
    public:
        myAdapter();
        void ProcessMessage(void* messageAddress) override;
        void OutputMessage() override;
    private:
};

myAdapter::myAdapter() : Adapter("myAdapter") {
}

void myAdapter::ProcessMessage(void *messageAddress) {
    messageAddress = static_cast<int*>(messageAddress) + 3;
    const LogMessageSeverity& messageSeverity = LoggingHub::GetInstance().GetMessageSeverity(messageAddress);

    if (messageSeverity >= _config.GetMessageSeverityCutoff()) {
        // format header
        ConstructMessageHeader(messageAddress);

        // format messages
        ConstructMessageBody(messageAddress);
    }
}

void myAdapter::OutputMessage() {
    while (!_formattedMessages.empty()) {
        std::cout << _formattedMessages.front();
        _formattedMessages.pop();
    }
}

class Data : public ECS::Components::BaseComponent {
    public:
        static unsigned ID;

        int a;
        int b;
        float c;
        char h[16];
};

unsigned Data::ID = std::hash<std::string>{}("Data");
//
//class Data2 : public ECS::Components::BaseComponent {
//    public:
//        int a;
//        int b;
//        float c;
//        char h[16];
//};
//
//class Data3 : public ECS::Components::BaseComponent {
//    public:
//        int a;
//        int b;
//        float c;
//        char h[16];
//};

int main() {
    UtilityBox::Logger::LoggingHub::Initialize();

    throw std::out_of_range("invalid range");


    ECS::Components::ComponentManagerCollection<Data>* managerCollection = new ECS::Components::ComponentManagerCollection<Data>();
    managerCollection->Initialize();
    auto* dataManager = managerCollection->GetComponentManager<Data>();
    auto* dataBlock = dataManager->CreateComponent();

    return 0;
}
