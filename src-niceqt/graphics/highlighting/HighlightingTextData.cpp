#include "HighlightingTextData.h"

HighlightingTextData::HighlightingTextData()
    : HighlightingData()
{
    textFlags = Qt::AlignCenter;
    textOpaticy = 1.;
    rectOpacity = 1.;
    rectPen = Qt::NoPen;
    rectBrush = Qt::NoBrush;
}

void HighlightingTextData::configureTextData(const QString &text, const QRectF &rect, int textFlags)
{
    this->text = text;
    this->rect = rect;
    this->textFlags = textFlags;
}

void HighlightingTextData::configureTextData(const QPen &textPen, const QFont &textFont, qreal textOpaticy)
{
    this->textPen = textPen;
    this->textFont = textFont;
    if(textOpaticy >= 0) {
        this->textOpaticy = textOpaticy;
    }
}

void HighlightingTextData::configureTextData(const QPen &rectPen, const QBrush &rectBrush, qreal rectOpacity)
{
    this->rectPen = rectPen;
    this->rectBrush = rectBrush;
    if(rectOpacity >= 0) {
        this->rectOpacity = rectOpacity;
    }
}
