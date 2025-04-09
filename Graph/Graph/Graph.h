#ifndef JADT_GRAPH_H
#define JADT_GRAPH_H

#include <cstddef>

#include "HashTable.h"

namespace JADT
{
	template <typename T, typename U = T>
	class Graph
	{
	private:
		class Vertex;

	public:
		Graph();
		Graph(const Graph<T, U>& graph);  // Copy constructor
		Graph(Graph<T, U>&& graph) noexcept;  // Move constructor
		~Graph();
		Graph<T, U>& operator=(const Graph<T, U>& graph);  // Copy assignment
		Graph<T, U>& operator=(Graph<T, U>&& graph) noexcept;  // Move assignment
		U& operator[](const T& key);
		const U& operator[](const T& key) const;
		bool empty() const;
		std::size_t size() const;
		bool contains(const T& key) const;
		void addVertex(const T& key, const U& value);
		U& getVertex(const T& key);
		const U& getVertex(const T& key) const;
		void removeVertex(const T& key);
		bool adjacent(const T& key1, const T& key2) const;
		const HashTable<T, int>& getAdjacent(const T& key) const;
		int getWeight(const T& key1, const T& key2) const;
		void addEdge(const T& key1, const T& key2, int weight=1);
		void removeEdge(const T& key1, const T& key2);
		void clear();

	private:
		std::size_t numVerts{ 0 };
		HashTable<T, Vertex*> vertTable;

		class Vertex
		{
		public:
			U value;
			HashTable<T, int> edges{ 1 };

			Vertex(const U& value);
		};
	};
}
#include "Graph.hpp"
#endif
