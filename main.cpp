#include <iostream>
#include "Utilities/memory_manager.h"
#include "Utilities/logger.h"
#include "Tree/binary_tree.h"
#include <chrono>

int main() {
    void* a = UtilityBox::MemoryManager::allocate(45);
    auto *me = new LogMessage();
    me->Supply(DEBUG, "this is a debug message");
    UtilityBox::Logger::Log(me);

    int *arraay = new int[10];
    delete[] arraay;

    int *r = new int[10];
    delete[] r;

    return 0;
}
