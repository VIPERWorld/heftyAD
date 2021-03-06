#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "widget/GridWidget.h"
#include "widget/PushButton.h"

#include <QLabel>
#include <QLCDNumber>
#include <QMenu>
#include <QPointer>
#include <QProgressBar>
#include <QStateMachine>
#include <QSlider>

class Algorithm;
class AlgorithmRunner;
class SimulationClarifier;
class SimulationLocker;
class View;

class SimulationController : public GridWidget
{
    Q_OBJECT

protected:
    Algorithm *m_algorithm;
    QPointer<AlgorithmRunner> m_algorithmRunner;
    SimulationLocker *m_locker;
    SimulationClarifier *m_clarifier;
    QPointer<View> m_view; // simulation view

    QLabel m_space;
    PushButton m_prev;
    PushButton m_resume, m_suspend;
    PushButton m_next;
    PushButton m_stop;
    PushButton m_restart;
    PushButton m_more;
        QMenu m_more_menu;
            QAction *m_hideProgressBar;
            QAction *m_blockClarifier;
    PushButton m_fullScreen;
    QProgressBar m_progressBar;
    QSlider m_acceleration; QLCDNumber m_accelerationLCD;

    QStateMachine m_stateMachine; // state machines
        QState m_state_init;
        QState m_state_running;
            QState m_state_resumed;
            QState m_state_suspended;

private:
    bool m_simulation_should_restart;
    int m_simulation_acceleration_onStop;

public:
    explicit SimulationController(QWidget *parent = 0);

    void setAlgorithm(Algorithm *algorithm);
    void setLocker(SimulationLocker *locker);
    void setClarifier(SimulationClarifier *clarifier);
    void setSimulationView(View *view);

    void retranslate();

protected:
    void setAlgorithmRunner(AlgorithmRunner *algorithmRunner);

    void connectSignalsToSlots();
    void setIcons();

private:
    void loadStateMachine();

signals:
    void simulationStarted();
    void simulationFinished();
    void simulationFailedWithException();
    void simulationFullScreenEnabled(bool enabled);

protected slots:
    void onStateMachineInitialStateEntered();

    void onResumeButtonPressed();
    void onSuspendButtonPressed();
    void onStopButtonPressed();
    void onRestartButtonPressed();
    void onFullScreenButtonToggled(bool checked);

    void onNextButtonPressed();

    void onMoreMenuTriggered(QAction *action);

    void onAccelerationValueChanged(int value);
    void useAccelerationValue();

    void onAlgorithmRunnerFailsWith(int errorFlag, const QString &errorMessage = "");
    void handleAlgorithmException(const QString &message);

    void onAlgorithmRunnerResumed();
    void onAlgorithmRunnerSuspended();
    void onAlgorithmRunnerStopped();
    void onAlgorithmRunnerFinished();
    void onAlgorithmRunnerDestroyed();
};

#endif // SIMULATIONCONTROLLER_H
