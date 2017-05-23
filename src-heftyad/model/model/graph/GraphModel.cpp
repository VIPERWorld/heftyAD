#include "GraphModel.h"
#include "GraphModelEdge.h"
#include "GraphModelVertex.h"
#include "GraphModelXmlReader.h"
#include "GraphModelXmlWriter.h"

GraphModel::GraphModel(QObject *parent)
    : Model(parent)
{
    m_reader = new GraphModelXmlReader(this);
    m_writer = new GraphModelXmlWriter(this);
}

GraphModel::~GraphModel()
{
    clear(false); // do note need to delete view since QGraphicsView handle that
    m_itemsToBeFreed.clear(); // "new"ed items (if any) will be destroyed

    delete m_reader;
    delete m_writer;
}

QVector<GraphModelVertex*> GraphModel::vertices() const {return m_vertices;}

QVector<GraphModelVertex*> GraphModel::vertices(std::function<bool (const GraphModelVertex&)> predicate) const
{
    QVector<GraphModelVertex*> res;
    for(auto *vertex : m_vertices) {
        if(predicate(*vertex)) {
            res.push_back(vertex);
        }
    }

    return res;
}

int GraphModel::vertexCount() const {return m_vertices.count();}

QVector<GraphModelEdge*> GraphModel::edges() const {return m_edges;}

QVector<GraphModelEdge*> GraphModel::edges(std::function<bool (const GraphModelEdge&)> predicate) const
{
    QVector<GraphModelEdge*> res;
    for(auto *edge : m_edges) {
        if(predicate(*edge)) {
            res.push_back(edge);
        }
    }

    return res;
}

QVector<GraphModelEdge*> GraphModel::edges(bool from, bool to, const GraphModelVertex &vertex, bool removeDuplicates) const
{
    /*
     * The vector computed below may contain duplicates:
     *     when from = to = true AND
     *     if there is an edge which can be walked through in both directions (bidirectional edge).
     */
    const QVector<GraphModelEdge*> &vect = edges([&] (const GraphModelEdge &edge) -> bool {
        const bool ok1 = from && edge.startsAt(vertex);
        const bool ok2 = to   && edge.endsAt(vertex);
        return ok1 || ok2;
    });

    return !removeDuplicates ? vect : QList<GraphModelEdge*>::fromVector(vect).toSet().toList().toVector();
}

QVector<GraphModelEdge*> GraphModel::edges(const GraphModelVertex &source, const GraphModelVertex &target) const
{
    return edges([&] (const GraphModelEdge &edge) -> bool {
        return edge.startsAt(source) && edge.endsAt(target);
    });
}

int GraphModel::edgeCount() const {return m_edges.count();}

bool GraphModel::contains(const GraphModelVertex &vertex) const
{
    for(auto *v : m_vertices) {
        if(v == &vertex) {
            return true;
        }
    }
    return false;
}

bool GraphModel::contains(const GraphModelEdge &edge) const
{
    for(auto *e : m_edges) {
        if(e == &edge) {
            return true;
        }
    }
    return false;
}

bool GraphModel::adjacent(const GraphModelVertex &vertex1, const GraphModelVertex &vertex2) const
{
    for(auto *edge : m_edges) {
        if( (edge->startsAt(vertex1) && edge->endsAt(vertex2)) ||
            (edge->startsAt(vertex2) && edge->endsAt(vertex1)) ) {
            return true;
        }
    }
    return false;
}

int GraphModel::degree(const GraphModelVertex &vertex, bool in, bool out) const
{
    return edges(out, in, vertex, false).count();
}

bool GraphModel::addVertex(GraphModelVertex *vertex)
{
    bool conditionOK = vertex && !contains(*vertex);
    if(conditionOK) {
        m_vertices.push_back(vertex);
        emit vertexAdded(vertex);
    }

//    switch(vertexCount()) {
//    case 1: vertex->setBrushColor(QColor("#00ff00")); break;
//    case 2: vertex->setBrushColor(QColor("#00ff00")); break;
//    case 3: vertex->switchToShapeKind(ModelNode::Rectangle); break;
//    case 4: vertex->setBrushColor(QColor("#f5b800")); break;
//    case 5: vertex->setBrushColor(QColor("#f5b800")); break;
//    }

    return conditionOK;
}

bool GraphModel::addEdge(GraphModelEdge *edge)
{
    /*
     * We don't check whether the edge's endpoints is in the graph.
     * So nodes are supposed to be added first.
     */

    bool conditionOK = edge && !contains(*edge);
    if(conditionOK) {
        GraphModelVertex &v1(edge->vertex1());
        GraphModelVertex &v2(edge->vertex2());
        v1.m_succs.append(&v2);
        if(edge->isBidir()) {
            v2.m_succs.append(&v1);
        }

        m_edges.push_back(edge);
        emit edgeAdded(edge);
    }

    return conditionOK;
}

GraphModelVertex& GraphModel::addNewVertex()
{
    auto *vertex = new GraphModelVertex;
    registerItemToBeFreed(vertex);

    addVertex(vertex);
    return *vertex;
}

GraphModelEdge& GraphModel::addNewEdge(const GraphModelVertex &node1, const GraphModelVertex &node2, bool bidir)
{
    auto *edge = new GraphModelEdge(node1, node2, bidir);
    registerItemToBeFreed(edge);

    addEdge(edge);
    return *edge;
}

void GraphModel::remove(const GraphModelVertex &vertex, bool deleteViews)
{
    // Which edges should be removed (if the given vertex were to be removed)?

    QVector<GraphModelEdge*> uselessEdges;
    for(auto *edge : m_edges) {
        if(edge->startsAt(vertex) || edge->endsAt(vertex)) {
            uselessEdges.append(edge);
        }
    }

    // Now remove those edges

    for(auto *edge : uselessEdges) {
        remove(*edge, deleteViews);
    }

    // Finally remove the given vertex

    auto &vertex2(const_cast<GraphModelVertex&>(vertex));
    if(m_vertices.removeOne(&vertex2)) {
        emit vertexRemoved(&vertex2, deleteViews);
    }
}

void GraphModel::remove(const GraphModelEdge &edge, bool deleteView)
{
    GraphModelVertex &v1(edge.vertex1());
    GraphModelVertex &v2(edge.vertex2());

    v1.m_succs.removeOne(&v2);
    if(edge.isBidir()) {
        v2.m_succs.removeOne(&v1);
    }

    auto &edge2(const_cast<GraphModelEdge&>(edge));
    if(m_edges.removeOne(&edge2)) {
        emit edgeRemoved(&edge2, deleteView);
    }
}

void GraphModel::clear(bool deleteViews)
{
    if(m_vertices.isEmpty()) { // There are neither vertices nor edges
        return;
    }

    while(!m_vertices.isEmpty()) {
        remove(*m_vertices.first(), deleteViews);
    }

    emit cleared();
}

void GraphModel::empty() {clear();}

void GraphModel::registerItemToBeFreed(ModelItem *item)
{
    if(item != nullptr) {
        std::unique_ptr<ModelItem> uptr(item);
        m_itemsToBeFreed.push_back(std::move(uptr));
    }
}
