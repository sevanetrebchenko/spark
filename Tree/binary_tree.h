
#ifndef DATASTRUCTURES_BINARY_TREE_H
#define DATASTRUCTURES_BINARY_TREE_H

#include "../Utilities//memory_manager.h"

template <typename T>
class BinaryTree {
    public:
        BinaryTree() {
            std::cout << "constructor binary tree" << std::endl;
            int *array = new int[10];
            delete[] array;
        }

        ~BinaryTree() {
            std::cout << "destructor binary tree" << std::endl;
        }

        class TreeNode {
            T data;
            TreeNode *left;
            TreeNode *right;
        };

    private:
        TreeNode *root;
};

#endif //DATASTRUCTURES_BINARY_TREE_H
