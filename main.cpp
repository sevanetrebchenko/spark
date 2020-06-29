#include <iostream>
#include "Utilities/Logger/logger.h"
#include "Utilities/Logger/adapter.h"
#include "Utilities/assert_dev.h"
#include "Utilities/Memory/segmented_pool_allocator.h"
#include "Utilities/Memory/contiguous_pool_allocator.h"
#include "Utilities/DataStructures/array.h"
#include "ECS/Components/component_manager.h"
#include "ECS/Components/base_component.h"

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
        int a;
        int b;
        float c;
        char h[16];
};

int main() {
    UtilityBox::Logger::LoggingHub::Initialize();

    std::array<Data, 5> array;

    auto* a = new UtilityBox::DataStructures::Array(sizeof(Data), 5);
    a->Initialize();
    auto* b = a->operator[]<Data>(0);
    b->b = 60;


//    auto* a = new UtilityBox::Memory::ContiguousPoolAllocator(2, 2, false);
//    a->Initialize();
//    void* b = a->RetrieveBlock();
//    void* c = a->RetrieveBlock();
//    void* d = a->RetrieveBlock();
//    a->ReturnBlock(b);
//    a->ReturnBlock(c);

//    auto* a = new UtilityBox::Memory::SegmentedPoolAllocator(2);
//    a->Initialize();
//    void* b = a->RetrieveBlock();
//    a->ReturnBlock(b);


//    auto* a = new ECS::Components::ComponentManager<Data>();
//    a->Initialize();
//    auto* b = a->CreateComponent();
//    a->DeleteComponent(b);
//    delete a;
delete a;

    return 0;
}
