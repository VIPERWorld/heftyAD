#ifndef SIMULATIONLOCKER_H
#define SIMULATIONLOCKER_H

#include <QObject>
#include <QTimer>

class SimulationLocker : public QObject
{
    Q_OBJECT

protected:
    QTimer m_lockTimer;
    int m_lockDuration;

    int m_simulationAcceleration;
    bool m_simulationSuspended;

public:
    SimulationLocker(QObject *parent = 0);

    void setSimulationAcceleration(int acc);
    void setSimulationSuspended(bool suspended);
    void lockFor(long msecs);

    /**
     * We can do without this function.
     * But it can be used within algorithm instances,
     * to adjust the duration of ModelPropertyAnimation instances.
     */
    int adjusted(int duration) const;

protected:
    void adjustLockDuration();

signals:
    void _startIt(); // see code in cpp for usage
};

#endif // SIMULATIONLOCKER_H
