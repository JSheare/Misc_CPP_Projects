#if 1
#include <iostream>

#include "Deque.h"

int main()
{
    JADT::Deque<int> deque;
    int range{ 100 };
    for (int i{ 0 }; i < range; ++i)
    {
        std::cout << "Pushing to the back: " << i << '\n';
        deque.pushBack(i);
    }

    std::cout << '\n';

    while (!deque.empty())
    {
        std::cout << "Popping from the back: " << deque.peekBack() << '\n';
        deque.popBack();
    }

    std::cout << '\n';

    for (int i{ 0 }; i < range; ++i)
    {
        std::cout << "Pushing to the front: " << i << '\n';
        deque.pushFront(i);
    }

    std::cout << '\n';

    while (!deque.empty())
    {
        std::cout << "Popping from the front: " << deque.peekFront() << '\n';
        deque.popFront();
    }

    std::cout << '\n';
    bool front{ true };
    for (int i{ 0 }; i < range; ++i)
    {
        if (front)
        {
            std::cout << "Pushing to the front: " << i << '\n';
            deque.pushFront(i);
        }
        else
        {
            std::cout << "Pushing to the back: " << i << '\n';
            deque.pushBack(i);
        }
        front = !front;
    }
    
    std::cout << '\n';
    front = !front;

    while (!deque.empty())
    {
        if (front)
        {
            std::cout << "Popping from the front: " << deque.peekFront() << '\n';
            deque.popFront();
        }
        else
        {
            std::cout << "Popping from the back: " << deque.peekBack() << '\n';
            deque.popBack();
        }
        front = !front;
    }

}
#endif