#include "MainGuiData.h"
#include "Resource.h"
#include "Workspace.h"
#include "WorkspaceAlgorithmTab.h"
#include "WorkspaceModelTab.h"
#include "WorkspaceSimulationTab.h"

Workspace::Workspace(QWidget *parent)
    : TabWidget(parent)
{
    m_lessons = new WorkspaceEmptyTab;
    m_models = new WorkspaceModelTab;
    m_algorithms = new WorkspaceAlgorithmTab;
    m_simulations = new WorkspaceSimulationTab;

    addTab(m_lessons, "");
    addTab(m_models, "");
    addTab(m_algorithms, "");
    addTab(m_simulations, "");

    setTabIcon(0, Resource::windowIcon());
    setTabIcon(1, QIcon());
    setTabIcon(2, QIcon());
    setTabIcon(3, QIcon());

    //setTabShape(QTabWidget::Triangular);

    connect(this, &Workspace::currentChanged, this, &Workspace::onCurrentChanged);
    connect(&m_contextMenu, &WorkspaceContextMenu::tabPosChanged, this, &Workspace::updateTabPosition);
}

Workspace::~Workspace()
{
    delete m_lessons;
    delete m_models;
    delete m_algorithms;
    delete m_simulations;
}

WorkspaceEmptyTab* Workspace::currentTab()
{
    return static_cast<WorkspaceEmptyTab*>(currentWidget());
}

bool Workspace::hasARunningSimulation() const
{
    return static_cast<WorkspaceSimulationTab*>(m_simulations)->hasARunningSimulation();
}

void Workspace::retranslate()
{
    m_contextMenu.retranslate();

    setTabText(0, trUtf8("Leçons faciles"));
    m_lessons->retranslate();

    setTabText(1, trUtf8("Modèles"));
    m_models->retranslate();

    setTabText(2, trUtf8("Algorithmes"));
    m_algorithms->retranslate();

    setTabText(3, trUtf8("Simulations"));
    m_simulations->retranslate();
}

void Workspace::onCurrentChanged(int index)
{
    auto *curr = qobject_cast<WorkspaceBasicTab*>(widget(index));
    MainGuiData::setCurrentWork(curr ? curr->currentWork() : nullptr);
}

void Workspace::onTabBarContextMenuRequested(const QPoint &pos)
{
    // set up menu

    m_contextMenu.clearAndDontDelete();
    m_contextMenu.addActions(currentTab()->contextMenuActions());
    m_contextMenu.addSeparator();
    m_contextMenu.addDefaultActions();

    // execute menu

    m_contextMenu.execAt(pos, (QWidget*)tabBar());
}

void Workspace::updateTabPosition(int newPos)
{
    setTabPosition((QTabWidget::TabPosition)newPos);
}
