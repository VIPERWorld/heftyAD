#ifndef SIMPLYCONTROLLABLE_H
#define SIMPLYCONTROLLABLE_H

#include "Controllable.hpp"

#include <QMutex>
#include <QObject>
#include <QWaitCondition>

/**
 * The SimplyControllable class provides a simple implementation of the Controllable concept.
 * It uses a QMutex to lock (i.e. to suspend) itself and a QWaitCondition to resume afterwards.
 */
class SimplyControllable : public QObject, public Controllable
{
    Q_OBJECT

public:
    /**
     * The StopException class is a kind of signal which is thrown by this controllable
     * when it needs to stop -> see .cpp.
     */
    class StopException : public std::exception {};

private:
    bool m_suspended;

    bool m_suspendWall_enabled;
    QMutex m_suspendWall_mutex;
    QWaitCondition m_suspendWall_waitCondition;

    bool m_stopWall_enabled;

public:
    explicit SimplyControllable(QObject *parent = 0);

    bool isSuspended() const override;

    void suspend() override;
    void resume() override;
    void stop() override;

protected:
    void suspendOrStopIfRequired();
    void resetAttrs();

private:
    void suspendIfSuspendWallEnabled();
    void stopIfStopWallEnabled();

    void setSuspended(bool suspended);

signals:
    void suspended();
    void resumed();
};

#endif // SIMPLYCONTROLLABLE_H
