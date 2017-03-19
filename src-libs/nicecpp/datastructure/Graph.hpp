#ifndef GRAPH_H
#define GRAPH_H

#include "nicecpplib_global.h"

#include "ConnectionArea.hpp"

namespace ncpp
{
/**
 * V: Vertex
 * E: Edge
 */
template<typename V, typename E>
class NICECPPLIBSHARED_EXPORT Graph
{
private:
    ConnectionArea<V, E> m_area;

public:
    /*
     ==========
     Properties
     ==========
     */

    const std::vector<V>& getPropertyVertexNeighbors(const V &vertex) const {
        return m_area.getPropertyNodeNeighbors(vertex);
    }

    const V& getPropertyEdgeSource(const E &edge) const {
        return m_area.getPropertyConnectionSource(edge);
    }

    const V& getPropertyEdgeTarget(const E &edge) const {
        return m_area.getPropertyConnectionTarget(edge);
    }

    /*
     ===================
     Querying this graph
     ===================
     */

    std::vector<V> vertices(void) const noexcept {return m_area.nodes();}

    std::vector<V> vertices(std::function<bool (const V&)> predicate) const noexcept {
        return m_area.nodes(predicate);
    }

    std::vector<V> vertexSources(const V &target) const noexcept {
        return m_area.nodeSources(target);
    }

    std::vector<V> vertexTargets(const V &source) const noexcept {
        return m_area.nodeTargets(source);
    }

    /**
     * Returns the same value as vertexSources(vertex).
     *
     * Indeed in a graph, a vertex u is said to be adjacent to a vertex v
     * if there is an edge leaving u and comming to v (an edge from u to v).
     */
    std::vector<V> verticesAdjacentTo(const V &vertex) const noexcept {
        return vertexSources(vertex);
    }

    int vertexCount(void) const noexcept {return m_area.nodeCount();}

    std::vector<E> edges(void) const noexcept {return m_area.connections();}

    std::vector<E> edges(std::function<bool (const E&)> predicate) const noexcept {
        return m_area.connections(predicate);
    }

    std::vector<E> edges(const V &source, const V &target) const noexcept {
        return m_area.connections(source, target);
    }

    std::vector<E> edges(bool from, bool to, const V &vertex) const noexcept {
        return m_area.connections(from, to, vertex);
    }

    /**
     * Returns the same value as edges(from, to, vertex).
     *
     * Indeed in a graph, an edge e is said to be incident
     *     from a vertex v: if that edge leaves v (i.e. v is its source).
     *     to   a vertex v: if that edge comes to v (i.e. v is its target).
     *     with a vertex v: if that edge is incident both from and to v.
     */
    std::vector<E> edgesIncident(bool from, bool to, const V &vertex) const noexcept {
        return edges(from, to, vertex);
    }

    int edgeCount(void) const noexcept {return m_area.connectionCount();}

    int edgeCount(std::function<bool (const E&)> predicate) const noexcept {
        return m_area.connectionCount(predicate);
    }

    int edgeCount(const V &source, const V &target) const noexcept {
        return m_area.connectionCount(source, target);
    }

    int edgeCount(bool from, bool to, const V &vertex) const noexcept {
        return m_area.connectionCount(from, to, vertex);
    }

    bool containsVertex(const V &vertex) const noexcept {
        return m_area.containsNode(vertex);
    }

    bool containsEdge(const E &edge) const noexcept {
        return m_area.containsConnection(edge);
    }

    /**
     * Tells whether there is an edge from the source vertex to the target vertex.
     */
    bool adjacent(const V &source, const V &target) const noexcept {
        return m_area.connected(source, target);
    }

    /**
     * Returns the number of edges going from vertex source to vertex target.
     */
    int adjacentCount(const V &source, const V &target) const noexcept {
        return m_area.connectionCount(source, target);
    }

    /**
     * Returns the number of edges incident
     *     to   vertex (if in ONLY)
     *     from vertex (if out ONLY)
     *     with vertex (if in AND out)
     */
    int degree(const V &vertex, bool in = true, bool out = true) const noexcept {
        return m_area.connectionCount(out, in, vertex);
    }

    /*
     =============================
     Adding elements to this graph
     =============================
     */

    bool addVertex(const V &vertex) {
        return m_area.addNode(vertex);
    }

    bool addEdge(const E &edge, const V &source, const V &target) {
        return m_area.addConnection(edge, source, target);
    }

    /*
     =================================
     Removing elements from this graph
     =================================
     */

    bool removeVertex(const V &vertex) {
        return m_area.removeNode(vertex);
    }

    bool removeEdge(const E &edge) {
        return m_area.removeConnection(edge);
    }

    void clear(void) {
        m_area.clear();
    }
};
}

#endif // GRAPH_H
