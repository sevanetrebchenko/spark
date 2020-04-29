
#ifndef DATASTRUCTURES_BINARY_TREE_H
#define DATASTRUCTURES_BINARY_TREE_H

#include "../MemoryManager/memory_manager.h"

template <typename T, class Allocator = MemoryManager<T>>
class BinaryTree {
    public:
        class TreeNode {
            T data;
            TreeNode *left;
            TreeNode *right;
        };

    private:
        Allocator manager;
        TreeNode *root;
};

#endif //DATASTRUCTURES_BINARY_TREE_H
