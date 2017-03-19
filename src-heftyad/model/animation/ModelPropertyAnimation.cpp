#include "ModelPropertyAnimation.h"

#include <QApplication>
#include <QMutex>
#include <QPropertyAnimation>
#include <QThread>
#include <QWaitCondition>

ModelPropertyAnimation::ModelPropertyAnimation(QObject *parent)
    : QObject(parent)
{
}

void ModelPropertyAnimation::configure(QPropertyAnimation *animation,
                                       QObject *target,
                                       const QByteArray &propertyName,
                                       const QVariant &from, const QVariant &to,
                                       int duration,
                                       int loopCount)
{
    animation->setTargetObject(target);
    animation->setPropertyName(propertyName);
    animation->setDuration(duration);
    animation->setStartValue(from);
    animation->setEndValue(to);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setLoopCount(loopCount);
}

void ModelPropertyAnimation::reverse(QPropertyAnimation *source, QPropertyAnimation *target)
{
    target->setTargetObject(source->targetObject());
    target->setPropertyName(source->propertyName());
    target->setDuration(source->duration());

    // reverse here
    target->setStartValue(source->endValue());
    target->setEndValue(source->startValue());

    target->setEasingCurve(source->easingCurve()); // to be changed later -> to reverse the easing curve
    target->setLoopCount(source->loopCount());
}

void ModelPropertyAnimation::trigger(QAbstractAnimation *animation, bool synchrone, DeletionContext deletionContext)
{
    /* The connexion below won't delete indefinitely looping animation.
     * To avoid memory leaks, the deletionContext paramater is introduced.
     */
    connect(animation, &QAbstractAnimation::finished, [=](){animation->deleteLater();});

    /* Considering what's stated in Qt's doc about Qt::AutoConnection behaviour
     * (see QObject::connect), we need to make sure
     *     ESPECIALLY WHEN this function is called from a thread other than the gui-thread:
     *         from an algorithm thread for instance,
     * that the animation passed as parameter leaves in the gui-thread,
     * so that it can perform flawlessly.
     *
     * Indeed, IN THAT CASE, performing a direct call to QAbstractAnimation::start causes the animation
     * not to perform in the way it should. So we first move it to the gui-thread and then
     * emit a signal which finally triggers its execution.
     *
     * This isn't probably the best way to address this issue, but it works.
     */
    // connect(this, &ModelPropertyAnimation::_startIt, animation, &QAbstractAnimation::start);
    // the previous syntax doesn't work due to argument count mismatching.
    connect(this, SIGNAL(_startIt()), animation, SLOT(start()));
    animation->moveToThread(QApplication::instance()->thread());
    auto startAnimation = [=]() { // define a function that will start the animation
        emit _startIt();
        // disconnect the signal, to make sure any further emission is ignored.
        disconnect(this, SIGNAL(_startIt()), animation, SLOT(start()));
    };

    QThread *t = nullptr;
    switch(deletionContext) {
    case None:                                               break;
    case ThisThread: t = thread();                           break;
    case GuiThread:  t = QApplication::instance()->thread(); break;
    }
    if(t) {
        connect(t, &QThread::finished, animation, &QAbstractAnimation::deleteLater);
    }

    if(!synchrone) {
        startAnimation();
    }
    else {
        QMutex mutex;
        QWaitCondition condition;
        auto connexion = connect(animation, &QAbstractAnimation::finished, [&condition](){condition.wakeAll();});

        mutex.lock();
        startAnimation();
        condition.wait(&mutex);
        mutex.unlock();

        disconnect(connexion);
    }
}
