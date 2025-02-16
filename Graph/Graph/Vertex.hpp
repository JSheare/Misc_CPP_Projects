#ifndef JADT_VERTEX_HPP
#define JADT_VERTEX_HPP

#include "List.h"
#include "Edge.h"

namespace JADT
{
	template <typename T, typename U>
	Vertex<T, U>::Vertex(T key)
		: key{ key }
	{}

	template <typename T, typename U>
	Vertex<T, U>::Vertex(T key, U value)
		: key{ key }, value{ value }
	{}

	template <typename T, typename U>
	U Vertex<T, U>::getValue() const 
	{ 
		return value; 
	}

	template <typename T, typename U>
	void Vertex<T, U>::setValue(U value) 
	{ 
		this->value = value; 
	}

	template <typename T, typename U>
	T Vertex<T, U>::getKey() const 
	{ 
		return key; 
	}

	template <typename T, typename U>
	void Vertex<T, U>::addEdge(Vertex<T, U>& vertex, int weight)
	{
		connections.append(Edge<T, U>{vertex, weight});
	}

	template <typename T, typename U>
	void Vertex<T, U>::removeEdge(Vertex<T, U>& vertex)
	{
		connections.remove(getEdgeIndex(vertex));
	}

	template <typename T, typename U>
	const List<Edge<T, U>>& Vertex<T, U>::getConnections() const
	{
		return connections;
	}

	template <typename T, typename U>
	bool Vertex<T, U>::adjacent(const Vertex<T, U>& vertex)
	{
		for (Edge<T, U>& edge : connections)
		{
			if (edge.edge == vertex)
			{
				return true;
			}
		}

		return false;
	}

	template <typename T, typename U>
	int Vertex<T, U>::getEdgeWeight(const Vertex<T, U>& vertex) const
	{
		return connections[getEdgeIndex(vertex)].weight;
	}

	template <typename T, typename U>
	int Vertex<T, U>::getEdgeIndex(Vertex<T, U>& vertex)
	{
		int index = { 0 };
		for (Edge<T, U>& edge : connections)
		{
			if (edge.edge == vertex)
			{
				return index;
			}

			++index;
		}

		throw std::invalid_argument("No edge to argued vertex");
	}
}
#endif