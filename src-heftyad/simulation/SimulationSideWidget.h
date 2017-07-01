#ifndef SIMULATIONSIDEWIDGET_H
#define SIMULATIONSIDEWIDGET_H

#include "SimulationConfigWidget.h"
#include "SimulationConsoleTab.h"
#include "SimulationController.h"

#include "widget/GridWidget.h"
#include "widget/TabWidget.h"

class SimulationSideWidget : public GridWidget
{
    Q_OBJECT

protected:
    SimulationController m_controller;
    TabWidget m_others;
        SimulationConfigWidget m_configWidget;
        SimulationConsoleTab m_consoles;

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
