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

QFont ScriptUltimateObject::getFont(const QString &family, int pointSize, int weight, bool italic) const
{
    return QFont(family, pointSize, weight, italic);
}
QFont ScriptUltimateObject::getFont(const QJSValue &value) const
{
    QFont font;
    if(value.hasProperty("family")) font.setFamily(value.property("family").toString());
    if(value.hasProperty("pointSize")) font.setPointSizeF(value.property("pointSize").toNumber());
    if(value.hasProperty("weight")) font.setWeight(value.property("weight").toInt());
    font.setBold(value.property("bold").toBool());
    font.setItalic(value.property("italic").toBool());
    font.setStrikeOut(value.property("strikeout").toBool());
    font.setUnderline(value.property("underline").toBool());
    font.setOverline(value.property("overline").toBool());
    return font;
}

QPen ScriptUltimateObject::getPen(const QBrush &brush, qreal width, int style) const {return QPen(brush, width, (Qt::PenStyle)style);}
QPen ScriptUltimateObject::getPen(const QJSValue &value) const
{
    const QBrush &brush = getBrush(value.property("brush"));
    const qreal width = value.property("width").toNumber();
    const int style = value.property("style").toInt();
    return getPen(brush, width, style);
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
