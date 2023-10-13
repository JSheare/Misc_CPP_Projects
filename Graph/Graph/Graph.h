#ifndef GRAPH_H
#define GRAPH_H

#include <stdexcept>

#include "List.h"
#include "Edge.h"
#include "Vertex.h"

namespace JADT
{
	template <typename T, typename U>
	class Graph
	{
	private:
		List<Vertex<T, U>*> m_vertList{};  // You should eventually redo this with a hash map. Same with the vertices
		List<T> m_vertKeys{};
		int m_numVerts{};

		// Copy constructor (prohibited)
		Graph(Graph<T, U>& graph)
		{}

		// Copy assignment (prohibited)
		Graph<T, U>& operator=(const Graph<T, U>& graph);

	public:
		Graph()
			: m_numVerts{ 0 }
		{}

		// Move constructor
		Graph(Graph<T, U>&& graph) noexcept
			: m_vertList{ graph.m_vertList }
			, m_vertKeys{ graph.m_vertKeys }
			, m_numVerts{ graph.m_numVerts }
		{
			graph.m_numVerts = 0;
		}

		~Graph()
		{
			for (int index{ 0 }; index < m_numVerts; ++index)
			{
				delete m_vertList[index];
			}
		}

		// Move assignment
		Graph<T, U>& operator=(Graph<T, U>&& graph)
		{
			m_vertList = graph.m_vertList;
			m_vertKeys = graph.m_vertKeys;
			m_numVerts = graph.m_numVerts;

			graph.m_numVerts = 0;
		}

		U operator[](T key) const
		{
			return getVertexValue(key);
		}

		// Adds a vertex with the given key to the graph
		void addVertex(T key)
		{
			if (!contains(key))
			{
				Vertex<T, U>* vertPtr = new Vertex<T, U>(key);
				m_vertList.append(vertPtr);
				m_vertKeys.append(key);
				++m_numVerts;
			}
		}

		// Adds a vertex with the given key and value to the graph
		void addVertex(T key, U value)
		{
			if (!contains(key))
			{
				Vertex<T, U>* vertPtr = new Vertex<T, U>(key, value);
				m_vertList.append(vertPtr);
				m_vertKeys.append(key);
				++m_numVerts;
			}
		}

		// Returns the value stored in the vertex with the given key
		U getVertexValue(T key) const
		{
			return getVertex(key).getValue();
		}

		// Sets the value of the vertex with the given key
		void setVertexValue(T key, U value)
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
		void removeVertex(T key)
		{
			if (contains(key))
			{
				Vertex<T, U>& targetVert{ getVertex(key) };
				for (T& vertKey : m_vertKeys)  // Removes any edges to the vertex we're removing
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

				int index{ m_vertKeys.index(key) };
				m_vertKeys.remove(index);
				delete m_vertList[index];
				m_vertList.remove(index);
				--m_numVerts;
			}
		}

		// Adds an edge from vertex1 (with key1) to vertex2 (with key2)
		void addEdge(T key1, T key2, int weight = 0)
		{
			Vertex<T, U>& vertex1{ getVertex(key1) };
			Vertex<T, U>& vertex2{ getVertex(key2) };

			if (!vertex1.adjacent(vertex2)) // Disallows multiple edges to the same node (i.e. no multigraphs)
			{
				vertex1.addEdge(vertex2, weight);
			}
		}

		// Adds an edge from vertex1 (with key1) to vertex2 (with key2) and vice versa
		void addDoubleEdge(T key1, T key2, int weight1 = 0, int weight2 = weight1)
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
		int getEdgeWeight(T key1, T key2)
		{
			return getVertex(key1).getEdgeWeight(getVertex(key2));
		}

		// Removes an edge (if it exists) from vertex1 (with key1) to vertex2 (with key2)
		void removeEdge(T key1, T key2)
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
		void removeDoubleEdge(T key1, T key2)
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
		const List<T>& getVertices() const
		{
			return m_vertKeys;
		}

		// Returns true if a vertex with the given key is in the graph
		bool contains(T key) const
		{
			return m_vertKeys.contains(key);
		}

		// Returns the number of vertices in the graph
		int size() const { return m_numVerts; }

		// Returns true if vertex1 (with key1) has an edge pointing to vertex2 (with key2)
		bool adjacent(T key1, T key2) const
		{
			return getVertex(key1).adjacent(getVertex(key2));
		}

		// Returns a list of vertices that vertex1 (with key1) has edges pointing towards
		List<T> neighbors(T key) const
		{
			List<T> adjKeys{};
			const Vertex<T, U>& vert{ getVertex(key) };
			for (int index{ 0 }; index < vert.getConnections().length(); ++index)
			{
				adjKeys.append(vert.getConnections()[index].m_edge.getKey());
			}

			return adjKeys;
		}

	private:
		// Returns a reference to the vertex with the given key
		Vertex<T, U>& getVertex(T key) 
		{
			int index{ m_vertKeys.index(key) };
			if (index == -1)
			{
				throw std::invalid_argument("Graph has no vertex with the given key");
			}
			else
			{
				return *(m_vertList[index]);
			}
		}

		// Returns a constant reference to the vertex with the given key
		const Vertex<T, U>& getVertex(T key) const
		{
			int index{ m_vertKeys.index(key) };
			if (index == -1)
			{
				throw std::invalid_argument("Graph has no vertex with the given key");
			}
			else
			{
				return *(m_vertList[index]);
			}
		}
	};
}

#endif
