#ifndef SIMULATIONWINDOW_H
#define SIMULATIONWINDOW_H

#include "SimulationWidget.h"

#include <QMdiSubWindow>

class SimulationWindow : public QMdiSubWindow
{
    Q_OBJECT

protected:
    SimulationWidget m_widget;

    QMdiArea *m_mdiAreaBeforeGoingFullScreen;
    QByteArray m_geometryBeforeGoingFullScreen;

public:
    explicit SimulationWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);

    /**
     * Returns false if a simulation is running
     *         true otherwise
     */
    bool canBeClosed() const;

    void onSimulationFullScreenEnabled(bool enabled);
    void retranslate();

protected:
    void closeEvent(QCloseEvent *closeEvent) override;

signals:

protected slots:
    void onWindowStateChanged(Qt::WindowStates oldState, Qt::WindowStates newState);
};

#endif // SIMULATIONWINDOW_H
