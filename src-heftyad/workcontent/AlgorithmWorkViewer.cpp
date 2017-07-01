#include "AlgorithmWorkViewer.h"

#include <QTreeView>

AlgorithmWorkViewer::AlgorithmWorkViewer(QWidget *parent)
    : ModelViewer(parent)
{
    auto *view = new QTreeView;
    setModel(&m_itemModel);
    setView(view);
    ModelViewer::configure(view);
    connect(view, &QTreeView::pressed, this, &AlgorithmWorkViewer::viewItemPressed);
}

QStandardItemModel* AlgorithmWorkViewer::itemModel() const {return const_cast<QStandardItemModel*>(&m_itemModel);}
void AlgorithmWorkViewer::addItem(QStandardItem *item) {m_itemModel.appendRow(item);}
