#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include "Model.h"

#include <memory> // for std::unique_ptr

class GraphModelEdge;
class GraphModelVertex;
class ModelItem;

class GraphModel: public Model
{
    Q_OBJECT

    //friend class GraphView; // required to perform undo/redo

protected:
    QVector<GraphModelVertex*> m_vertices;
    QVector<GraphModelEdge*> m_edges;

    std::vector<std::unique_ptr<ModelItem>> m_itemsToBeFreed; // vertices and edges to be freed

public:
    GraphModel(QObject *parent = 0);
    ~GraphModel();

    QVector<GraphModelVertex*> vertices() const;
    QVector<GraphModelVertex*> vertices(std::function<bool (const GraphModelVertex&)> predicate) const;
    int vertexCount() const;

    QVector<GraphModelEdge*> edges() const;
    QVector<GraphModelEdge*> edges(std::function<bool (const GraphModelEdge&)> predicate) const;
    QVector<GraphModelEdge*> edges(bool from, bool to, const GraphModelVertex &vertex, bool removeDuplicates = true) const;
    QVector<GraphModelEdge*> edges(const GraphModelVertex &source, const GraphModelVertex &target) const;
    int edgeCount() const;

    bool contains(const GraphModelVertex &vertex) const;
    bool contains(const GraphModelEdge &edge) const;

    /**
     * Tells whether the two vertices are adjacents.
     * Wikipedia -> https://en.wikipedia.org/wiki/Neighbourhood_(graph_theory)
     *     In graph theory, an adjacent vertex of a vertex v in a graph
     *     is a vertex that is connected to v by an edge.
     *
     * So this function tells wether there is an edge connecting vertex1 and vertex2.
     */
    bool adjacent(const GraphModelVertex &vertex1, const GraphModelVertex &vertex2) const;

    /**
     * Returns the number of edges incident
     *     to   vertex (if in ONLY)
     *     from vertex (if out ONLY)
     *     with vertex (if in AND out)
     *
     * Indeed in a graph, an edge e is said to be incident
     *     from a vertex v: if that edge leaves v (i.e. v is its source).
     *     to   a vertex v: if that edge comes to v (i.e. v is its target).
     *     with a vertex v: if that edge is incident both from and to v.
     *
     * NB: When in = out = true, bidirectional edges are counted twice.
     */
    int degree(const GraphModelVertex &vertex, bool in = true, bool out = true) const;

    bool addVertex(GraphModelVertex *vertex);
    bool addEdge(GraphModelEdge *edge);

    GraphModelVertex& addNewVertex();
    GraphModelEdge& addNewEdge(const GraphModelVertex &node1, const GraphModelVertex &node2, bool bidir = false);

    void remove(const GraphModelVertex &vertex, bool deleteViews = true);
    void remove(const GraphModelEdge &edge, bool deleteView = true);

    void clear(bool deleteViews = true);

    void empty() override;

protected:
    void registerItemToBeFreed(ModelItem *item);

signals:
    void vertexAdded(GraphModelVertex *vertex);
    void vertexRemoved(GraphModelVertex *vertex, bool viewDeletionRequested);

    void edgeAdded(GraphModelEdge *edge);
    void edgeRemoved(GraphModelEdge *edge, bool viewDeletionRequested);

    void cleared();
};

#endif // GRAPHMODEL_H
