#include <iostream>
#include <string>

#include "PriorityQueue.h"

int main()
{
    JADT::PriorityQueue<std::string> queue{};

    queue.push(0, "a");
    queue.push(4, "e");
    queue.push(2, "c");
    queue.push(1, "b");
    queue.push(3, "d");

    while (!queue.isEmpty())
    {
        std::cout << "Popping queue: " << queue.pop() << '\n';
    }

    std::cout << '\n';
    JADT::PriorityQueue<std::string> queue1{true};

    queue1.push(0, "a");
    queue1.push(4, "e");
    queue1.push(2, "c");
    queue1.push(1, "b");
    queue1.push(3, "d");

    while (!queue1.isEmpty())
    {
        std::cout << "Popping queue: " << queue1.pop() << '\n';
    }

}

