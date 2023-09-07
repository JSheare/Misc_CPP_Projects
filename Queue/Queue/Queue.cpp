#include <iostream>

#include "Queue.h"

int main()
{
    JADT::Queue<int> queue{1, 2, 3, 4, 5};
    queue.push(6);
    while (!queue.isEmpty())
    {
        std::cout << "Popping from queue: " << queue.pop() << '\n';
    }
}
