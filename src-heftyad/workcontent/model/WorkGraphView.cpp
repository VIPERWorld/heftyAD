#include "Resource.h"
#include "WorkGraphView.h"

WorkGraphView::WorkGraphView(QWidget *parent)
    : GraphView(parent)
{
    createModelRelatedActions();
    createSelectionRelatedActions();
}

WorkGraphView::~WorkGraphView()
{
}

void WorkGraphView::retranslate()
{
    View::retranslate();
    retranslateModelRelatedActions();
    retranslateSelectionRelatedActions();
}

QList<QAction*> WorkGraphView::modelRelatedActions() const
{
    return QList<QAction*>() << m_actionGroup1.actions()
                             << m_separator
                             << m_actionGroup2.actions();
}

QList<QAction*> WorkGraphView::selectionRelatedActions() const
{
    return QList<QAction*>() << m_selectVertices << m_selectEdges;
}

void WorkGraphView::createModelRelatedActions()
{
    // Action group 1

    m_actionGroup1.setAcceptNoSelection(true);

    m_addNode = new QAction(&m_actionGroup1);
    m_addEdge = new QAction(&m_actionGroup1);
    m_addEdgeSuccessively = new QAction(&m_actionGroup1);
    m_addEdgeFromSourceOnMousePressed = new QAction(&m_actionGroup1);
    m_textEdition = new QAction(&m_actionGroup1);
    m_removeItem = new QAction(&m_actionGroup1);

    m_addNode->setCheckable(true);
    m_addEdge->setCheckable(true);
    m_addEdgeSuccessively->setCheckable(true);
    m_addEdgeFromSourceOnMousePressed->setCheckable(true);
    m_textEdition->setCheckable(true);

    m_addNode->setIcon(Resource::instance().toolBarIcon("node.png"));
    m_addEdge->setIcon(Resource::instance().toolBarIcon("curve.png"));
    m_addEdgeSuccessively->setIcon(Resource::instance().toolBarIcon("curve_continuous.png"));
    m_addEdgeFromSourceOnMousePressed->setIcon(Resource::instance().toolBarIcon("curve_sourced.png"));
    m_textEdition->setIcon(Resource::instance().toolBarIcon(".png"));
    m_removeItem->setIcon(Resource::instance().toolBarIcon("remove_item.png"));

    connect(m_addNode,                         &QAction::toggled,   this, &WorkGraphView::setOnMousePressedAddNode);
    connect(m_addEdge,                         &QAction::toggled,   this, &WorkGraphView::setOnMousePressedAddEdgeTwoByTwo);
    connect(m_addEdgeSuccessively,             &QAction::toggled,   this, &WorkGraphView::setOnMousePressedAddEdgeSuccessively);
    connect(m_addEdgeFromSourceOnMousePressed, &QAction::toggled,   this, &WorkGraphView::setOnMousePressedAddEdgeFromSource);
    connect(m_textEdition,                     &QAction::toggled,   this, &WorkGraphView::setEnabledItemEdition);
    connect(m_removeItem,                      &QAction::triggered, this, &WorkGraphView::removeSelectedItems);

    // Separator

    m_separator = new QAction("", this);
        m_separator->setSeparator(true);

    // Action group 2

    m_actionGroup2.setAcceptNoSelection(true);

    m_showEdgePoints = new QAction(&m_actionGroup2);
    m_showEdgePoints->setCheckable(true);
    m_showEdgePoints->setIcon(Resource::instance().toolBarIcon(".png"));
    connect(m_showEdgePoints, &QAction::toggled,   this, &WorkGraphView::setVisibleEdgePoints);

    m_actionGroup2.setSelectedAction(m_showEdgePoints);
}

void WorkGraphView::retranslateModelRelatedActions()
{
    m_addNode->setText(trUtf8("Ajouter des sommets"));
    m_addEdge->setText(trUtf8("Ajouter des arcs"));
    m_addEdgeSuccessively->setText(trUtf8("Ajouter des arcs continuellement"));
    m_addEdgeFromSourceOnMousePressed->setText(trUtf8("Ajouter des arcs à partir d'une source"));
    m_textEdition->setText(trUtf8("Modifier le contenu d'un item"));
    m_removeItem->setText(trUtf8("Supprimer les items sélectionnés"));

    m_showEdgePoints->setText(trUtf8("Afficher/Cacher les noeuds sur les arcs"));
}

void WorkGraphView::createSelectionRelatedActions()
{
    m_selectVertices = new QAction("", this);
    m_selectEdges    = new QAction("", this);

    m_selectVertices->setIcon(Resource::instance().toolBarIcon(".png"));
    m_selectEdges   ->setIcon(Resource::instance().toolBarIcon(".png"));

    connect(m_selectVertices, &QAction::triggered, this, &WorkGraphView::selectVertices);
    connect(m_selectEdges,    &QAction::triggered, this, &WorkGraphView::selectEdges);
}

void WorkGraphView::retranslateSelectionRelatedActions()
{
    m_selectVertices->setText(trUtf8("Sélectionner les sommets"));
    m_selectEdges   ->setText(trUtf8("Sélectionner les arcs"));
}
