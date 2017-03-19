#include "AlgorithmWorkContent.h"
#include "SciCodeEditor.h"

AlgorithmWorkContent::AlgorithmWorkContent(QWidget *parent)
    : GridWidget(parent)
{
    addWidget(&m_splitter, 0, 0);
    m_splitter.addWidget(&m_files);
    m_splitter.addWidget(&m_editors);

    m_splitter.setStretchFactor(1, 20);
    m_splitter.setCollapsible(1, false);

    initProject();

    connect(&m_files, &AlgorithmWorkFileViewer::fileItemDoubleClicked, this, &AlgorithmWorkContent::onFileItemDoubleClicked);
}

QStandardItem* AlgorithmWorkContent::addFile(const QString &fileName, int category)
{
    QStandardItem *item = new QStandardItem(fileName);

    m_files.addFileItem(item, category);
    auto *editor = new SciCodeEditor(this);
    m_map[item] = editor;
    m_editors.addWidget(editor);

    return item;
}

void AlgorithmWorkContent::retranslate()
{
    m_files.retranslate();
}

void AlgorithmWorkContent::initProject()
{
    QStandardItem *item = addGlobalFile("algo.xml");
    item->setEditable(false);
    m_map[item]->setText("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<algorithm>\n</algorithm>");

    addGlobalFile("config.xml")->setEditable(false);
}

QStandardItem* AlgorithmWorkContent::addGlobalFile(const QString &fileName) {return addFile(fileName, AlgorithmWorkFileViewer::Global);}
QStandardItem* AlgorithmWorkContent::addSourceFile(const QString &fileName) {return addFile(fileName, AlgorithmWorkFileViewer::Sources);}

void AlgorithmWorkContent::onFileItemDoubleClicked(const QModelIndex &index)
{
    QStandardItem *item = m_files.fileModel()->itemFromIndex(index);
    if(m_map.contains(item)) {
        SciCodeEditor *editor = m_map[item];
        editor->setFocus();
        m_editors.setCurrentWidget(editor);
    }
}
