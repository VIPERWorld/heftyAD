#ifndef SIMULATIONWIDGET_H
#define SIMULATIONWIDGET_H

#include "SimulationSideWidget.h"

#include <QPointer>
#include <QSplitter>

class View;

class SimulationWidget : public GridWidget
{
    Q_OBJECT

protected:
    QSplitter m_splitter;
        SimulationSideWidget m_sideWidget;
        QPointer<View> m_view;

    QWidgetList m_simulationWidgets;

public:
    explicit SimulationWidget(QWidget *parent = 0);

    SimulationSideWidget* sideWidget() const;

    void setView(View *view);

    void addSimulationWidget(QWidget *widget, int index = -1);

    void retranslate();

signals:
    void simulationFullScreenEnabled(bool enabled);

public slots:
};

#endif // SIMULATIONWIDGET_H
