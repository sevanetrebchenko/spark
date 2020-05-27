#include <iostream>
#include "Utilities/Logger/logger.h"

int main() {
    UtilityBox::Logger::LoggingHub::Initialize();

    //void* a = UtilityBox::MemoryManager::allocate(45);
    using namespace UtilityBox::Logger;
    auto* loggingSystem = new LoggingSystem("testing");
    auto* logMessage = new LogMessage(LogMessageSeverity::DEBUG);
    logMessage->Supply("hello with numbers! %i", 1);
    logMessage->Supply("2 hello with 2 numbers! %i %i", 42, 54);
    logMessage->Supply("This is going to be an exceptionally long message to test the dynamically resizing buffers of log messages and the subsequent intermediate log messages that will be included under the debug flag.");
    loggingSystem->Log(logMessage);
    delete loggingSystem;

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

    LoggingHub::Reset();
    return 0;
}
