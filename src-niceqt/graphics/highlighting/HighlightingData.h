#ifndef HIGHLIGHTINGDATA_H
#define HIGHLIGHTINGDATA_H

#include <QObject>

class QTimer;

class HighlightingData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool inForeground MEMBER inForeground)

private:
    /*
     * We use a pointer so that timer is created in the thread from which startTimer() is called,
     * not in the one which creates the highlighting data.
     */
    QTimer *m_timer;
    bool m_singleShot; // timer singleshot
    int m_duration; // timer duration
    int m_remainingTimeWhenSuspended;

    int m_acceleration; // a numnber in range [-100, 100]

public:
    bool inForeground; // defaults to true (when not in foreground, it's in background).

public:
    Q_INVOKABLE HighlightingData(); // duration is set to 2000 ms

    Q_INVOKABLE bool isTimerSingleShot() const;
    Q_INVOKABLE void setTimerSingleShot(bool singleShot);
    Q_INVOKABLE int timerDuration() const; // may differ from duration() due to acceleration for instance

    Q_INVOKABLE int duration() const;
    Q_INVOKABLE void setDuration(int duration);

    Q_INVOKABLE int acceleration() const;
    Q_INVOKABLE void setAcceleration(int acc);
    Q_INVOKABLE int acceleratedDuration() const;

    Q_INVOKABLE void startTimer();
    Q_INVOKABLE void stopTimer();

    Q_INVOKABLE void suspendTimer();
    Q_INVOKABLE void resumeTimer();

    Q_INVOKABLE void configureData(bool inForeground);

protected:
    void registerTimerStop();

signals:
    void timerStopped();
};

#endif // HIGHLIGHTINGDATA_H
