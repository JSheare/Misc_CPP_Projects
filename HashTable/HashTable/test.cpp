#include <iostream>

#include "HashTable.h"

int main()
{
	JML::HashTable<char, int> test;
	std::cout << "Adding pairs to hash table:\n";
	for (int i{ 0 }; i < 26; ++i)
	{
		char key{ static_cast<char>('a' + i) };
		std::cout << "key: " << key << " value: " << i << '\n';
		test[key] = i;
	}
	std::cout << '\n';

	std::cout << "Traversing hash table in order:\n";
	for (int i{ 0 }; i < 26; ++i)
	{
		char key{ static_cast<char>('a' + i) };
		std::cout << "key: " << key << " value: " << test[key] << '\n';
		test['a' + i] = i;
	}
	std::cout << '\n';


	std::cout << "Traversing hash table with iterators:\n";
	for (auto& key : test)
	{
		std::cout << "key: " << key << " value: " << test[key] << '\n';
	}
	return 0;
}