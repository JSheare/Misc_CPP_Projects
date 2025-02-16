#if 1
#include <iostream>

#include "Graph.h"

int main()
{
	JADT::Graph<int, int> graph{};
	graph.addVertex(0, 0);
	graph.addVertex(1, 1);
	graph.addEdge(1, 0);
	auto neighbors{ graph.neighbors(1) };
	std::cout << neighbors << '\n';
	std::cout << graph.size() << '\n';
	std::cout << graph[0] << '\n';
	graph.removeVertex(0);
	std::cout << graph.size() << '\n';
}

#endif