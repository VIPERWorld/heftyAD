#ifndef SIMULATIONSIDEWIDGET_H
#define SIMULATIONSIDEWIDGET_H

#include "SimulationConfigWidget.h"
#include "SimulationClarifier.h"
#include "SimulationController.h"
#include "SimulationHighlighter.h"
#include "SimulationLocker.h"
#include "SimulationShadowConsole.h"
#include "SimulationSimpleConsole.h"

#include "widget/GridWidget.h"
#include "widget/TabWidget.h"

class SimulationSideWidget : public GridWidget
{
    Q_OBJECT

protected:
    /*
     * We could have made the clarifier an attribute of Algorithm.
     * But we make it this class's attribute since it allows us to use
     * the clarifier even if no algorithm has yet been set:
     *     to display a welcoming message for example.
     */
    SimulationLocker m_locker;
    SimulationClarifier m_clarifier;
    SimulationHighlighter m_highlighter; // Make it this class's attribute just to do as above.

    SimulationController m_controller;
    TabWidget m_others;
        SimulationConfigWidget m_configWidget;
        TabWidget m_consoles;
            SimulationSimpleConsole m_simpleConsole;
            SimulationShadowConsole m_shadowConsole;

public:
    explicit SimulationSideWidget(QWidget *parent = 0);

    SimulationConfigWidget* configWidget() const;

    QSize sizeHint() const override;

    void connectSignalsToSlots();
    void retranslate();

signals:
    void simulationFullScreenEnabled(bool enabled);
    void configWidgetModelChanged();

public slots:
};

#endif // SIMULATIONSIDEWIDGET_H
