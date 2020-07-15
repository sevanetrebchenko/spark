#include <iostream>
#include "UtilityBox/Logger/include/logger.h"
#include "UtilityBox/Logger/include/adapter.h"
#include "UtilityBox/Tools/assert_dev.h"
#include "UtilityBox/Memory/include/segmented_pool_allocator.h"
#include "UtilityBox/Memory/include/contiguous_pool_allocator.h"
#include "ECS/Components/include/component_manager.h"
#include "ECS/Components/include/base_component.h"
#include "ECS/Components/include/component_manager_collection.h"
#include "ECS/Systems/include/base_component_system.h"
#include "World/world.h"
#include "UtilityBox/Tools/compile_time_hash.h"
#include <SDL.h>

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
    const LogMessageSeverity& messageSeverity = LoggingHub::GetInstance()->GetMessageSeverity(messageAddress);

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
        static constexpr unsigned ID = STRINGHASH("Data");
        static constexpr const char* Name = "Data";

        int a;
        int b;
        float c;
        char h[16];
};


class Data2 : public ECS::Components::BaseComponent {
    public:
        static constexpr std::uint32_t ID = STRINGHASH("Data2");
        static constexpr const char* Name = "Data2";
        int a;
        int b;
        float c;
        char h[16];
};

class Data3 : public ECS::Components::BaseComponent {
    public:
        static constexpr unsigned ID = STRINGHASH("Data3");
        static constexpr const char* Name = "Data3";

        int a;
        int b;
        float c;
        char h[16];
};

class mySystem : ECS::Systems::BaseComponentSystem<Data, Data2, Data3> {
    public:
        explicit mySystem(std::string&& name);

        void Initialize() override;
        void Update(float dt) override;
        void Shutdown() override;
    private:
};

mySystem::mySystem(std::string &&name) : ECS::Systems::BaseComponentSystem<Data, Data2, Data3>(std::move(name)) {
}

void mySystem::Update(float dt) {
}

void mySystem::Initialize() {
    BaseSystem::Initialize();
    ComponentTuple a;
    Data2* ad = new Data2();
    std::get<1>(a) = ad;
    auto* data2ptr = GetComponent<Data3>(a);
    LogMessage message = LogMessage();
    message.Supply("Logging from mySystem.");
    this->_loggingSystem.Log(message);
}

void mySystem::Shutdown() {
}

int main() {
    UtilityBox::Logger::LoggingHub::Initialize();
    ECS::Components::ComponentManagerCollection<ALL_COMPONENTS>* ok = ECS::Components::ComponentManagerCollection<ALL_COMPONENTS>::GetInstance();

    ECS::Components::ComponentManager<ECS::Components::BaseComponent> a = ECS::Components::ComponentManager<ECS::Components::BaseComponent> {};

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
            "SDL2Test",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640,
            480,
            0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(3000);

    SDL_DestroyWindow(window);
//    SDL_Quit();

    auto* entitymanager = ENGINE_NAME::World::GetInstance().GetEntityManager();
    entitymanager->CreateEntity("Entity1");
    entitymanager->AddComponent<ECS::Components::BaseComponent>("Entity1");
    entitymanager->DeleteComponent<ECS::Components::BaseComponent>("Entity1");
    entitymanager->DestroyEntity("Entity1");

    return 0;
}
