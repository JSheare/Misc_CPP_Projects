#include <iostream>

#include "List.h"

bool descending(int x, int y)
{
	return x >= y;
}

int main()
{
	JADT::List<int> list{678459, 56347, 35, 53, 1, 867, 2, 4, 12, 42, 543, 3, 654, 34, 15, 78};

	std::cout << list << '\n';
	std::cout << "Length: " << list.length() << '\n';

	std::cout << "Sorting list in descending order\n";
	list.sort(descending);
	std::cout << list << '\n';

	std::cout << "Sorting list in ascending order\n";
	list.sort();
	std::cout << list << '\n';

	int data = list.remove(0);
	std::cout << "Popping data from list: " << data << '\n';
	std::cout << list << '\n';
	std::cout << "Length: " << list.length() << '\n';

	if (list.contains(678459))
	{
		std::cout << "Yes\n";
	}
	else
	{
		std::cout << "No\n";
	}

	return 0;
}
