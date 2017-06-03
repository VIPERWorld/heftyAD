#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "EditMenu.h"
#include "FileMenu.h"
#include "HomeWidget.h"
#include "SettingMenu.h"
#include "WelcomeWidget.h"
#include "Workspace.h"

#include "winframe/BasicMainWindow.h"

class MainWindow : public BasicMainWindow
{
    Q_OBJECT

protected:
    FileMenu m_fileMenu;
    EditMenu m_editMenu;
    SettingMenu m_settingMenu;

    WelcomeWidget m_welcomeWidget;
    HomeWidget m_homeWidget;
    Workspace m_workspace;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *closeEvent) override;

private:
    void connectSignalsToSlots();
    void addMenus();

public slots:
    /**
     * Not only loads the home page/widget but also adds menus to the menu bar.
     * So this function should be called once (to prevent the menu items from being added many times).
     */
    void loadHomePage();
    /**
     * Set the workspace widget visibility to visible.
     * If visible is true then the workspace widget is loaded ; else the homewidget is loaded.
     */
    void setWorkSpaceVisible(bool visible);

    void retranslate() override;
};

#endif // MAINWINDOW_H
