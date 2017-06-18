#ifndef MODELPROPERTYANIMATION_H
#define MODELPROPERTYANIMATION_H

#include <QObject>

class QAbstractAnimation;
class QPropertyAnimation;

class ModelPropertyAnimation : public QObject
{
    Q_OBJECT

public:
    typedef enum {
        None,
        ThisThread,
        GuiThread
    } DeletionContext;

public:
    explicit ModelPropertyAnimation(QObject *parent = 0);

    static void configure(QPropertyAnimation *animation,
                          QObject *target,
                          const QByteArray &propertyName,
                          const QVariant &from, const QVariant &to,
                          int duration,
                          int loopCount = 1);
    static void reverse(QPropertyAnimation *source, QPropertyAnimation *target);

signals:
    void _startIt(); // see .cpp to know about how it's used.

public slots:
    /**
     * Triggers the execution of the given animation, either in a synchronous or asynchronous way.
     * If synchrone is true, this function blocks until the animation finishes.
     * In any case the animation is deleted WHEN IT FINISHES.
     *
     * Since animation may loop indefinitely (that is it may never emit QAbstractAnimation::finished),
     * you can get yourself helped by passing the deletion context parameter.
     * For example, if context is ThisThread, the animation will be deleted on this thread termination.
     */
    void trigger(QAbstractAnimation *animation, bool synchrone = true, DeletionContext deletionContext = None);
};

#endif // MODELPROPERTYANIMATION_H
