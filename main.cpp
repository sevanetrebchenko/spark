#include <iostream>
#include "Utilities/Logger/logger.h"
#include "Utilities/Logger/adapter.h"
#include "Utilities/assert_dev.h"
#include "Utilities/Memory/memory_manager.h"

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

struct Data {
    int a;
    int b;
    float c;
    char h[10];
};

int main() {
    UtilityBox::Logger::LoggingHub::Initialize();

    UtilityBox::Memory::PoolAllocator* myPoolAllocator = new UtilityBox::Memory::PoolAllocator(sizeof(Data));
    myPoolAllocator->Initialize();
    Data* block = new(myPoolAllocator->RetrieveBlock()) Data();
    block->a = 9;
    block->b = 16;
    block->c = 3.14;
    myPoolAllocator->ReturnBlock(block);

    return 0;
}
