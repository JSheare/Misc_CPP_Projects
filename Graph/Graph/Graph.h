#ifndef JML_GRAPH_H
#define JML_GRAPH_H

#include <cstddef>

#include "HashTable.h"

namespace JML
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
		template <typename T1, typename U1> friend bool operator==(const Graph<T1, U1>& graph1, const Graph<T1, U1>& graph2);
		template <typename T1, typename U1> friend bool operator!=(const Graph<T1, U1>& graph1, const Graph<T1, U1>& graph2);
		U& operator[](const T& key);
		const U& operator[](const T& key) const;
		bool empty() const;
		std::size_t size() const;
		bool contains(const T& key) const;
		template <typename V, typename W> void addVertex(V&& key, W&& value);
		U& getVertex(const T& key);
		const U& getVertex(const T& key) const;
		void removeVertex(const T& key);
		bool adjacent(const T& key1, const T& key2) const;
		const HashTable<T, int>& getAdjacent(const T& key) const;
		int getWeight(const T& key1, const T& key2) const;
		void addEdge(const T& key1, const T& key2, int weight=0);
		void removeEdge(const T& key1, const T& key2);
		void clear();

	private:
		std::size_t numVerts{ 0 };
		HashTable<T, Vertex*> vertTable{};

		class Vertex
		{
		public:
			U value{};
			HashTable<T, int> edges{ 1 };

			Vertex(const U& value);
			Vertex(U&& value);
		};
	};
}
#include "Graph.hpp"
#endif
