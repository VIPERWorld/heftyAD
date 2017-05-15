#ifndef SIGNALBREAKER_H
#define SIGNALBREAKER_H

#include <QObject>

#include <functional>

/**
 * The SignalBreaker class is an alternative for the QSignalBlocker class introduced since Qt5.3.
 * But unlike QSignalBlocker SignalBreaker doesn't block all signals. It merely
 *     cuts off a given signal-slot connection in its constructor and
 *     sets it back in its destructor (only if the connection was successfully broken).
 *
 * More literatures (about blocking qt signals) here
 *     http://stackoverflow.com/questions/3556687/prevent-firing-signals-in-qt
 *     http://stackoverflow.com/questions/15633086/temporarily-block-signals-between-two-qobjects
 */
class SignalBreaker
{
private:
    std::function<void()> m_destructorBody;

private:
    void init() {
        m_destructorBody = []() {};
    }

public:
    SignalBreaker(const QObject *sender, const char *signal,
                  const QObject *receiver, const char *member,
                  Qt::ConnectionType type = Qt::AutoConnection)
    {
        init();
        if(QObject::disconnect(sender, signal, receiver, member)) {
            m_destructorBody = [=]() {
                QObject::connect(sender, signal, receiver, member, type);
            };
        }
    }

    template <typename Func1, typename Func2>
    SignalBreaker(const typename QtPrivate::FunctionPointer<Func1>::Object *sender, Func1 signal,
                  const typename QtPrivate::FunctionPointer<Func2>::Object *receiver, Func2 slot,
                  Qt::ConnectionType type = Qt::AutoConnection)
    {
        init();
        if(QObject::disconnect(sender, signal, receiver, slot)) {
            m_destructorBody = [=]() {
                QObject::connect(sender, signal, receiver, slot, type);
            };
        }
    }

    ~SignalBreaker()
    {
        m_destructorBody();
    }
};

#endif // SIGNALBREAKER_H
