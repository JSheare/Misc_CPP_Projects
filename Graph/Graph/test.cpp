#include <iostream>

#include "Graph.h"
#include "Heap.h"

struct EdgePair
{
	char key{};
	int weight{};
};

bool compareEdge(const EdgePair& pair1, const EdgePair& pair2)
{
	return pair1.weight < pair2.weight;
}

JADT::Graph<char, int> makeGraph()
{
	JADT::Graph<char, int> test;
	for (int i{ 0 }; i < 26; ++i)
	{
		char key{ static_cast<char>('a' + i) };
		test.addVertex(key, i);
	}
	return test;
}


int main()
{
	JADT::Graph<char, int> test;
	std::cout << "Adding Vertices to graph:\n";
	for (int i{ 0 }; i < 26; ++i)
	{
		char key{ static_cast<char>('a' + i) };
		std::cout << "Key: " << key << " value: " << i << '\n';
		test.addVertex(key, i);
	}
	std::cout << '\n';

	std::cout << "Graph size: " << test.size() << "\n\n";

	std::cout << "Adding edges between vertices:\n";
	char key1{ 'a' };
	for (int i{ 1 }; i < 26; ++i)
	{
		char key2{ static_cast<char>('a' + i) };
		std::cout << "Vertex " << key1 << " to vertex " << key2 << " with weight " << i << '\n';
		test.addEdge(key1, key2, i);
		if (i % 3 == 0)
			key1 = key2;
	}
	std::cout << '\n';

	std::cout << "Getting the weight of the edge from a to b: " << test.getWeight('a', 'b') << "\n\n";

	std::cout << "Doing a BFS of the graph by edge weight:\n";
	JADT::Heap<EdgePair, compareEdge> pq;
	pq.insert(EdgePair('a', 0));
	while (!pq.empty())
	{
		EdgePair curr{ pq.top() };
		pq.pop();
		std::cout << "Key: " << curr.key << " value: " << test[curr.key] << '\n';
		auto& adjacent{ test.getAdjacent(curr.key) };
		for (auto& key : adjacent)
		{
			pq.insert(EdgePair(key, adjacent[key]));
		}
	}
	std::cout << '\n';
	return 0;
}