#include "AlgorithmWorkViewer.h"

#include <QTreeView>

AlgorithmWorkViewer::AlgorithmWorkViewer(QWidget *parent)
    : ModelViewer(parent)
{
    m_meta = new QStandardItem;
    m_meta->setEditable(false);
    m_itemModel.appendRow(m_meta);

    m_sources = new QStandardItem;
    m_sources->setEditable(false);
    m_itemModel.appendRow(m_sources);

    auto *view = new QTreeView;
    setModel(&m_itemModel);
    setView(view);
    ModelViewer::configure(view);
    connect(view, &QTreeView::pressed, this, &AlgorithmWorkViewer::viewItemPressed);
}

QStandardItemModel* AlgorithmWorkViewer::itemModel() const {return const_cast<QStandardItemModel*>(&m_itemModel);}

void AlgorithmWorkViewer::addItem(QStandardItem *item, int category)
{
    switch(category) {
    case MetaData: m_meta->appendRow(item);    break;
    case Sources:  m_sources->appendRow(item); break;
    }
}

void AlgorithmWorkViewer::retranslate()
{
    m_meta->setText(trUtf8("métadonnées"));
    m_sources->setText(trUtf8("sources"));
}
