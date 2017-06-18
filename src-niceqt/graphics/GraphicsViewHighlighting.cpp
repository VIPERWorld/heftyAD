#include "GraphicsViewHighlighting.h"

#include <QTimer>

HighlightingData::HighlightingData()
    : QObject()
{
    m_singleShot = true;
    m_duration = 2000;
    m_remainingTimeWhenSuspended = -1;
    inForeground = true;
    m_timer = nullptr;
    m_acceleration = 0;
}

bool HighlightingData::isTimerSingleShot() const {return m_timer && m_timer->isSingleShot();}
void HighlightingData::setTimerSingleShot(bool singleShot)
{
    m_singleShot = singleShot; // register this value, since the timer may have not been created yet
    if(m_timer) {
        m_timer->setSingleShot(m_singleShot);
    }
}
int HighlightingData::timerDuration() const {return m_timer->interval();}

int HighlightingData::duration() const {return m_duration;}
void HighlightingData::setDuration(int duration) {m_duration = duration<0 ? -duration : duration;}

int HighlightingData::acceleration() const {return m_acceleration;}
void HighlightingData::setAcceleration(int acc)
{
    if(acc<-100 || acc>100 || m_acceleration==acc) {
        return;
    }

    m_acceleration = acc;

    const int d = acceleratedDuration();
    if(m_timer) {
        if(m_timer->isActive()) { // timer is running (neither stopped nor paused)
            m_timer->setInterval(d);
            return;
        }
        if(m_remainingTimeWhenSuspended >= 0) { // timer is paused
            m_remainingTimeWhenSuspended = d;
            return;
        }
        // if the timer is stopped, there is nothing to do since since the right duration will be set on start
    }
}
int HighlightingData::acceleratedDuration() const {return m_duration - m_duration*m_acceleration/100;}

void HighlightingData::startTimer()
{
    if(m_timer == nullptr) {
        m_timer = new QTimer;
        m_timer->setSingleShot(m_singleShot);

        connect(m_timer, &QTimer::timeout, [this]() {
            if(m_timer->isSingleShot()) {
                registerTimerStop();
            }
        });

        connect(this, &HighlightingData::destroyed, [this]() {
            delete m_timer;
        });
    }

    m_remainingTimeWhenSuspended = -1;

    m_timer->setInterval(acceleratedDuration());
    m_timer->start();
}

void HighlightingData::stopTimer()
{
    if(m_timer) {
        m_timer->stop(); // Note: QTimer::timeout won't be emitted
        registerTimerStop();
    }
}

void HighlightingData::suspendTimer()
{
    if(m_timer) {
        m_remainingTimeWhenSuspended = m_timer->remainingTime();
        m_timer->stop();
    }
}

void HighlightingData::resumeTimer()
{
    if(m_timer && m_remainingTimeWhenSuspended >= 0) {
        m_timer->start(m_remainingTimeWhenSuspended);
        m_remainingTimeWhenSuspended = -1;
    }
}

void HighlightingData::configureData(bool inForeground)
{
    this->inForeground = inForeground;
}

void HighlightingData::registerTimerStop()
{
    m_remainingTimeWhenSuspended = -1;
    emit timerStopped();
}

HighlightingTexTData::HighlightingTexTData()
{
    textFlags = Qt::AlignCenter;
    textOpaticy = 1.;
    rectOpacity = 1.;
    rectPen = Qt::NoPen;
    rectBrush = Qt::NoBrush;
}

void HighlightingTexTData::configureTextData(const QString &text, const QRectF &rect, int textFlags)
{
    this->text = text;
    this->rect = rect;
    this->textFlags = textFlags;
}

void HighlightingTexTData::configureTextData(const QPen &textPen, const QFont &textFont, qreal textOpaticy)
{
    this->textPen = textPen;
    this->textFont = textFont;
    if(textOpaticy >= 0) {
        this->textOpaticy = textOpaticy;
    }
}

void HighlightingTexTData::configureTextData(const QPen &rectPen, const QBrush &rectBrush, qreal rectOpacity)
{
    this->rectPen = rectPen;
    this->rectBrush = rectBrush;
    if(rectOpacity >= 0) {
        this->rectOpacity = rectOpacity;
    }
}
