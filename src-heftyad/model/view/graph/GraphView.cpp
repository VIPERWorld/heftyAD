#include "GraphModel.h"
#include "GraphModelEdge.h"
#include "GraphModelVertex.h"
#include "GraphView.h"
#include "GraphViewEdge.h"
#include "GraphViewVertex.h"

#include "command/UndoFactory.h"
#include "command/UndoStack.h"

#include <QMouseEvent>

GraphView::GraphView(QWidget *parent)
    : View(parent),
      m_modelAsGraph(nullptr)
{
    m_onMousePressedAddNode = false;
    m_onMousePressedAddEdgeTwoByTwo = false;
        resetLastlyPressedVertices();
    m_onMousePressedAddEdgeSuccessively = false;
    m_onMousePressedAddEdgeFromSource = false;

    m_visibleEdgePoints = false;

    // Speed up items adding, moving and removing (because most items move around most of the time)
    scene()->setItemIndexMethod(QGraphicsScene::NoIndex);

    setViewportUpdateMode(BoundingRectViewportUpdate);
    setTransformationAnchor(AnchorUnderMouse);
    //setCacheMode(CacheBackground); // if not commented, update in highlighting doesn't work
}

bool GraphView::acceptModel(Model *model) {return model==nullptr || dynamic_cast<GraphModel*>(model);}

void GraphView::mousePressEvent(QMouseEvent *event)
{
    View::mousePressEvent(event);

    const bool &mouseLeftButtonIsPressed = (event->buttons() & Qt::LeftButton);

    auto *vertexUnderMouse = dynamic_cast<GraphViewVertex*>(itemUnderMouseEvent(event));
    auto *edgeUnderMouse   = dynamic_cast<GraphViewEdge*>(itemUnderMouseEvent(event));
    Q_UNUSED(edgeUnderMouse)

    if(!mouseLeftButtonIsPressed || !vertexUnderMouse) {
        resetLastlyPressedVertices();
        if(!mouseLeftButtonIsPressed) {
            return;
        }
    }

    // Add nodes

    if(m_onMousePressedAddNode && !vertexUnderMouse) {
        GraphModelVertex &vertex = m_modelAsGraph->addNewVertex();
        //vertex.setPos(mapToScene(event->pos()));
        vertex.setPos(mapToScene(mapFromGlobal(event->globalPos())));
        return;
    }

    // Add edges

    if(m_onMousePressedAddEdgeTwoByTwo && vertexUnderMouse) {
        m_twoVerticesLastlyPressed[m_lastlyPressedVertexIndex+1] = vertexUnderMouse;
        m_lastlyPressedVertexIndex++;
        if(m_lastlyPressedVertexIndex == 1) {
            auto *vertex1 = m_twoVerticesLastlyPressed[0];
            auto *vertex2 = m_twoVerticesLastlyPressed[1];
            const bool &conditionOk = true; //vertex1 != vertex2
            if(conditionOk) {
                auto *from = (GraphModelVertex*)vertex1->modelItem();
                auto *to   = (GraphModelVertex*)vertex2->modelItem();
                m_modelAsGraph->addNewEdge(*from, *to);
                resetLastlyPressedVertices();
            }
            else {
                // If no edge were added between vertex1 and vertex2,
                // vertex2 will now be considered as source for next eventual edges.
                resetLastlyPressedVertices(vertex2, nullptr);
            }
        }
        return;
    }

    if(m_onMousePressedAddEdgeSuccessively && vertexUnderMouse) {
        m_twoVerticesLastlyPressed[m_lastlyPressedVertexIndex+1] = vertexUnderMouse;
        m_lastlyPressedVertexIndex++;
        if(m_lastlyPressedVertexIndex == 1) {
            auto *vertex1 = m_twoVerticesLastlyPressed[0];
            auto *vertex2 = m_twoVerticesLastlyPressed[1];
            const bool &conditionOk = true; //vertex1 != vertex2
            if(conditionOk) {
                auto *from = (GraphModelVertex*)vertex1->modelItem();
                auto *to   = (GraphModelVertex*)vertex2->modelItem();
                m_modelAsGraph->addNewEdge(*from, *to);
            }
            // vertex2 is now considered as source for next eventual edges.
            resetLastlyPressedVertices(vertex2, nullptr);
        }
        return;
    }

    if(m_onMousePressedAddEdgeFromSource && vertexUnderMouse) {
        if(m_twoVerticesLastlyPressed[0] == nullptr) {
            // Set the vertex under mouse as source for further edges
            m_twoVerticesLastlyPressed[0] = vertexUnderMouse;
        }
        else {
            auto *vertex1 = m_twoVerticesLastlyPressed[0];
            auto *vertex2 = vertexUnderMouse;
            const bool &conditionOk = true; //vertex1 != vertex2
            if(conditionOk) {
                auto *from = (GraphModelVertex*)vertex1->modelItem();
                auto *to   = (GraphModelVertex*)vertex2->modelItem();
                m_modelAsGraph->addNewEdge(*from, *to);
            }
        }
        return;
    }
}

void GraphView::selectVertices()
{
    if(m_modelAsGraph) {
        for(GraphModelVertex *vertex : m_modelAsGraph->vertices()) {
            vertex->setSelected(true);
        }
    }
}

void GraphView::selectEdges()
{
    if(m_modelAsGraph) {
        for(GraphModelEdge *edge : m_modelAsGraph->edges()) {
            edge->setSelected(true);
        }
    }
}

