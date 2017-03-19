#include "AlgorithmWorkFileViewer.h"

#include <QTreeView>

AlgorithmWorkFileViewer::AlgorithmWorkFileViewer(QWidget *parent)
    : ModelViewer(parent)
{
    m_global = new QStandardItem;
    m_global->setEditable(false);
    m_fileModel.appendRow(m_global);

    m_sources = new QStandardItem;
    m_sources->setEditable(false);
    m_fileModel.appendRow(m_sources);

    auto *view = new QTreeView;
    setModel(&m_fileModel);
    setView(view);
    ModelViewer::configure(view);
    connect(view, &QTreeView::doubleClicked, this, &AlgorithmWorkFileViewer::fileItemDoubleClicked);
}

QStandardItemModel* AlgorithmWorkFileViewer::fileModel() const {return const_cast<QStandardItemModel*>(&m_fileModel);}

void AlgorithmWorkFileViewer::addFileItem(QStandardItem *item, int category)
{
    switch(category) {
    case Global:  m_global->appendRow(item);  break;
    case Sources: m_sources->appendRow(item); break;
    }
}

void AlgorithmWorkFileViewer::retranslate()
{
    m_global->setText(trUtf8("algorithme"));
    m_sources->setText(trUtf8("sources"));
}
