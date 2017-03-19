#include "ModelViewer.h"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QTreeView>

ModelViewer::ModelViewer(QWidget *parent)
    : GridWidget(parent),
      m_model(nullptr),
      m_view(nullptr)
{
}

QAbstractItemModel* ModelViewer::model(void) const {return m_model;}
void ModelViewer::setModel(QAbstractItemModel *model) {m_model = model;}

QAbstractItemView* ModelViewer::view(void) const {return m_view;}
void ModelViewer::setView(QAbstractItemView *view)
{
    QAbstractItemView *oldView = m_view;

    if(m_view) {
        removeWidget(m_view);
    }
    m_view = view;
    if(m_view) {
        m_view->setModel(m_model);
        addWidget(m_view, 0, 0);
    }

    emit viewChanged(oldView, m_view);
}

void ModelViewer::configure(QTreeView *view)
{
    const int c = view->header()->count();
    for(int i=1; i<c; i++) {
        view->header()->hideSection(i);
    }

    view->setHeaderHidden(true);
    view->setAnimated(true);
    view->setIndentation(20);
    view->setSortingEnabled(true);
    view->sortByColumn(0, Qt::AscendingOrder);
}
