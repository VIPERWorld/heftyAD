#include "SimulationLocker.h"

#include <QMutex>
#include <QWaitCondition>

SimulationLocker::SimulationLocker(QObject *parent)
    : QObject(parent)
{
    m_lockDuration = 0;
    m_simulationAcceleration = 0;
    m_simulationSuspended = false;
}

void SimulationLocker::setSimulationAcceleration(int acc)
{
    if(acc<-100 || acc>100 || acc==m_simulationAcceleration) {
        return;
    }

    m_simulationAcceleration = acc;
    adjustLockDuration();
}

void SimulationLocker::setSimulationSuspended(bool suspended)
{
    if(m_simulationSuspended == suspended) {
        return;
    }

    m_simulationSuspended = suspended;
    // timer suspension is not actually handled
}

void SimulationLocker::lockFor(long msecs)
{
    m_lockDuration = msecs<0 ? 0 : msecs;
    adjustLockDuration();

    auto startPause = [this]() {
        auto conn = connect(this, SIGNAL(_startIt()), &m_lockTimer, SLOT(start()));
        emit _startIt(); // make sure timer starts in gui thread
        QObject::disconnect(conn);
    };

    QMutex mutex;
    QWaitCondition condition;
    auto connexion = connect(&m_lockTimer, &QTimer::timeout, [&condition](){condition.wakeAll();});

    mutex.lock();
    startPause(); // to make sure timer starts in this' thread (gui thread)
    condition.wait(&mutex);
    mutex.unlock();

    QObject::disconnect(connexion);
}

int SimulationLocker::adjusted(int duration) const
{
    return duration - duration*m_simulationAcceleration/100;
}

void SimulationLocker::adjustLockDuration()
{
    m_lockTimer.setInterval(adjusted(m_lockDuration));
}
