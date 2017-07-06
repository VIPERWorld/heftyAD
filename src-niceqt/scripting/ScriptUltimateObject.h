#ifndef SCRIPTULTIMATEOBJECT_H
#define SCRIPTULTIMATEOBJECT_H

#include <QBrush>
#include <QColor>
#include <QFont>
#include <QJSValue>
#include <QObject>
#include <QPen>
#include <QRectF>

/**
 * The ScriptUltimateObject class can be exposed to any script engine.
 * It provides several functions to create stuffs like:
 *     QColor, QRectF, ...
 */
class ScriptUltimateObject : public QObject
{
    Q_OBJECT

public:
    explicit ScriptUltimateObject(QObject *parent = 0);

    Q_INVOKABLE QColor getColor(const QString &name) const;
    Q_INVOKABLE QColor getColor(int r, int g, int b, int a = 255) const;
    Q_INVOKABLE QColor getColor(const QJSValue &value) const;
    Q_INVOKABLE QString getColorStr(const QColor &color) const;

    Q_INVOKABLE QBrush getBrush(const QColor &color, int style = Qt::SolidPattern) const;
    Q_INVOKABLE QBrush getBrush(const QJSValue &value) const;
    Q_INVOKABLE QString getBrushStr(const QBrush &brush) const;

    Q_INVOKABLE QFont getFont(const QString &family, int pointSize = -1) const;
    Q_INVOKABLE QFont getFont(const QJSValue &value) const;
    Q_INVOKABLE QString getFontStr(const QFont &font) const;

    Q_INVOKABLE QPen getPen(const QBrush &brush, qreal width = 1., int style = Qt::SolidLine) const;
    Q_INVOKABLE QPen getPen(const QJSValue &value) const;
    Q_INVOKABLE QString getPenStr(const QPen &pen) const;

    Q_INVOKABLE QPointF getPointF(qreal x, qreal y) const;
    Q_INVOKABLE QPointF getPointF(const QJSValue &value) const;
    Q_INVOKABLE QString getPointFStr(const QPointF &point) const;

    Q_INVOKABLE QRectF getRectF(qreal x, qreal y, qreal width, qreal height) const;
    Q_INVOKABLE QRectF getRectF(const QJSValue &value) const;
    Q_INVOKABLE QString getRectFStr(const QRectF &rect) const;

signals:

public slots:
};

#endif // SCRIPTULTIMATEOBJECT_H
