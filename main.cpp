#include <iostream>
#include "Utilities/memory_manager.h"
#include "Utilities/logger.h"
#include "Tree/binary_tree.h"
#include <chrono>

int main() {
    //void* a = UtilityBox::MemoryManager::allocate(45);

    UtilityBox::Logger::LogMessage(UtilityBox::Logger::DEBUG, "this is a debug message");

    int *arraay = new int[10];
    delete[] arraay;

    UtilityBox::Logger::LogMessage(UtilityBox::Logger::WARNING, "this is a warning message");
    UtilityBox::Logger::LogMessage(UtilityBox::Logger::WARNING, "this is a warning message with variables! %i, %s", 420, "wowieswow!");

//    std::cout << sizeof(Utilities::MemoryManager) << std::endl;

    int *r = new int[10];
    delete[] r;

    UtilityBox::Logger::LogMessage(UtilityBox::Logger::ERROR, "this is an error message");

    return 0;
}
