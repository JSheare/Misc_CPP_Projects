#ifndef VERTEX_H
#define VERTEX_H

#include <stdexcept>

#include "List.h"
#include "Edge.h"

namespace JADT
{
	template <typename T, typename U>
	class Vertex
	{
	private:
		T m_key{};
		U m_value{};
		List<Edge<T, U>> m_connections{};

		// Copy constructor (prohibited)
		Vertex(Vertex<T, U>& vertex)
		{}

		// Move constructor (prohibited)
		Vertex(Vertex<T, U>&& vertex)
		{}

		// Copy assignment (prohibited)
		Vertex<T, U>& operator=(Vertex<T, U>& vertex);

		// Move assignment (prohibited)
		Vertex<T, U>& operator=(Vertex<T, U>&& vertex);

	public:
		Vertex(T key)
			: m_key{ key }
		{}
		
		Vertex(T key, U value)
			: m_key{ key }, m_value{ value }
		{}

		~Vertex() = default;

		friend bool operator==(const Vertex<T, U>& vert1, const Vertex<T, U>& vert2)
		{
			return (	vert1.m_key == vert2.m_key
				&& vert1.m_value == vert2.m_value
				&& vert1.m_connections == vert2.m_connections);
		}

		friend bool operator!=(const Vertex<T, U>& vert1, const Vertex<T, U>& vert2)
		{
			return !(operator==(vert1, vert2));
		}

		U getValue() const { return m_value; }

		void setValue(U value) { m_value = value; }

		T getKey() const { return m_key; }

		void addEdge(Vertex<T, U>& vertex, int weight)
		{
			m_connections.append(Edge<T, U>{vertex, weight});
		}

		void removeEdge(Vertex<T, U>& vertex)
		{
			m_connections.remove(getEdgeIndex(vertex));
		}

		const List<Edge<T, U>>& getConnections() const
		{
			return m_connections;
		}

		bool adjacent(const Vertex<T, U>& vertex) const
		{
			for (int index{0}; index < m_connections.length(); ++index)
			{
				if (m_connections[index].m_edge == vertex)
				{
					return true;
				}
			}

			return false;
		}

		int getEdgeWeight(const Vertex<T, U>& vertex) const
		{
			return m_connections[getEdgeIndex(vertex)].m_weight;
		}

	private:
		int getEdgeIndex(Vertex<T, U>& vertex) const
		{
			for (int index{ 0 }; index < m_connections.length(); ++index)
			{
				if (m_connections[index].m_edge == vertex)
				{
					return index;
				}
			}

			throw std::invalid_argument("No edge to argued vertex");
		}
	};
}

#endif
