#include "GraphicsViewHighlighter.h"
#include "GraphicsViewHighlighting.h"
#include "HighlightableGraphicsView.h"

HighlightableGraphicsView::HighlightableGraphicsView(QWidget *parent)
    : GraphicsView(parent),
      m_highlighter(nullptr),
      m_highlightingDataAcceleration(0)
{
//    addHighlightingText(7500, false, true,
//                        "I can do it!", QRectF(-200, -200, 100, 50), Qt::AlignCenter,
//                        QPen(Qt::blue), QFont(),
//                        QPen(), QBrush(Qt::lightGray));
//    addHighlightingText(5000, true, true,
//                        "Yes we can!", QRectF(-100, -100, 100, 50), Qt::AlignCenter,
//                        QPen(), QFont(),
//                        QPen(Qt::DashLine));
//    addHighlightingText(2500, true, false,
//                        "Text spreading over\ntwo lines", QRectF(0, 0, 150, 35), Qt::AlignLeft);
}

HighlightableGraphicsView::~HighlightableGraphicsView()
{
    stopHighlighting();
}

void HighlightableGraphicsView::setHighlighter(GraphicsViewHighlighter *highlighter)
{
    if(m_highlighter == highlighter) {
        return;
    }

    if(m_highlighter != nullptr) {
        disconnect(m_highlighter, 0, this, 0);
    }

    m_highlighter = highlighter;
    if(m_highlighter != nullptr) {
        connect(m_highlighter, &GraphicsViewHighlighter::startRequested,   this, &HighlightableGraphicsView::onHighlighterNeedsStart);
        connect(m_highlighter, &GraphicsViewHighlighter::suspendRequested, this, &HighlightableGraphicsView::onHighlighterNeedsSuspend);
        connect(m_highlighter, &GraphicsViewHighlighter::stopRequested,    this, &HighlightableGraphicsView::onHighlighterNeedsStop);
    }
}

int HighlightableGraphicsView::highlightingDataAcceleration() const {return m_highlightingDataAcceleration;}
void HighlightableGraphicsView::setHighlightingDataAcceleration(int acc)
{
    if(m_highlightingDataAcceleration == acc) { // no need to check range since it's checked in HighlightingData::setAcceleration
        return;
    }

    m_highlightingDataAcceleration = acc;
    for(HighlightingData *data : m_highlightingData) {
        data->setAcceleration(acc);
    }

    emit highlightingDataAccelerationChanged();
}

void HighlightableGraphicsView::addHighlighting(HighlightingData *data)
{
    if(!m_highlightingData.contains(data)) {
        m_highlightingData.append(data);
        data->setAcceleration(m_highlightingDataAcceleration);

        connect(data, &HighlightingData::timerStopped, [this, data]() {
            m_highlightingData.removeOne(data);
            if(m_highlightingNewedData.removeOne(data)) {
                delete data;
            }
        });
    }
}

void HighlightableGraphicsView::addHighlighting(HighlightingData *data, bool inForeground)
{
    data->inForeground = inForeground;
    addHighlighting(data);
}

void HighlightableGraphicsView::addHighlightingNewedData(HighlightingData *data, bool inForeground)
{
    addHighlighting(data, inForeground);
    m_highlightingNewedData.append(data);

    data->startTimer(); // start timer after data has been registered,
                        // since starting timer sets the visibility to false until timer finishes.
}

void HighlightableGraphicsView::addHighlightingText(int duration, bool inForeground, bool singleShotTimer,
                                                    const QString &text, const QRectF &rect, int flags,
                                                    const QPen &textPen, const QFont &textFont,
                                                    const QPen &rectPen, const QBrush &rectBrush)
{
    auto *data = new HighlightingTexTData;
    data->configureTextData(text, rect, flags);
    data->configureTextData(textPen, textFont);
    data->configureTextData(rectPen, rectBrush);

    data->setDuration(duration);
    data->setTimerSingleShot(singleShotTimer);
    addHighlightingNewedData(data, inForeground);
}

void HighlightableGraphicsView::suspendHighlighting()
{
    for(HighlightingData *data : m_highlightingData) {
        data->suspendTimer();
    }
}

void HighlightableGraphicsView::resumeHighlighting()
{
    for(HighlightingData *data : m_highlightingData) {
        data->resumeTimer();
    }
}

void HighlightableGraphicsView::stopHighlighting()
{
    for(HighlightingData *data : m_highlightingData) {
        data->stopTimer();
    }
}

void HighlightableGraphicsView::onHighlighterNeedsStart(HighlightingData *data, int duration)
{
    addHighlighting(data);

    data->setDuration(duration);
    data->setAcceleration(m_highlightingDataAcceleration);
    data->startTimer();
}

void HighlightableGraphicsView::onHighlighterNeedsSuspend(HighlightingData *data)
{
    data->setAcceleration(m_highlightingDataAcceleration);
    data->suspendTimer();
}

void HighlightableGraphicsView::onHighlighterNeedsStop(HighlightingData *data)
{
    data->setAcceleration(m_highlightingDataAcceleration);
    data->stopTimer();
}

void HighlightableGraphicsView::drawAllHighlightingData(QPainter *painter)
{
    drawAllHighlightingData(painter, [](HighlightingData*){return true;});
}

void HighlightableGraphicsView::drawAllHighlightingData(QPainter *painter, std::function<bool (HighlightingData*)> predicate)
{
    for(HighlightingData *data : m_highlightingData) {
        if(predicate(data)) {
            drawHighlightingData(painter, data);
        }
    }

    update();
}

void HighlightableGraphicsView::drawHighlightingData(QPainter *painter, HighlightingData *data)
{
    auto *textData = dynamic_cast<HighlightingTexTData*>(data);
    if(textData) {
        // First draw rect border
        painter->setPen(textData->rectPen);
        painter->setBrush(textData->rectBrush);
        painter->setOpacity(textData->rectOpacity); // change opacity first
        painter->drawRect(textData->rect);

        // Then draw text
        painter->setPen(textData->textPen);
        painter->setFont(textData->textFont);
        painter->setOpacity(textData->textOpaticy); // change opacity first
        painter->drawText(textData->rect, textData->textFlags, textData->text);

        return;
    }
}

void HighlightableGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    GraphicsView::drawBackground(painter, rect);
    drawAllHighlightingData(painter, [](HighlightingData *data){return data->inForeground==false;});
}

void HighlightableGraphicsView::drawForeground(QPainter *painter, const QRectF &rect)
{
    GraphicsView::drawForeground(painter, rect);
    drawAllHighlightingData(painter, [](HighlightingData *data){return data->inForeground==true;});
}
