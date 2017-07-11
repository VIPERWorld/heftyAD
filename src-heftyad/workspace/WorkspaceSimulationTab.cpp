#include "SimulationWindow.h"
#include "WorkspaceSimulationTab.h"

#include <QAction>

WorkspaceSimulationTab::WorkspaceSimulationTab(QWidget *parent)
    : WorkspaceEmptyTab(parent)
{
    // create actions

    m_newSimulation = new QAction(QIcon(""), "", this);

    m_tiledHor = m_windows.addAction(QIcon(""), "");
    m_tiledVer = m_windows.addAction(QIcon(""), "");
    m_closeAll = m_windows.addAction(QIcon(""), "");
    m_closeAll->setShortcuts(QList<QKeySequence>() << QKeySequence("Ctrl+Shift+W") << QKeySequence("Ctrl+Shift+F4")); addAction(m_closeAll);

    m_background = new QAction(QIcon(""), "", this);

    // add and customize widgets

    m_splitter.addWidget(&m_container);
    m_container.setBackground(QBrush(Qt::lightGray, Qt::DiagCrossPattern));
    //m_container.setBackground(QBrush(QPixmap("linux.jpg")));
    updateWindowsRelatedActions();

    // connect signals to slots

    connect(m_newSimulation, &QAction::triggered, this, &WorkspaceSimulationTab::onNewSimulationActionTriggered);

    connect(m_tiledHor, &QAction::triggered, this,         &WorkspaceSimulationTab::tileSubWindowsHorizontally);
    connect(m_tiledVer, &QAction::triggered, this,         &WorkspaceSimulationTab::tileSubWindowsVertically);
    connect(m_closeAll, &QAction::triggered, &m_container, &QMdiArea::closeAllSubWindows);

    // momentarily

    m_background->setVisible(false);
}

WorkspaceSimulationTab::~WorkspaceSimulationTab()
{
}

QList<QAction*> WorkspaceSimulationTab::contextMenuActions() const
{
    return QList<QAction*>() << m_newSimulation << m_windows.menuAction() << m_background;
}

void WorkspaceSimulationTab::retranslate()
{
    m_newSimulation->setText(trUtf8("Nouvelle simulation"));

    m_windows.setTitle(trUtf8("Fenêtres"));
    m_tiledHor->setText(trUtf8("Aligner horizontalement"));
    m_tiledVer->setText(trUtf8("Aligner verticalement"));
    m_closeAll->setText(trUtf8("Fermer Tout"));

    m_background->setText(trUtf8("Arrière-plan")+"...");

    for(QMdiSubWindow *window : m_container.subWindowList()) {
        auto *win = qobject_cast<SimulationWindow*>(window);
        if(win) {
            win->retranslate();
        }
    }
}

bool WorkspaceSimulationTab::hasARunningSimulation() const
{
    for(QMdiSubWindow *window : m_container.subWindowList()) {
        auto *win = qobject_cast<SimulationWindow*>(window);
        if(win && !win->canBeClosed()) {
            return true;
        }
    }

    return false;
}

void WorkspaceSimulationTab::updateWindowsRelatedActions()
{
    const bool enable = !m_container.subWindowList().isEmpty();
    m_tiledHor->setEnabled(enable);
    m_tiledVer->setEnabled(enable);
    m_closeAll->setEnabled(enable);
}

void WorkspaceSimulationTab::onNewSimulationActionTriggered()
{
    auto *window = new SimulationWindow;
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->setSystemMenu(nullptr);
    m_container.addSubWindow(window);
    window->show();

    window->retranslate(); // To make sure inner widgets are translated
    window->setWindowTitle(trUtf8("Nouvelle Simulation"));

    connect(window, &SimulationWindow::destroyed, this, &WorkspaceSimulationTab::updateWindowsRelatedActions);
    updateWindowsRelatedActions();
}

void WorkspaceSimulationTab::tileSubWindowsHorizontally()
{
    QMdiArea *mdiArea = &m_container; // just so that this function can be easily exported to another project

    // code inspired by http://qt-digia.developpez.com/tutoriels/qt/mdi-organisation/#LIII

    QPoint position(0, 0);
    const int count = mdiArea->subWindowList().count();
    for(QMdiSubWindow *window : mdiArea->subWindowList()) {
        QRect rect(0, 0, mdiArea->width() / count, mdiArea->height());
        window->setGeometry(rect);
        window->move(position);
        position.setX(position.x() + window->width());
    }
}

void WorkspaceSimulationTab::tileSubWindowsVertically()
{
    QMdiArea *mdiArea = &m_container; // just so that this function can be easily exported to another project

    // code inspired by http://qt-digia.developpez.com/tutoriels/qt/mdi-organisation/#LIII

    QPoint position(0, 0);
    const int count = mdiArea->subWindowList().count();
    for(QMdiSubWindow *window : mdiArea->subWindowList()) {
        QRect rect(0, 0, mdiArea->width(), mdiArea->height() / count);
        window->setGeometry(rect);
        window->move(position);
        position.setY(position.y() + window->height());
    }
}
