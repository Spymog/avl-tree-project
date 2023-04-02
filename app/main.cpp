#include <fstream>
#include <iostream>
#include "MyAVLTree.hpp"

int main()
{
    MyAVLTree<int, int> testAVLTree;

    testAVLTree.insert(4, 4);
    testAVLTree.insert(1, 1);
    testAVLTree.insert(2, 2);
    testAVLTree.insert(3, 3);
    testAVLTree.insert(5, 5);
    testAVLTree.insert(6, 6);

    std::vector<int> testVector = testAVLTree.preOrder();

    for (int i = 0; i < testVector.size(); i++)
    {
        std::cout << testVector.at(i) << std::endl;
    }

    // testAVLTree.contains(4);

    // int l = testAVLTree.lChild(2);
    // int r = testAVLTree.rChild(2);

    // std::cout << l << std::endl;
    // std::cout << r << std::endl;
    return 0;
}

