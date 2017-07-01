#include "GraphicsViewHighlighter.h"
#include "HighlightingData.h"

#include <QMutex>
#include <QWaitCondition>

GraphicsViewHighlighter::GraphicsViewHighlighter(QObject *parent)
    : QObject(parent)
{
}

void GraphicsViewHighlighter::start(HighlightingData *data, int duration, bool synchrone)
{
    auto startHighlighting = [=]() {
        emit startRequested(data, duration);
    };

    if(!synchrone) {
        startHighlighting();
    }
    else {
        QMutex mutex;
        QWaitCondition condition;
        auto connexion = connect(data, &HighlightingData::timerStopped, [&condition](){condition.wakeAll();});

        mutex.lock();
        startHighlighting();
        condition.wait(&mutex);
        mutex.unlock();

        QObject::disconnect(connexion);
    }
}

void GraphicsViewHighlighter::suspend(HighlightingData *data)
{
    emit suspendRequested(data);
}

void GraphicsViewHighlighter::stop(HighlightingData *data)
{
    emit stopRequested(data);
}

void GraphicsViewHighlighter::start(const QJSValue &dataProxy, int duration, bool synchrone)
{
    QObject *obj = dataProxy.toQObject();
    auto *data = qobject_cast<HighlightingData*>(obj);
    if(data) {
        start(data, duration, synchrone);
    }
}

void GraphicsViewHighlighter::suspend(const QJSValue &dataProxy)
{
    QObject *obj = dataProxy.toQObject();
    auto *data = qobject_cast<HighlightingData*>(obj);
    if(data) {
        suspend(data);
    }
}

void GraphicsViewHighlighter::stop(const QJSValue &dataProxy)
{
    QObject *obj = dataProxy.toQObject();
    auto *data = qobject_cast<HighlightingData*>(obj);
    if(data) {
        stop(data);
    }
}
