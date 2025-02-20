#if 1
#include <iostream>

#include "Heap.h"

int main()
{
    std::cout << "Min heap with starting capacity 2:" << '\n';
    JADT::Heap<int, int> heap1{ 2 };
    heap1.insert(5, 5);
    heap1.insert(17, 17);
    heap1.insert(3, 3);
    heap1.insert(0, 0);
    heap1.insert(6, 6);

    std::cout << "Popping from min heap\n";
    while (!heap1.isEmpty())
    {
        std::cout << heap1.extract() << '\n';
    }

    std::cout << '\n';
    std::cout << "Max heap:" << '\n';
    JADT::Heap<int, int> heap2{ true };
    heap2.insert(5, 5);
    heap2.insert(17, 17);
    heap2.insert(3, 3);
    heap2.insert(0, 0);
    heap2.insert(6, 6);

    std::cout << "Popping from max heap\n";
    while (!heap2.isEmpty())
    {
        std::cout << heap2.extract() << '\n';
    }
}
#endif