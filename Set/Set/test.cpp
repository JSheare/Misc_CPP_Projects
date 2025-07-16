#include <iostream>

#include "Set.h"

int main()
{
	JML::Set<int> test;
	std::cout << "Adding elements to set:\n";
	for (int i{ 0 }; i < 26; ++i)
	{
		std::cout << "Element: " << i << '\n';
		test.insert(i);
	}
	std::cout << '\n';

	std::cout << "Traversing set in order:\n";
	for (int i{ 0 }; i < 26; ++i)
	{
		std::cout << "Contains " << i << ": " << (test.contains(i) ? "True" : "False") << '\n';
	}
	std::cout << '\n';


	std::cout << "Traversing set with iterators:\n";
	for (auto& element : test)
	{
		std::cout << "Element: " << element << '\n';
	}
	return 0;
}