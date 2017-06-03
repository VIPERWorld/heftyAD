#ifndef WORKSPACECONTEXTMENU_H
#define WORKSPACECONTEXTMENU_H

#include "widget/Menu.h"

/**
 * Represents the menu which shows up
 * when the tab bar of the workspace widget is right clicked.
 */

class WorkspaceContextMenu : public Menu
{
    Q_OBJECT

protected:
    QMenu m_tabPos;
        QActionGroup *m_tabPosGroup;
            QAction *m_tabPosNorth;
            QAction *m_tabPosEast;
            QAction *m_tabPosSouth;
            QAction *m_tabPosWest;

public:
    explicit WorkspaceContextMenu(QWidget *parent = 0);
    ~WorkspaceContextMenu();

    void setShortcuts();
    void connectSignalsToSlots();
    void retranslate();

    void addDefaultActions();

private:
    /**
     * Creates default actions (those that are commons to all tabs in the workspace widget).
     * It should be called once and before using those actions.
     */
    void createDefaultActions();

signals:
    void tabPosChanged(int); // the parameter should match with one of those described by QTabWidget::TabPosition

protected slots:
    void onTabPosTriggered(QAction *action);
};

#endif // WORKSPACECONTEXTMENU_H
