#if 1
#include <iostream>

#include "Stack.h"

int main()
{
    JML::Stack<int> stack;
    for (int i{ 0 }; i < 100; ++i)
    {
        std::cout << "Pushing: " << i << '\n';
        stack.push(i);
    }

    std::cout << '\n';

    stack.shrink_to_fit();

    while (!stack.empty())
    {
        std::cout << "Popping: " << stack.top() << '\n';
        stack.pop();
    }

    return 0;
}
#endif