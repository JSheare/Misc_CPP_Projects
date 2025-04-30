#ifndef JML_GRAPH_HPP
#define JML_GRAPH_HPP

#include <cstddef>
#include <stdexcept>

#include "HashTable.h"

namespace JML
{
	template <typename T, typename U>
	Graph<T, U>::Graph()
	{}

	// Copy constructor
	template <typename T, typename U>
	Graph<T, U>::Graph(const Graph<T, U>& graph) :
		numVerts{graph.numVerts}
	{
		for (const T& key : graph.vertTable)
		{
			Vertex* oldVert{ graph.vertTable[key] };
			Vertex* newVert{ new Vertex(oldVert->value) };
			newVert->edges = oldVert->edges;
			vertTable[key] = newVert;
		}
	}

	// Move constructor
	template <typename T, typename U>
	Graph<T, U>::Graph(Graph<T, U>&& graph) noexcept :
		numVerts{graph.numVerts}
	{
		vertTable = graph.vertTable;
		graph.vertTable.clear();
		graph.numVerts = 0;
	}

	template <typename T, typename U>
	Graph<T, U>::~Graph()
	{
		clear();
	}

	// Copy assignment
	template <typename T, typename U>
	Graph<T, U>& Graph<T, U>::operator=(const Graph<T, U>& graph)
	{
		if (&graph == this)
			return *this;

		clear();
		numVerts = graph.numVerts;
		for (const T& key : graph.vertTable)
		{
			Vertex* oldVert{ graph.vertTable[key] };
			Vertex* newVert{ new Vertex(oldVert->value) };
			newVert->edges = oldVert->edges;
			vertTable[key] = newVert;
		}
		return *this;
	}

	// Move assignment
	template <typename T, typename U>
	Graph<T, U>& Graph<T, U>::operator=(Graph<T, U>&& graph) noexcept
	{
		clear();
		vertTable = graph.vertTable;
		graph.vertTable.clear();
		graph.numVerts = 0;
		return *this;

	}

