#if 1
#include <iostream>

#include "Heap.h"

int main()
{
    std::cout << "Max heap with starting capacity 2:" << '\n';
    JML::Heap<int> minHeap{ 2 };
    minHeap.insert(5);
    minHeap.insert(17);
    minHeap.insert(3);
    minHeap.insert(0);
    minHeap.insert(6);

    std::cout << "Popping from max heap\n";
    while (!minHeap.empty())
    {
        std::cout << minHeap.top() << '\n';
        minHeap.pop();
    }

    std::cout << '\n';
    std::cout << "Min heap:" << '\n';
    JML::Heap<int, [](const int& x, const int& y) {return x < y; } > maxHeap;
    maxHeap.insert(5);
    maxHeap.insert(17);
    maxHeap.insert(3);
    maxHeap.insert(0);
    maxHeap.insert(6);

    std::cout << "Popping from min heap\n";
    while (!maxHeap.empty())
    {
        std::cout << maxHeap.top() << '\n';
        maxHeap.pop();
    }
}
#endif