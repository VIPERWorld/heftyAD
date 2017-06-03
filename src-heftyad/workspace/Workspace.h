#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "WorkspaceContextMenu.h"

#include "widget/TabWidget.h"

class WorkspaceEmptyTab;

class Workspace : public TabWidget
{
    Q_OBJECT

protected:
    WorkspaceContextMenu m_contextMenu;

    WorkspaceEmptyTab *m_lessons;
    WorkspaceEmptyTab *m_models;
    WorkspaceEmptyTab *m_algorithms;
    WorkspaceEmptyTab *m_simulations;

public:
    explicit Workspace(QWidget *parent = 0);
    ~Workspace();

    WorkspaceEmptyTab* currentTab();

    bool hasARunningSimulation() const;

signals:

public slots:
    void retranslate();

private slots:
    void onCurrentChanged(int index);
    void onTabBarContextMenuRequested(const QPoint &pos) override;
    void updateTabPosition(int newPos);
};

#endif // WORKSPACE_H
