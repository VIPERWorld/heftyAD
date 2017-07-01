#ifndef HIGHLIGHTINGTEXTDATA_H
#define HIGHLIGHTINGTEXTDATA_H

#include "HighlightingData.h"

#include <QFont>
#include <QObject>
#include <QPen>

class HighlightingTextData : public HighlightingData
{
    Q_OBJECT

    Q_PROPERTY(QString text MEMBER text)
    Q_PROPERTY(QRectF  rect MEMBER rect)

    Q_PROPERTY(int   textFlags   MEMBER textFlags)
    Q_PROPERTY(QPen  textPen     MEMBER textPen)
    Q_PROPERTY(QFont textFont    MEMBER textFont)
    Q_PROPERTY(qreal textOpaticy MEMBER textOpaticy)

    Q_PROPERTY(QPen   rectPen     MEMBER rectPen)
    Q_PROPERTY(QBrush rectBrush   MEMBER rectBrush)
    Q_PROPERTY(qreal  rectOpacity MEMBER rectOpacity)

public:
    Q_INVOKABLE HighlightingTextData();

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
    Q_INVOKABLE void configureTextData(const QString &text, const QRectF &rect, int textFlags);
    Q_INVOKABLE void configureTextData(const QPen &textPen, const QFont &textFont, qreal textOpaticy = -1);
    Q_INVOKABLE void configureTextData(const QPen &rectPen, const QBrush &rectBrush, qreal rectOpacity = -1);
};

#endif // HIGHLIGHTINGTEXTDATA_H
