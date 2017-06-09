#include "AlgorithmWorkContent.h"
#include "SciCodeEditor.h"

AlgorithmWorkContent::AlgorithmWorkContent(QWidget *parent)
    : GridWidget(parent)
{
    addWidget(&m_splitter, 0, 0);
    m_splitter.addWidget(&m_viewer);
    m_splitter.addWidget(&m_editors);

    m_splitter.setStretchFactor(1, 20);
    m_splitter.setCollapsible(1, false);

    fillViewer();

    connect(&m_viewer, &AlgorithmWorkViewer::viewItemPressed, this, &AlgorithmWorkContent::onViewItemPressed);
}

void AlgorithmWorkContent::retranslate()
{
    m_viewer.retranslate();
}

void AlgorithmWorkContent::fillViewer()
{
    // Add meta items

    QStandardItem *item;

    item = addMetaItem(trUtf8("infos"));
    item->setEditable(false);
    m_map[item]->setText("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<algorithm>\n</algorithm>");

    item = addMetaItem(trUtf8("config"));
    item->setEnabled(false);
    m_map[item]->setReadOnly(true);

    // Add source items

    addSourceItem("preExecute")->setEditable(false);
    addSourceItem("execute")->setEditable(false);
    addSourceItem("postExecute")->setEditable(false);
}

QStandardItem* AlgorithmWorkContent::addItem(const QString &name, int category)
{
    auto *item = new QStandardItem(name);

    m_viewer.addItem(item, category);
    auto *editor = new SciCodeEditor(this);
    m_map[item] = editor;
    m_editors.addWidget(editor);

    return item;
}
QStandardItem* AlgorithmWorkContent::addMetaItem(const QString &name) {return addItem(name, AlgorithmWorkViewer::MetaData);}
QStandardItem* AlgorithmWorkContent::addSourceItem(const QString &name) {return addItem(name, AlgorithmWorkViewer::Sources);}

void AlgorithmWorkContent::onViewItemPressed(const QModelIndex &index)
{
    QStandardItem *item = m_viewer.itemModel()->itemFromIndex(index);
    if(m_map.contains(item)) {
        SciCodeEditor *editor = m_map[item];
        editor->setFocus();
        m_editors.setCurrentWidget(editor);
    }
}
