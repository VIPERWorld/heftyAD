#include "SimplyControllable.h"

SimplyControllable::SimplyControllable(QObject *parent)
    : QObject(parent),
      Controllable()
{
    resetAttrs();
}

bool SimplyControllable::isSuspended() const {return m_suspended;}

void SimplyControllable::suspend()
{
    m_suspendWall_enabled = true;
}

void SimplyControllable::resume()
{
    m_suspendWall_enabled = false;
    m_suspendWall_waitCondition.wakeAll();
}

void SimplyControllable::stop()
{
    m_stopWall_enabled = true;
}

void SimplyControllable::suspendOrStopIfRequired()
{
    suspendIfSuspendWallEnabled();
    stopIfStopWallEnabled();
}

void SimplyControllable::resetAttrs()
{
    m_suspended = false;

    m_suspendWall_enabled = false;
    m_stopWall_enabled = false;
}

void SimplyControllable::suspendIfSuspendWallEnabled()
{
    m_suspendWall_mutex.lock();
    while(m_suspendWall_enabled) {
        setSuspended(true);
        m_suspendWall_waitCondition.wait(&m_suspendWall_mutex); // Will block until the mutex is unlocked (i.e. the thread is resumed).
        setSuspended(false);
    }
    m_suspendWall_mutex.unlock();
}

void SimplyControllable::stopIfStopWallEnabled()
{
    if(m_stopWall_enabled) {
        m_stopWall_enabled = false;

        /*
         * No "stopped" signal is provided/emitted because if we had emitted a signal,
         * this would have only informed us that this controllable is about to stop BUT NOT THAT it has stopped.
         * So catching a thrown exception really comes in help here.
         */
        throw StopException();
    }
}

void SimplyControllable::setSuspended(bool suspended)
{
    if(m_suspended != suspended) {
        m_suspended = suspended;
        if(m_suspended) {
            emit this->suspended();
        }
        else {
            emit resumed();
        }
    }
}