void GraphView::resetLastlyPressedVertices(GraphViewVertex *vertex0, GraphViewVertex *vertex1)
{
    if(!vertex0 && vertex1) { // We return immediately since there can't be a target node (node1) if there is no source node (node0)
        return;
    }

    m_twoVerticesLastlyPressed[0] = vertex0;
    m_twoVerticesLastlyPressed[1] = vertex1;

    m_lastlyPressedVertexIndex = -1;
    if(m_twoVerticesLastlyPressed[0]) {
        m_lastlyPressedVertexIndex = 0;
    }
    if(m_twoVerticesLastlyPressed[1]) {
        m_lastlyPressedVertexIndex = 1;
    }
}

#include <QDebug>
void GraphView::removeSelectedItems()
{
    qDebug() << "Supprimer items sélectionnés";
}

void GraphView::setEnabledItemMove(bool enabled)
{
    foreach(QGraphicsItem *item, items()) {
        const auto *vertexItem = dynamic_cast<GraphViewVertex*>(item);
        if(vertexItem) { // only vertex state is altered
            item->setFlag(QGraphicsItem::ItemIsMovable, enabled);
        }
    }
}

void GraphView::setVisibleEdgePoints(bool visible)
{
    m_visibleEdgePoints = visible;

    foreach(QGraphicsItem *item, items()) {
        auto *edge = dynamic_cast<GraphViewEdge*>(item);
        if(edge) {
            edge->setPointsVisibility(m_visibleEdgePoints);
        }
    }
}

void GraphView::setOnMousePressedAddNode(bool value)
{
    m_onMousePressedAddNode = value;
    setDragMode(m_onMousePressedAddNode ? NoDrag : RubberBandDrag);
}

void GraphView::setOnMousePressedAddEdgeTwoByTwo(bool value)
{
    m_onMousePressedAddEdgeTwoByTwo = value;
    resetLastlyPressedVertices();
    setEnabledItemMove(!m_onMousePressedAddEdgeTwoByTwo);
}

void GraphView::setOnMousePressedAddEdgeSuccessively(bool value)
{
    m_onMousePressedAddEdgeSuccessively = value;
    resetLastlyPressedVertices();
    setEnabledItemMove(!m_onMousePressedAddEdgeSuccessively);
}

void GraphView::setOnMousePressedAddEdgeFromSource(bool value)
{
    m_onMousePressedAddEdgeFromSource = value;
    resetLastlyPressedVertices();
    setEnabledItemMove(!m_onMousePressedAddEdgeFromSource);
}

void GraphView::onModelChanged()
{
    m_modelAsGraph = dynamic_cast<GraphModel*>(model());
    if(m_modelAsGraph) {
        connect(m_modelAsGraph, &GraphModel::vertexAdded,   this, &GraphView::onVertexAddedToModel);
        connect(m_modelAsGraph, &GraphModel::vertexRemoved, this, &GraphView::onVertexRemovedFromModel);
        connect(m_modelAsGraph, &GraphModel::edgeAdded,     this, &GraphView::onEdgeAddedToModel);
        connect(m_modelAsGraph, &GraphModel::edgeRemoved,   this, &GraphView::onEdgeRemovedFromModel);
        connect(m_modelAsGraph, &GraphModel::cleared,       this, &GraphView::onModelCleared);
    }

    View::onModelChanged();
}

void GraphView::onVertexAddedToModel(GraphModelVertex *vertex)
{
    auto *vertexView = new GraphViewVertex;
    vertexView->setModelItem(vertex);
    scene()->addItem(vertexView);

    /*
     * Register undo/redo
     */

    const bool dirty = isDirty();
    m_undoStack.push(ncpp::UndoFactory::createCommand(
        /**
         * Destructor body
         */
        []() {
            // There is nothing to do since
            //     model items deletion is handled by the model
            // AND item views are kept in the scene until QGraphicsScene removes and destroys them (in its destructor).
        },
        /**
         * Undo Add Vertex
         */
        [=]() {
            /*m_modelAsGraph->m_graph.removeVertex(vertex);
            vertexView->setVisible(false);*/

            setDirty(dirty);
        },
        /**
         * Redo Add Vertex
         */
        [=]() {
            /*m_modelAsGraph->m_graph.addVertex(vertex);
            vertexView->setVisible(true);*/

            setDirty(true);
        },
        "Add Graph Vertex"
    ));
    setDirty(true);
}

void GraphView::onEdgeAddedToModel(GraphModelEdge *edge)
{
    auto *edgeView = new GraphViewEdge;
    scene()->addItem(edgeView); // should be done before setting the model
    edgeView->setModelItem(edge);
    edgeView->adjust(); // to make sure the edge is well drawn

    edgeView->setPointsVisibility(m_visibleEdgePoints);

    /*
     * Register undo/redo
     */

    // TODO
}

void GraphView::onVertexRemovedFromModel()
{
    /*if(viewDeletionRequested) {
        ArrayViewItem *view = nullptr;
        for(QGraphicsItem *gi : items()) {
            view = dynamic_cast<ArrayViewItem*>(gi);
            if(view && view->modelItem()==item) {
                break;
            }
        }
        if(view) {
            scene()->removeItem(view);
            delete view;
        }
    }*/
}

void GraphView::onEdgeRemovedFromModel()
{
}

void GraphView::onModelCleared()
{
}
