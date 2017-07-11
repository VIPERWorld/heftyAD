#ifndef WORKSPACESIMULATIONTAB_H
#define WORKSPACESIMULATIONTAB_H

#include "WorkspaceEmptyTab.h"

#include <QMdiArea>
#include <QMenu>

class WorkspaceSimulationTab : public WorkspaceEmptyTab
{
    Q_OBJECT

protected:
    QMdiArea m_container;

    QAction *m_newSimulation;
    QMenu m_windows;
        QAction *m_tiledHor;
        QAction *m_tiledVer;
        QAction *m_closeAll;
    QAction *m_background;

public:
    explicit WorkspaceSimulationTab(QWidget *parent = 0);
    ~WorkspaceSimulationTab();

    QList<QAction*> contextMenuActions() const override;
    void retranslate() override;

    bool hasARunningSimulation() const;

private:
    void updateWindowsRelatedActions();

signals:

protected slots:
    void onNewSimulationActionTriggered();

    void tileSubWindowsHorizontally();
    void tileSubWindowsVertically();
};

#endif // WORKSPACESIMULATIONTAB_H