	template <typename T1, typename U1>
	bool operator==(const Graph<T1, U1>& graph1, const Graph<T1, U1>& graph2)
	{
		if (graph1.numVerts == graph2.numVerts)
		{
			typename Graph<T1, U1>::Vertex* vertex1{ nullptr };
			typename Graph<T1, U1>::Vertex* vertex2{ nullptr };
			for (const T1& key : graph1.vertTable)
			{
				try
				{
					vertex1 = graph1.vertTable[key];
					vertex2 = graph2.vertTable[key];
					if ((vertex1->value != vertex2->value) || (vertex1->edges != vertex2->edges))
						return false;
				}
				catch (std::invalid_argument&)
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	template <typename T1, typename U1>
	bool operator!=(const Graph<T1, U1>& graph1, const Graph<T1, U1>& graph2)
	{
		return !operator==(graph1, graph2);
	}

	template <typename T, typename U>
	U& Graph<T, U>::operator[](const T& key)
	{
		return getVertex(key);
	}

	template <typename T, typename U>
	const U& Graph<T, U>::operator[](const T& key) const
	{
		return getVertex(key);
	}

	// Returns true if there aren't any vertices in the graph
	template <typename T, typename U>
	bool Graph<T, U>::empty() const
	{
		return numVerts == 0;
	}

	// Returns the number of vertices in the graph
	template <typename T, typename U>
	std::size_t Graph<T, U>::size() const
	{
		return numVerts;
	}

	// Returns true if the graph contains a vertex with the given key
	template <typename T, typename U>
	bool Graph<T, U>::contains(const T& key) const
	{
		return vertTable.contains(key);
	}

	// Adds a vertex with the given key and value to the graph. Supports perfect forwarding
	template <typename T, typename U>
	template <typename V, typename W> void Graph<T, U>::addVertex(V&& key, W&& value)
	{
		Vertex* newVertex{ new Vertex(static_cast<W&&>(value)) };
		vertTable[static_cast<V&&>(key)] = newVertex;
		++numVerts;
	}

	// Returns the value of the vertex with the given key. Throws std::invalid_argument if no vertex with the given key exists
	template <typename T, typename U>
	U& Graph<T, U>::getVertex(const T& key)
	{
		if (vertTable.contains(key))
			return vertTable[key]->value;

		throw std::invalid_argument("Not a valid vertex key");
	}

	template <typename T, typename U>
	const U& Graph<T, U>::getVertex(const T& key) const
	{
		if (vertTable.contains(key))
			return vertTable[key]->value;

		throw std::invalid_argument("Not a valid vertex key");
	}

	// Removes the vertex with the given key from the graph
	template <typename T, typename U>
	void Graph<T, U>::removeVertex(const T& key)
	{
		if (vertTable.contains(key))
		{
			// Removing the vertex itself
			delete vertTable[key];
			vertTable.remove(key);
			--numVerts;
			// Finding and removing all edges to the vertex
			for (T& vertKey : vertTable)
			{
				for (T& adjacentKey : vertTable[vertKey]->edges)
				{
					if (adjacentKey == key)
					{
						vertTable[vertKey]->edges.remove(key);
						break;
					}
				}
			}
		}
	}

	// Returns true if there's an edge from the vertex with key1 to the vertex with key2. Throws std::invalid_argument if no vertex exists for one or both of the keys
	template <typename T, typename U>
	bool Graph<T, U>::adjacent(const T& key1, const T& key2) const
	{
		if (vertTable.contains(key1) && vertTable.contains(key2))
		{
			return vertTable[key1]->edges.contains(key2);
		}
		throw std::invalid_argument("One or both of the given keys are not valid");
	}

	// Returns a constant reference to the outgoing edge table of the vertex with the given key. The table is full of key-weight pairs
	template <typename T, typename U>
	const HashTable<T, int>& Graph<T, U>::getAdjacent(const T& key) const
	{
		if (vertTable.contains(key))
			return vertTable[key]->edges;

		throw std::invalid_argument("Not a valid vertex key");
	}

	// Returns the weight of the outgoing edge from the vertex with key1 to the vertex with key2. Throws std::invalid_argument if no vertex exists for one or both of the keys
	// or if no outgoing edge from vertex1 to vertex2 exists
	template <typename T, typename U>
	int Graph<T, U>::getWeight(const T& key1, const T& key2) const
	{
		if (vertTable.contains(key1) && vertTable.contains(key2))
		{
			if (vertTable[key1]->edges.contains(key2))
				return vertTable[key1]->edges[key2];

			else
				throw std::invalid_argument("No edge from key1 vertex to key2 vertex");
		}
		else
			throw std::invalid_argument("One or both of the given keys are not valid");
	}

	// Adds an outgoing edge from the vertex with key1 to the vertex with key2, or updates the weight of the existing edge. Throws std::invalid_argument if no vertex
	// exists for one or both of the keys
	template <typename T, typename U>
	void Graph<T, U>::addEdge(const T& key1, const T& key2, int weight)
	{
		if (vertTable.contains(key1) && vertTable.contains(key2))
		{
			vertTable[key1]->edges[key2] = weight;
		}
		else
			throw std::invalid_argument("One or both of the given keys are not valid");
	}

	// Removes the outgoing edge between the vertex with key1 and the vertex with key2
	template <typename T, typename U>
	void Graph<T, U>::removeEdge(const T& key1, const T& key2)
	{
		if (vertTable.contains(key1) && vertTable.contains(key2))
		{
			vertTable[key1]->edges.remove(key2);
		}
		else
			throw std::invalid_argument("One or both of the given keys are not valid");
	}

	// Clears all vertices and edges from the graph
	template <typename T, typename U>
	void Graph<T, U>::clear()
	{
		for (auto& key : vertTable)
		{
			delete vertTable[key];
		}
		vertTable.clear();
		numVerts = 0;
	}

	// Vertex class implementation

	template <typename T, typename U>
	Graph<T, U>::Vertex::Vertex(const U& value) :
		value{ value }
	{}

	template <typename T, typename U>
	Graph<T, U>::Vertex::Vertex(U&& value) :
		value{ value }
	{}
}
#endif