#include <iostream>

#include "Dequeue.h"

int main()
{
    JADT::Dequeue<int> dequeue{ 1, 2, 3, 4, 5, 6 };

    dequeue.addFront(0);
    dequeue.addRear(7);

    JADT::Dequeue<int> dequeue1{ dequeue };

    while (!dequeue.isEmpty())
    {
        std::cout << "Popping from the front: " << dequeue.removeFront() << '\n';
    }
    std::cout << '\n';

    while (!dequeue1.isEmpty())
    {
        std::cout << "Popping from the rear: " << dequeue1.removeRear() << '\n';
    }
}