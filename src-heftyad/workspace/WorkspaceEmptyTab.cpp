#include "WorkspaceEmptyTab.h"

WorkspaceEmptyTab::WorkspaceEmptyTab(QWidget *parent)
    : GridWidget(parent)
{
    m_allowedActions = None;

    gridLayout()->setMargin(5);
    addWidget(&m_splitter, 0, 0);

    m_splitter.setOrientation(Qt::Horizontal);
}

WorkspaceEmptyTab::WorkspaceTabAction WorkspaceEmptyTab::allowedActions() const {return m_allowedActions;}
bool WorkspaceEmptyTab::allowAction(WorkspaceEmptyTab::WorkspaceTabAction action) const {return m_allowedActions & action;}
bool WorkspaceEmptyTab::allowCloseAll() const {return allowAction(CloseAll);}
bool WorkspaceEmptyTab::allowSaveAll() const {return allowAction(SaveAll);}

QList<QAction*> WorkspaceEmptyTab::contextMenuActions() const {return QList<QAction*>();}
void WorkspaceEmptyTab::retranslate(){}

bool WorkspaceEmptyTab::hasDirtyWork() const {return false;}
bool WorkspaceEmptyTab::hasOpenedWork() const {return false;}

void WorkspaceEmptyTab::saveAllWorks() {}
void WorkspaceEmptyTab::closeAllWorks() {}
