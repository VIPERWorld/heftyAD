#ifndef WORKSPACEMODELTAB_H
#define WORKSPACEMODELTAB_H

#include "WorkspaceBasicTab.h"

#include <QMenu>

class WorkspaceModelTab : public WorkspaceBasicTab
{
    Q_OBJECT

protected:
    QMenu m_newModel;
        QAction *m_newAutomaton;
        QAction *m_newGraph;
        // ---
        QAction *m_newArray;
        QAction *m_newMatrix;
        // ---
        QAction *m_newMaze;
    QAction *m_loadModels;

public:
    explicit WorkspaceModelTab(QWidget *parent = 0);
    ~WorkspaceModelTab();

    QList<QAction*> contextMenuActions() const override;
    void retranslate() override;

private:
    void connectSignalsToSlots();

signals:
};

#endif // WORKSPACEMODELTAB_H
