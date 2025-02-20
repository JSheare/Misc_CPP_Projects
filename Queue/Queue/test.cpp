#if 1
#include <iostream>

#include "queue.h"

int main()
{
    JADT::Queue<int> queue;
    for (int i{ 0 }; i < 100; ++i)
    {
        std::cout << "Pushing: " << i << '\n';
        queue.push(i);
    }

    std::cout << '\n';

    while (!queue.empty())
    {
        std::cout << "Popping: " << queue.front() << '\n';
        queue.pop();
    }

	return 0;
}
#endif