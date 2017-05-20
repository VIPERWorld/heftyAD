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

    m_addNodes = new QAction(&m_actionGroup1);
    m_addEdges = new QAction(&m_actionGroup1);
    m_addEdgesSuccessively = new QAction(&m_actionGroup1);
    m_addEdgeFromSourceOnMousePressed = new QAction(&m_actionGroup1);
    m_textEdition = new QAction(&m_actionGroup1);
    m_removeItems = new QAction(&m_actionGroup1);

    m_addNodes->setCheckable(true);
    m_addEdges->setCheckable(true);
    m_addEdgesSuccessively->setCheckable(true);
    m_addEdgeFromSourceOnMousePressed->setCheckable(true);
    m_textEdition->setCheckable(true);

    m_addNodes->setIcon(Resource::instance().toolBarIcon("node.png"));
    m_addEdges->setIcon(Resource::instance().toolBarIcon("curve.png"));
    m_addEdgesSuccessively->setIcon(Resource::instance().toolBarIcon("curve_continuous.png"));
    m_addEdgeFromSourceOnMousePressed->setIcon(Resource::instance().toolBarIcon("curve_sourced.png"));
    m_textEdition->setIcon(Resource::instance().toolBarIcon(".png"));
    m_removeItems->setIcon(Resource::instance().toolBarIcon("remove_item.png"));

    connect(m_addNodes,                         &QAction::toggled,   this, &WorkGraphView::setOnMousePressedAddNode);
    connect(m_addEdges,                         &QAction::toggled,   this, &WorkGraphView::setOnMousePressedAddEdgeTwoByTwo);
    connect(m_addEdgesSuccessively,             &QAction::toggled,   this, &WorkGraphView::setOnMousePressedAddEdgeSuccessively);
    connect(m_addEdgeFromSourceOnMousePressed, &QAction::toggled,   this, &WorkGraphView::setOnMousePressedAddEdgeFromSource);
    connect(m_textEdition,                     &QAction::toggled,   this, &WorkGraphView::setEnabledItemEdition);
    connect(m_removeItems,                      &QAction::triggered, this, &WorkGraphView::removeSelectedItems);

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
    m_addNodes->setText(trUtf8("Ajouter des sommets"));
    m_addEdges->setText(trUtf8("Ajouter des arcs"));
    m_addEdgesSuccessively->setText(trUtf8("Ajouter des arcs continuellement"));
    m_addEdgeFromSourceOnMousePressed->setText(trUtf8("Ajouter des arcs à partir d'une source"));
    m_textEdition->setText(trUtf8("Modifier la valeur des items"));
    m_removeItems->setText(trUtf8("Supprimer les items sélectionnés"));

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
