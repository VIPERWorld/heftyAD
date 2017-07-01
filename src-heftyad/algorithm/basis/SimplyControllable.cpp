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
         * No "stopped" signal is provided/emitted because emitting a signal
         * would only mean that "stop" has been requested BUT NOT THAT this controllable has stopped.
         * So throwing an exception (that should be caught) really comes in help here.
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
