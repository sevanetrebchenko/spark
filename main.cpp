#include <iostream>
#include "Utilities/memory_manager.h"
#include "Utilities/logger.h"
#include "Tree/binary_tree.h"
#include <chrono>

int main() {
    //void* a = UtilityBox::MemoryManager::allocate(45);


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

    return 0;
}
