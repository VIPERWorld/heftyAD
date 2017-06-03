#include "WorkspaceEmptyTab.h"

WorkspaceEmptyTab::WorkspaceEmptyTab(QWidget *parent)
    : GridWidget(parent)
{
    gridLayout()->setMargin(5);
    addWidget(&m_splitter, 0, 0);

    m_splitter.setOrientation(Qt::Horizontal);
}

QList<QAction*> WorkspaceEmptyTab::contextMenuActions() const {return QList<QAction*>();}
void WorkspaceEmptyTab::retranslate(){}
