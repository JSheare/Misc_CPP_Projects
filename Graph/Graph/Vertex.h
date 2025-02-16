#ifndef JADT_VERTEX_H
#define JADT_VERTEX_H

#include <stdexcept>

#include "List.h"
#include "Edge.h"

namespace JADT
{
	template <typename T, typename U>
	class Vertex
	{
	public:
		Vertex(T key);
		Vertex(T key, U value);

		friend bool operator==(const Vertex<T, U>& vert1, const Vertex<T, U>& vert2)
		{
			return (vert1.key == vert2.key
				&& vert1.value == vert2.value
				&& vert1.connections == vert2.connections);
		}

		friend bool operator!=(const Vertex<T, U>& vert1, const Vertex<T, U>& vert2)
		{
			return !(operator==(vert1, vert2));
		}

		U getValue() const;
		void setValue(U value);
		T getKey() const;
		void addEdge(Vertex<T, U>& vertex, int weight);
		void removeEdge(Vertex<T, U>& vertex);
		const List<Edge<T, U>>& getConnections() const;
		bool adjacent(const Vertex<T, U>& vertex);
		int getEdgeWeight(const Vertex<T, U>& vertex) const;

	private:
		T key{};
		U value{};
		List<Edge<T, U>> connections{};

		int getEdgeIndex(Vertex<T, U>& vertex);
		Vertex(Vertex<T, U>& vertex);  // Copy constructor (prohibited)
		Vertex(Vertex<T, U>&& vertex);  // Move constructor (prohibited)
		Vertex<T, U>& operator=(Vertex<T, U>& vertex);  // Copy assignment (prohibited)
		Vertex<T, U>& operator=(Vertex<T, U>&& vertex);  // Move assignment (prohibited)
	};
}
#include "Vertex.hpp"
#endif
