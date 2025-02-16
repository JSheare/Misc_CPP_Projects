#ifndef JADT_GRAPH_H
#define JADT_GRAPH_H

#include "List.h"
#include "Vertex.h"

namespace JADT
{
	template <typename T, typename U>
	class Graph
	{
	public:
		Graph();
		Graph(Graph<T, U>&& graph) noexcept;
		~Graph();
		Graph<T, U>& operator=(Graph<T, U>&& graph);
		U operator[](T key) const;
		void addVertex(T key);
		void addVertex(T key, U value);
		U getVertexValue(T key) const;
		void setVertexValue(T key, U value);
		void removeVertex(T key);
		void addEdge(T key1, T key2, int weight = 0);
		void addDoubleEdge(T key1, T key2, int weight1 = 0, int weight2 = weight1);
		int getEdgeWeight(T key1, T key2);
		void removeEdge(T key1, T key2);
		void removeDoubleEdge(T key1, T key2);
		const List<T>& getVertices() const;
		bool contains(T key) const;
		int size() const;
		bool adjacent(T key1, T key2) const;
		List<T> neighbors(T key) const;

	private:
		List<Vertex<T, U>*> vertList{};  // You should eventually redo this with a hash map. Same with the vertices
		List<T> vertKeys{};
		int numVerts{};

		Vertex<T, U>& getVertex(T key);
		const Vertex<T, U>& getVertex(T key) const;
		Graph(Graph<T, U>& graph);  // Copy constructor (prohibited)
		Graph<T, U>& operator=(const Graph<T, U>& graph);  // Copy assignment (prohibited)
	};
}
#include "Graph.hpp"
#endif
