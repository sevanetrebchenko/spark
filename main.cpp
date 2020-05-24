#include <iostream>
#include "Utilities/memory_manager.h"
#include "Utilities/Logger/logger.h"
#include "Tree/binary_tree.h"
#include <chrono>
#include <thread>

int main() {
    std::string a = "\0";
    std::string b = "\0";

    if (a == b) {
        std::cout << "equal" << std::endl;
    }

    auto time = std::chrono::high_resolution_clock::now();

    //void* a = UtilityBox::MemoryManager::allocate(45);
    using namespace UtilityBox::Logger;
    auto* loggingSystem = new LoggingSystem("testing");
    auto* logMessage = new LogMessage(DEBUG);
    logMessage->Supply("hello with numbers! %i");
    logMessage->Supply("2 hello with 2 numbers! %i %i", 42, 54);
    loggingSystem->Log(logMessage);
    loggingSystem->Log(DEBUG, "testing");
    loggingSystem->Log(DEBUG, "testing");
    loggingSystem->Log(DEBUG, "testing");

    //LogMessage<int> *v = new LogMessage<int>(DEBUG, "lmao");

//    auto *ml = new UtilityBox::Logger::MultiLineMessage();
//    ml->Supply(UtilityBox::Logger::DEBUG, "this is the beginning to a multiline debug message");
//    ml->Supply(UtilityBox::Logger::DEBUG, "this is the second line");
//    ml->Supply(UtilityBox::Logger::DEBUG, "this is the third line");
//    UtilityBox::Logger::Log(ml);
//
//    UtilityBox::Logger::Log(UtilityBox::Logger::ERROR, "this a single line message with numbers %i %f %d", 1, 3.14, 2);
//
//    auto* sl1 = new UtilityBox::Logger::SingleLineMessage(UtilityBox::Logger::WARNING,"this is a single line message from a constructor.");
//    UtilityBox::Logger::Log(sl1);
//
//    auto* sl2 = new UtilityBox::Logger::SingleLineMessage(UtilityBox::Logger::WARNING,"this is a second single line message from a constructor. %i", 10);
//    UtilityBox::Logger::Log(sl2);

    int *arraay = new int[10];
    delete[] arraay;

    int *r = new int[10];
    delete[] r;

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "program ran for " << (end - time).count() / 1000 << " milliseconds" << std::endl;

    return 0;
}
