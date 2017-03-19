#include "SimulationWindow.h"
#include "WorkspaceSimulationTab.h"

#include <QAction>

WorkspaceSimulationTab::WorkspaceSimulationTab(QWidget *parent)
    : WorkspaceEmptyTab(parent)
{
    m_allowedActions = CloseAll;

    m_newSimulation = new QAction(QIcon(""), "", nullptr);
    m_background = new QAction(QIcon(""), "", nullptr);

    m_splitter.addWidget(&m_container);
    m_container.setBackground(QBrush(Qt::lightGray, Qt::DiagCrossPattern));
    //m_container.setBackground(*new QBrush(*new QPixmap("linux.jpg")));

    connect(m_newSimulation, &QAction::triggered, this, &WorkspaceSimulationTab::onNewSimulationActionTriggered);
}

WorkspaceSimulationTab::~WorkspaceSimulationTab()
{
    delete m_newSimulation;
    delete m_background;
}

QList<QAction*> WorkspaceSimulationTab::contextMenuActions() const
{
    return QList<QAction*>() << m_newSimulation << m_windows.menuAction() << m_background;
}

void WorkspaceSimulationTab::retranslate()
{
    m_newSimulation->setText(trUtf8("Nouvelle simulation"));
    m_windows.setTitle(trUtf8("Fenêtres"));
    m_background->setText(trUtf8("Arrière-plan")+"...");

    for(QMdiSubWindow *window : m_container.subWindowList()) {
        auto *win = dynamic_cast<SimulationWindow*>(window);
        if(win) {
            win->retranslate();
        }
    }
}

bool WorkspaceSimulationTab::hasOpenedWork() const {return !m_container.subWindowList().isEmpty();}

void WorkspaceSimulationTab::onNewSimulationActionTriggered()
{
    auto *window = new SimulationWindow;
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->setSystemMenu(nullptr);
    m_container.addSubWindow(window);
    window->show();

    window->retranslate(); // To make sure inner widgets are translated
    const int &count = m_container.subWindowList().count();
    window->setWindowTitle(trUtf8("Simulation %1").arg(QString::number(count)));
}
