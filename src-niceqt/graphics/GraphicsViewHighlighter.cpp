#include "GraphicsViewHighlighter.h"
#include "GraphicsViewHighlighting.h"

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
