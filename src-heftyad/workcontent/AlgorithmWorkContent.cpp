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

void AlgorithmWorkContent::fillViewer()
{
    addItem("metadata");
    addItem("preExecute");
    addItem("execute");
    addItem("postExecute");
}

QStandardItem* AlgorithmWorkContent::addItem(const QString &name, const QString &editorText)
{
    auto *item = new QStandardItem(name);
    item->setEditable(false);
    m_viewer.addItem(item);

    auto *editor = new SciCodeEditor(this);
    editor->setText(editorText);
    m_map[item] = editor;
    m_editors.addWidget(editor);

    return item;
}

void AlgorithmWorkContent::onViewItemPressed(const QModelIndex &index)
{
    QStandardItem *item = m_viewer.itemModel()->itemFromIndex(index);
    if(m_map.contains(item)) {
        SciCodeEditor *editor = m_map[item];
        editor->setFocus();
        m_editors.setCurrentWidget(editor);
    }
}
