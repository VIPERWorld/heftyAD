#include "MainWindow.h"
#include "Resource.h"

#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : BasicMainWindow(parent)
{
    setWindowTitle("heftyAD - Your hefty algorithm demystifier");
    setWindowIcon(Resource::instance().windowIcon());
    resize(800, 600);

    Resource::instance().loadStyleSheets();
    Resource::instance().makeUserDirs();

    addWidget(&m_welcomeWidget); // becomes the current widget since the inner stack widget is empty
    addWidget(&m_homeWidget);
    addWidget(&m_workspace);

    connectSignalsToSlots();
    retranslate();
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectSignalsToSlots()
{
    connect(&m_welcomeWidget, &WelcomeWidget::needToBeHidden, this, &MainWindow::loadHomePage);

    connect(&m_fileMenu, &FileMenu::workSpaceActionTriggered, this, &MainWindow::setWorkSpaceVisible);
}

void MainWindow::addMenus()
{
    menuBar()->addMenu(&m_fileMenu);
    menuBar()->addMenu(&m_editMenu);
    menuBar()->addMenu(&m_settingMenu);
}

void MainWindow::loadHomePage()
{
    setCurrentWidget(&m_homeWidget);
    addMenus();
}

void MainWindow::setWorkSpaceVisible(bool visible)
{
    visible ? setCurrentWidget(&m_workspace) : setCurrentWidget(&m_homeWidget);
}

void MainWindow::retranslate()
{
    m_fileMenu.retranslate();
    m_editMenu.retranslate();
    m_settingMenu.retranslate();

    m_workspace.retranslate();
}
