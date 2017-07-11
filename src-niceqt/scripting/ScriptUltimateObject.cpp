#include "ScriptUltimateObject.h"

ScriptUltimateObject::ScriptUltimateObject(QObject *parent)
    : QObject(parent)
{
}

QColor ScriptUltimateObject::getColor(const QString &name) const {return QColor(name);}
QColor ScriptUltimateObject::getColor(int r, int g, int b, int a) const {return QColor(r, g, b, a);}
QColor ScriptUltimateObject::getColor(const QJSValue &value) const
{
    const int r = value.property("r").toInt();
    const int g = value.property("g").toInt();
    const int b = value.property("b").toInt();
    const int a = value.hasProperty("a") ? value.property("a").toInt() : 255;
    return getColor(r, g, b, a);
}
QString ScriptUltimateObject::getColorStr(const QColor &color) const
{
    const int &r = color.red();
    const int &g = color.green();
    const int &b = color.blue();
    const int &a = color.alpha();
    return QString("r=%1 g=%2 b=%3 a=%4").arg(r).arg(g).arg(b).arg(a);
}

QBrush ScriptUltimateObject::getBrush(const QColor &color, int style) const {return QBrush(color, (Qt::BrushStyle)style);}
QBrush ScriptUltimateObject::getBrush(const QJSValue &value) const
{
    const QColor &color = getColor(value.property("color"));
    const int style = value.hasProperty("style") ? value.property("style").toInt() : Qt::SolidPattern;
    return getBrush(color, style);
}
QString ScriptUltimateObject::getBrushStr(const QBrush &brush) const
{
    return getColorStr(brush.color()) + " | " + QString("style=%1").arg(brush.style());
}

QFont ScriptUltimateObject::getFont(const QString &family, int pointSize) const {return QFont(family, pointSize);}
QFont ScriptUltimateObject::getFont(const QJSValue &value) const
{
    QFont font;
    font.setFamily(value.property("family").toString());
    font.setPointSizeF(value.property("pointSize").toNumber());
    font.setWeight(value.property("weight").toInt());
    font.setBold(value.property("bold").toBool());
    font.setItalic(value.property("italic").toBool());
    font.setStrikeOut(value.property("strikeout").toBool());
    font.setUnderline(value.property("underline").toBool());
    font.setOverline(value.property("overline").toBool());
    return font;
}
QString ScriptUltimateObject::getFontStr(const QFont &font) const
{
    return QString("family=%1 pointSize=%2 weight=%3 bold=%4 italic=%5 strikeout=%6 underline=%7 overline=%8")
            .arg(font.family()).arg(font.pointSizeF()).arg(font.weight())
            .arg(font.bold()).arg(font.italic())
            .arg(font.strikeOut()).arg(font.underline()).arg(font.overline());
}

QPen ScriptUltimateObject::getPen(const QBrush &brush, qreal width, int style) const {return QPen(brush, width, (Qt::PenStyle)style);}
QPen ScriptUltimateObject::getPen(const QJSValue &value) const
{
    const QBrush &brush = getBrush(value.property("brush"));
    const qreal width = value.hasProperty("width") ? value.property("width").toNumber() : 1.;
    const int style = value.hasProperty("style") ? value.property("style").toInt() : Qt::SolidLine;
    return getPen(brush, width, style);
}
QString ScriptUltimateObject::getPenStr(const QPen &pen) const
{
    return getBrushStr(pen.brush()) + " | " + QString("width=%1 style=%2").arg(pen.widthF()).arg(pen.style());
}

QPointF ScriptUltimateObject::getPointF(qreal x, qreal y) const {return QPointF(x, y);}
QPointF ScriptUltimateObject::getPointF(const QJSValue &value) const
{
    const qreal x = value.property("x").toNumber();
    const qreal y = value.property("y").toNumber();
    return getPointF(x, y);
}
QString ScriptUltimateObject::getPointFStr(const QPointF &point) const
{
    return QString("x=%1 y=%2").arg(point.x()).arg(point.y());
}

QSizeF ScriptUltimateObject::getSizeF(qreal width, qreal height) const {return QSizeF(width, height);}
QSizeF ScriptUltimateObject::getSizeF(const QJSValue &value) const
{
    const qreal w = value.property("w").toNumber();
    const qreal h = value.property("h").toNumber();
    return getSizeF(w, h);
}
QString ScriptUltimateObject::getSizeFStr(const QSizeF &size) const
{
    return QString("w=%1 h=%2").arg(size.width()).arg(size.height());
}

QRectF ScriptUltimateObject::getRectF(qreal x, qreal y, qreal width, qreal height) const {return QRectF(x, y, width, height);}
QRectF ScriptUltimateObject::getRectF(const QJSValue &value) const
{
    const qreal x = value.property("x").toNumber();
    const qreal y = value.property("y").toNumber();
    const qreal w = value.property("w").toNumber();
    const qreal h = value.property("h").toNumber();
    return getRectF(x, y, w, h);
}
QString ScriptUltimateObject::getRectFStr(const QRectF &rect) const
{
    return QString("x=%1 y=%2 w=%3 h=%4").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
}
