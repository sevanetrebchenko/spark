//#include "utilitybox/logger/logger.h"
//#include "utilitybox/logger/adapter.h"
//#include "utilitybox/tools/assert_dev.h"
//#include "utilitybox/memory/segmented_pool_allocator.h"
//#include "ecs/components/component_manager.h"
//#include "ecs/components/base_component.h"
//#include "ecs/components/component_manager_collection.h"
//#include "ecs/systems/base_component_system.h"
//#include "core/core.h"
//#include "utilitybox/tools/compile_time_hash.h"
//
//#include <graphics/context/window.h>
//#include <GLFW/glfw3.h>
//
//using namespace Spark::UtilityBox::Logger;
//
//class myAdapter : public Adapter {
//    public:
//        myAdapter();
//        void ProcessMessage(void* messageAddress) override;
//        void OutputMessage() override;
//
//    private:
//
//};
//
//myAdapter::myAdapter() : Adapter("myAdapter") {
//    std::queue<HeaderFormatElement> headerFormat;
//    headerFormat.push(HeaderFormatElement::SEPARATOR);
//    headerFormat.push(HeaderFormatElement::NEWLINE);
//    headerFormat.push(HeaderFormatElement::DATE);
//    headerFormat.push(HeaderFormatElement::NEWLINE);
//    headerFormat.push(HeaderFormatElement::SEPARATOR);
//    headerFormat.push(HeaderFormatElement::NEWLINE);
//    _config.PushHeaderFormat(headerFormat);
//
//    std::queue<MessageFormatElement> messageFormat;
//    messageFormat.push(MessageFormatElement::TAB);
//    messageFormat.push(MessageFormatElement::TIMESTAMP);
//    messageFormat.push(MessageFormatElement::SPACE);
//    messageFormat.push(MessageFormatElement::SPACE);
//    messageFormat.push(MessageFormatElement::SPACE);
//    messageFormat.push(MessageFormatElement::MESSAGE);
//    messageFormat.push(MessageFormatElement::NEWLINE);
//    _config.PushMessageFormat(messageFormat);
//}
//
//void myAdapter::ProcessMessage(void *messageAddress) {
//    messageAddress = static_cast<int*>(messageAddress) + 3;
//    const LogMessageSeverity& messageSeverity = LoggingHub::GetInstance()->GetMessageSeverity(messageAddress);
//
//    if (messageSeverity >= _config.GetMessageSeverityCutoff()) {
//        // format header
//        ConstructMessageHeader(messageAddress);
//
//        // format messages
//        ConstructMessageBody(messageAddress);
//    }
//}
//
//void myAdapter::OutputMessage() {
//    while (!_formattedMessages.empty()) {
//        std::cerr << _formattedMessages.front();
//        _formattedMessages.pop();
//    }
//}
//
//class Data : public Spark::ECS::Components::BaseComponent {
//    public:
//        static constexpr unsigned ID = STRINGHASH("Data");
//        static constexpr const char* Name = "Data";
//
//        int a;
//        int b;
//        float c;
//        char h[16];
//};
//
//
//class Data2 : public Spark::ECS::Components::BaseComponent {
//    public:
//        static constexpr std::uint32_t ID = STRINGHASH("Data2");
//        static constexpr const char* Name = "Data2";
//        int a;
//        int b;
//        float c;
//        char h[16];
//};
//
//class Data3 : public Spark::ECS::Components::BaseComponent {
//    public:
//        static constexpr unsigned ID = STRINGHASH("Data3");
//        static constexpr const char* Name = "Data3";
//
//        int a;
//        int b;
//        float c;
//        char h[16];
//};
//
//class mySystem : Spark::ECS::Systems::BaseComponentSystem<Data, Data2, Data3> {
//    public:
//        explicit mySystem(std::string&& name);
//
//        void Initialize() override;
//        void OnUpdate(float dt) override;
//        void Shutdown() override;
//    private:
//};
//
//mySystem::mySystem(std::string &&name) : Spark::ECS::Systems::BaseComponentSystem<Data, Data2, Data3>(std::move(name)) {
//}
//
//void mySystem::OnUpdate(float dt) {
//}
//
//void mySystem::Initialize() {
//    BaseSystem::Initialize();
//    ComponentTuple a;
//    Data2* ad = new Data2();
//    std::get<1>(a) = ad;
//    auto* data2ptr = GetComponent<Data3>(a);
//    LogMessage message = LogMessage();
//    message.Supply("Logging from mySystem.");
//    this->_loggingSystem.Log(message);
//}
//
//void mySystem::Shutdown() {
//}
//
////int main() {
////    Spark::UtilityBox::Logger::LoggingHub::Initialize();
////    myAdapter* a = new myAdapter();
////    LoggingHub::GetInstance()->AttachCustomAdapter(a);
////
////    Spark::Core::GetInstance()->Initialize();
////    Spark::Core::GetInstance()->OnUpdate();
////    Spark::Core::GetInstance()->Shutdown();
////}
