#include "WorkspaceAlgorithmViewer.h"

#include <QTreeView>

WorkspaceAlgorithmViewer::WorkspaceAlgorithmViewer(QWidget *parent)
    : ModelViewer(parent)
{
    m_sources = new QStandardItem;
    m_releases = new QStandardItem;

    m_viewerModel.appendRow(m_sources);
    m_viewerModel.appendRow(m_releases);

    setModel(&m_viewerModel);
    setView(new QTreeView);
    ModelViewer::configure(static_cast<QTreeView*>(view()));
}

void WorkspaceAlgorithmViewer::retranslate()
{
    m_sources->setText(trUtf8("Sources"));
    m_releases->setText(trUtf8("Releases"));
}
