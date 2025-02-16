#ifndef JADT_GRAPH_HPP
#define JADT_GRAPH_HPP

#include <stdexcept>

#include "List.h"
#include "Edge.h"
#include "Vertex.h"

namespace JADT
{
	template <typename T, typename U>
	Graph<T, U>::Graph()
		: numVerts{ 0 }
	{}

	// Move constructor
	template <typename T, typename U>
	Graph<T, U>::Graph(Graph<T, U>&& graph) noexcept
		: vertList{ graph.vertList }
		, vertKeys{ graph.vertKeys }
		, numVerts{ graph.numVerts }
	{
		graph.numVerts = 0;
	}

	template <typename T, typename U>
	Graph<T, U>::~Graph()
	{
		for (int index{ 0 }; index < numVerts; ++index)
		{
			delete vertList[index];
		}
	}

	// Move assignment
	template <typename T, typename U>
	Graph<T, U>& Graph<T, U>::operator=(Graph<T, U>&& graph)
	{
		vertList = graph.vertList;
		vertKeys = graph.vertKeys;
		numVerts = graph.numVerts;

		graph.numVerts = 0;
	}

	template <typename T, typename U>
	U Graph<T, U>::operator[](T key) const
	{
		return getVertexValue(key);
	}

	// Adds a vertex with the given key to the graph
	template <typename T, typename U>
	void Graph<T, U>::addVertex(T key)
	{
		if (!contains(key))
		{
			Vertex<T, U>* vertPtr = new Vertex<T, U>(key);
			vertList.append(vertPtr);
			vertKeys.append(key);
			++numVerts;
		}
	}

	// Adds a vertex with the given key and value to the graph
	template <typename T, typename U>
	void Graph<T, U>::addVertex(T key, U value)
	{
		if (!contains(key))
		{
			Vertex<T, U>* vertPtr = new Vertex<T, U>(key, value);
			vertList.append(vertPtr);
			vertKeys.append(key);
			++numVerts;
		}
	}

	// Returns the value stored in the vertex with the given key
	template <typename T, typename U>
	U Graph<T, U>::getVertexValue(T key) const
	{
		return getVertex(key).getValue();
	}

	// Sets the value of the vertex with the given key
	template <typename T, typename U>
	void Graph<T, U>::setVertexValue(T key, U value)
	{
		if (!contains(key))
		{
			addVertex(key, value);
		}
		else
		{
			getVertex(key).setValue(value);
		}
	}

	// Removes the vertex with the given key from the graph
	template <typename T, typename U>
	void Graph<T, U>::removeVertex(T key)
	{
		if (contains(key))
		{
			Vertex<T, U>& targetVert{ getVertex(key) };
			for (T& vertKey : vertKeys)  // Removes any edges to the vertex we're removing
			{
				if (key == vertKey)
				{
					continue;
				}

				Vertex<T, U>& tempVert{ getVertex(vertKey) };
				if (tempVert.adjacent(targetVert))
				{
					tempVert.removeEdge(targetVert);
				}
			}

			int index{ vertKeys.index(key) };
			vertKeys.remove(index);
			delete vertList[index];
			vertList.remove(index);
			--numVerts;
		}
	}

	// Adds an edge from vertex1 (with key1) to vertex2 (with key2)
	template <typename T, typename U>
	void Graph<T, U>::addEdge(T key1, T key2, int weight)
	{
		Vertex<T, U>& vertex1{ getVertex(key1) };
		Vertex<T, U>& vertex2{ getVertex(key2) };

		if (!vertex1.adjacent(vertex2)) // Disallows multiple edges to the same node (i.e. no multigraphs)
		{
			vertex1.addEdge(vertex2, weight);
		}
	}

	// Adds an edge from vertex1 (with key1) to vertex2 (with key2) and vice versa
	template <typename T, typename U>
	void Graph<T, U>::addDoubleEdge(T key1, T key2, int weight1, int weight2)
	{
		Vertex<T, U>& vertex1{ getVertex(key1) };
		Vertex<T, U>& vertex2{ getVertex(key2) };

		if (!vertex1.adjacent(vertex2))
		{
			vertex1.addEdge(vertex2, weight1);
		}

		if (!vertex2.adjacent(vertex1))
		{
			vertex2.addEdge(vertex1, weight2);
		}
	}

	// Returns the weight of the edge connecting vertex1 (with key1) and vertex2 (with key2)
	template <typename T, typename U>
	int Graph<T, U>::getEdgeWeight(T key1, T key2)
	{
		return getVertex(key1).getEdgeWeight(getVertex(key2));
	}

	// Removes an edge (if it exists) from vertex1 (with key1) to vertex2 (with key2)
	template <typename T, typename U>
	void Graph<T, U>::removeEdge(T key1, T key2)
	{
		if (contains(key1) && contains(key2))
		{
			Vertex<T, U>& vertex1{ getVertex(key1) };
			Vertex<T, U>& vertex2{ getVertex(key2) };

			if (vertex1.adjacent(vertex2))
			{
				vertex1.removeEdge(vertex2);
			}
		}
	}

	// Removes a double edge(if it exists) from vertex1 (with key1) to vertex2 (with key2) and vice versa
	template <typename T, typename U>
	void Graph<T, U>::removeDoubleEdge(T key1, T key2)
	{
		if (contains(key1) && contains(key2))
		{
			Vertex<T, U>& vertex1{ getVertex(key1) };
			Vertex<T, U>& vertex2{ getVertex(key2) };

			if (vertex1.adjacent(vertex2))
			{
				vertex1.removeEdge(vertex2);
			}

			if (vertex2.adjacent(vertex1))
			{
				vertex2.removeEdge(vertex1);
			}
		}
	}

	// Returns a list of vertex keys
	template <typename T, typename U>
	const List<T>& Graph<T, U>::getVertices() const
	{
		return vertKeys;
	}

	// Returns true if a vertex with the given key is in the graph
	template <typename T, typename U>
	bool Graph<T, U>::contains(T key) const
	{
		return vertKeys.contains(key);
	}

	// Returns the number of vertices in the graph
	template <typename T, typename U>
	int Graph<T, U>::size() const { return numVerts; }

	// Returns true if vertex1 (with key1) has an edge pointing to vertex2 (with key2)
	template <typename T, typename U>
	bool Graph<T, U>::adjacent(T key1, T key2) const
	{
		return getVertex(key1).adjacent(getVertex(key2));
	}

	// Returns a list of vertices that vertex1 (with key1) has edges pointing towards
	template <typename T, typename U>
	List<T> Graph<T, U>::neighbors(T key) const
	{
		List<T> adjKeys{};
		const Vertex<T, U>& vert{ getVertex(key) };
		for (int index{ 0 }; index < vert.getConnections().length(); ++index)
		{
			adjKeys.append(vert.getConnections()[index].edge.getKey());
		}

		return adjKeys;
	}

	// Returns a reference to the vertex with the given key
	template <typename T, typename U>
	Vertex<T, U>& Graph<T, U>::getVertex(T key)
	{
		int index{ vertKeys.index(key) };
		if (index == -1)
		{
			throw std::invalid_argument("Graph has no vertex with the given key");
		}
		else
		{
			return *(vertList[index]);
		}
	}

	// Returns a constant reference to the vertex with the given key
	template <typename T, typename U>
	const Vertex<T, U>& Graph<T, U>::getVertex(T key) const
	{
		int index{ vertKeys.index(key) };
		if (index == -1)
		{
			throw std::invalid_argument("Graph has no vertex with the given key");
		}
		else
		{
			return *(vertList[index]);
		}
	}
}
#endif