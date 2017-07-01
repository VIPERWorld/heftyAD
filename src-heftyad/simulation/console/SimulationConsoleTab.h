#ifndef SIMULATIONCONSOLETAB_H
#define SIMULATIONCONSOLETAB_H

#include "SimulationShadowConsole.h"
#include "SimulationSimpleConsole.h"

#include "widget/TabWidget.h"

class SimulationClarifier;

class SimulationConsoleTab : public TabWidget
{
    Q_OBJECT

protected:
    SimulationSimpleConsole m_simpleConsole;
    SimulationShadowConsole m_shadowConsole;

public:
    explicit SimulationConsoleTab(QWidget *parent = 0);

    void setClarifier(SimulationClarifier *clarifier);
    void retranslate();

signals:

public slots:
    void onTabBarContextMenuRequested(const QPoint &pos) override;
};

#endif // SIMULATIONCONSOLETAB_H
