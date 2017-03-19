#ifndef GRAPHICSVIEWHIGHLIGHTING_H
#define GRAPHICSVIEWHIGHLIGHTING_H

#include <QFont>
#include <QObject>
#include <QPen>

class QTimer;

class HighlightingData : public QObject
{
    Q_OBJECT

private:
    QTimer *m_timer;
    bool m_singleShot; // timer singleshot
    int m_duration; // timer duration
    int m_remainingTimeWhenSuspended;

    int m_acceleration; // a numnber in range [-100, 100]

public:
    bool inForeground; // defaults to true (when not in foreground, it's in background).

public:
    HighlightingData(); // duration is set to 2000 ms

    bool isUseless() const;

    bool isTimerSingleShot() const;
    void setTimerSingleShot(bool singleShot);
    int timerDuration() const; // may differ from duration() due to acceleration for instance

    int duration() const;
    void setDuration(int duration);

    int acceleration() const;
    void setAcceleration(int acc);
    int durationFromAcceleration() const;

    void startTimer();
    void stopTimer();

    void suspendTimer();
    void resumeTimer();

    void configureData(bool inForeground);

protected:
    void registerTimerStop();

signals:
    void timerStopped();
};

class HighlightingTexTData : public HighlightingData
{
    Q_OBJECT

public:
    HighlightingTexTData();

    QString text;
    QRectF rect;

    int textFlags;      // defaults to Qt::AlignCenter
    QPen textPen;
    QFont textFont;
    qreal textOpaticy;  // defaults to 1 (opacity of the painter when drawing text)

    QPen rectPen;       // defaults to Qt::NoPen
    QBrush rectBrush;   // defaults to Qt::NoBrush
    qreal rectOpacity;  // defaults to 1 (opacity of the painter when drawing rect)

public:
    void configureTextData(const QString &text, const QRectF &rect, int textFlags);
    void configureTextData(const QPen &textPen, const QFont &textFont, qreal textOpaticy = -1);
    void configureTextData(const QPen &rectPen, const QBrush &rectBrush, qreal rectOpacity = -1);
};

#endif // GRAPHICSVIEWHIGHLIGHTING_H
