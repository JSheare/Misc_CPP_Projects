#include <iostream>

#include "List.h"

bool descending(int x, int y)
{
	return x >= y;
}

int main()
{
	JADT::List<int> list{ 678459, 56347, 35, 53, 1, 867, 2, 4, 12, 42, 543, 3, 654, 34, 15, 78 };

	list.pushBack(5);
	list.pushFront(10);

	std::cout << list << '\n';
	std::cout << "Length: " << list.size() << '\n';

	std::cout << "Sorting list in descending order\n";
	list.sort(descending);
	std::cout << list << '\n';

	std::cout << "Sorting list in ascending order\n";
	list.sort();
	std::cout << list << '\n';

	std::cout << "Popping data from list: " << list.front() << '\n';
	list.popFront();
	std::cout << list << '\n';
	std::cout << "Length: " << list.size() << '\n';

	std::cout << "Popping data from list: " << list.back() << '\n';
	list.popBack();
	std::cout << list << '\n';
	std::cout << "Length: " << list.size() << '\n';

	std::cout << "Getting element 10: " << list[10] << '\n';

	std::cout << "Looping through list:\n";
	for (auto& num : list)
	{
		std::cout << num << '\n';
	}

	std::cout << "Contains num 10: ";
	if (list.contains(10))
		std::cout << "Yes\n";
	else
		std::cout << "No\n";

	return 0;
}
