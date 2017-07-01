#include "Resource.h"
#include "SimulationConsoleTab.h"

#include "utility/Utility.h"

#include <QMenu>

SimulationConsoleTab::SimulationConsoleTab(QWidget *parent)
    : TabWidget(parent)
{
    addTab(&m_simpleConsole, "");
    addTab(&m_shadowConsole, "");

//    setCurrentIndex(1);
    setTabPosition(QTabWidget::South);
    setTabIcon(0, Resource::windowIcon());
    setTabIcon(1, Resource::shadowIcon("shadow.png"));
}

void SimulationConsoleTab::setClarifier(SimulationClarifier *clarifier)
{
    m_simpleConsole.setClarifier(clarifier);
    m_shadowConsole.setClarifier(clarifier);
}

void SimulationConsoleTab::retranslate()
{
    setTabText(0, trUtf8("Console Basique"));
    setTabText(1, trUtf8("Console Shadow"));
}

void SimulationConsoleTab::onTabBarContextMenuRequested(const QPoint &pos)
{
    return; // momentarily

    // Build menu

    QMenu menu;

    const int index = currentIndex();
    switch(index) {
    case 0: // simple console
        menu.addAction("Dark Color Scheme");
        break;

    case 1: // shadow console
        break;
    }

    // Execute menu

    Utility::execMenuAt(&menu, pos, (QWidget*)tabBar());
}
