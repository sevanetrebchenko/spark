#include <iostream>
#include "Utilities/memory_manager.h"
#include "Utilities/logger.h"
#include "Tree/binary_tree.h"

int main() {
    void* a = UtilityBox::MemoryManager::allocate(45);

    UtilityBox::Logger::logMessage(UtilityBox::Logger::DEBUG, "lol!");
//
//    std::cout << static_cast<Utilities::AssertionHandler::AssertionLevel>(-1) << std::endl;

    int *arraay = new int[10];
    delete[] arraay;

//    std::cout << sizeof(Utilities::MemoryManager) << std::endl;

    int *r = new int[10];
    delete[] r;

    return 0;
}
