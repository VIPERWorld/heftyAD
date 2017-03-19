#include "Resource.h"
#include "WorkArrayView.h"

#include <QAction>

WorkArrayView::WorkArrayView(QWidget *parent)
    : ArrayView(parent)
{
    createModelRelatedActions();
}

WorkArrayView::~WorkArrayView()
{
}

void WorkArrayView::retranslate()
{
    View::retranslate();
    retranslateModelRelatedActions();
}

QList<QAction*> WorkArrayView::modelRelatedActions() const
{
    return QList<QAction*>() << m_actionGroup1.actions() << m_separator1
                             << m_actionGroup2.actions() << m_separator2
                             << m_actionGroup3.actions();
}

void WorkArrayView::createModelRelatedActions()
{
    // Action group 1

    m_actionGroup1.setAcceptNoSelection(true);

    m_addItem = new QAction(&m_actionGroup1);
    m_textEdition = new QAction(&m_actionGroup1);
    m_removeItem = new QAction(&m_actionGroup1);

    m_addItem->setCheckable(true);
    m_textEdition->setCheckable(true);

    m_addItem->setIcon(Resource::instance().toolBarIcon(".png"));
    m_textEdition->setIcon(Resource::instance().toolBarIcon(".png"));
    m_removeItem->setIcon(Resource::instance().toolBarIcon("remove_item.png"));

    connect(m_addItem,     &QAction::toggled,   this, &WorkArrayView::setOnMousePressedAddItem);
    connect(m_textEdition, &QAction::toggled,   this, &WorkArrayView::setEnabledItemEdition);
    connect(m_removeItem,  &QAction::triggered, this, &WorkArrayView::removeSelectedItems);

    // Separator

    m_separator1 = new QAction("", this);
        m_separator1->setSeparator(true);

    // Action group 2

    m_actionGroup2.setAcceptNoSelection(true);

    m_showItemIndexes = new QAction(&m_actionGroup2);
    m_showItemsAsInArray = new QAction(&m_actionGroup2);

    m_showItemIndexes->setCheckable(true);

    m_showItemIndexes->setIcon(Resource::instance().toolBarIcon(".png"));
    m_showItemsAsInArray->setIcon(Resource::instance().toolBarIcon(".png"));

    connect(m_showItemIndexes,    &QAction::toggled,   this, &WorkArrayView::setVisibleItemIndexesInArray);
    connect(m_showItemsAsInArray, &QAction::triggered, this, &WorkArrayView::showItemsAsInArray);

    // Separator

    m_separator2 = new QAction("", this);
        m_separator2->setSeparator(true);

    // Action group 3

    m_actionGroup3.setAcceptNoSelection(true);

    m_shuffleItems = new QAction(&m_actionGroup3);
    m_sortItemsByPos = new QAction(&m_actionGroup3);
    m_sortItemsByValue = new QAction(&m_actionGroup3);

    m_shuffleItems->setIcon(Resource::instance().toolBarIcon(".png"));
    m_sortItemsByPos->setIcon(Resource::instance().toolBarIcon(".png"));
    m_sortItemsByValue->setIcon(Resource::instance().toolBarIcon(".png"));

    connect(m_shuffleItems,     &QAction::triggered, this, &WorkArrayView::shuffleItems);
    connect(m_sortItemsByPos,   &QAction::triggered, this, &WorkArrayView::sortItemsBySceneRectX);
    connect(m_sortItemsByValue, &QAction::triggered, this, &WorkArrayView::sortItemsByValue);
}

void WorkArrayView::retranslateModelRelatedActions()
{
    m_addItem->setText(trUtf8("Ajouter un item"));
    m_textEdition->setText(trUtf8("Modifier le contenu des items"));
    m_removeItem->setText(trUtf8("Supprimer les items sélectionnés"));

    m_showItemIndexes->setText(trUtf8("Afficher les indexes des items"));
    m_showItemsAsInArray->setText(trUtf8("Afficher les items comme dans un tableau"));

    m_shuffleItems->setText(trUtf8("Mélanger les items aléatoirement"));
    m_sortItemsByPos->setText(trUtf8("Ordonner les items à partir de leur position"));
    m_sortItemsByValue->setText(trUtf8("Ordonner les items à partir de leur valeur associée"));
}
