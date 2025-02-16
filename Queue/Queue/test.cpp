#if 1
#include <iostream>

#include "queue.h"

int main()
{
    JADT::Queue<int> queue{ 1, 2, 3, 4, 5 };
    queue.push(6);
    while (!queue.isEmpty())
    {
        std::cout << "Popping from queue: " << queue.pop() << '\n';
    }
	return 0;
}
#endif