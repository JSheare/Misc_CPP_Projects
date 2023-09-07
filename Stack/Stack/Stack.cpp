#include <iostream>

#include "Stack.h"

int main()
{
    JADT::Stack<int> stack{ 1, 2, 3, 4, 5, 6 };

    std::cout << "Peeking at stack " << stack.peek() << '\n';

    while (!stack.isEmpty())
    {
        std::cout << "Popping from stack: " << stack.pop() << '\n';
    }
}

