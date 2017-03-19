#ifndef DATAXMLWRITER_H
#define DATAXMLWRITER_H

#include <QObject>

class QXmlStreamWriter;
//
class QGradient;
class QConicalGradient;
class QLinearGradient;
class QRadialGradient;

class DataXmlWriter : public QObject
{
    Q_OBJECT

public:
    /* Disable creation and copy (using C++11 syntax). */
    DataXmlWriter() = delete;
    DataXmlWriter(const DataXmlWriter&) = delete;
    DataXmlWriter& operator=(const DataXmlWriter&) = delete;
    DataXmlWriter(const DataXmlWriter&&) = delete;

    static void writeQBrush (QXmlStreamWriter &stream, const QString &eltName, const QBrush &brush);
    static void writeQColor (QXmlStreamWriter &stream, const QString &eltName, const QColor &color);
    static void writeQFont  (QXmlStreamWriter &stream, const QString &eltName, const QFont &font);
    static void writeQPen   (QXmlStreamWriter &stream, const QString &eltName, const QPen &pen);
    static void writeQPixmap(QXmlStreamWriter &stream, const QString &eltName, const QPixmap &pixmap, const char *format);

    static void writeQGradient       (QXmlStreamWriter &stream, const QString &eltName, const QGradient &gradient);
    static void writeQConicalGradient(QXmlStreamWriter &stream, const QConicalGradient &gradient);
    static void writeQLinearGradient (QXmlStreamWriter &stream, const QLinearGradient &gradient);
    static void writeQRadialGradient (QXmlStreamWriter &stream, const QRadialGradient &gradient);

    static void writeQPointFAttr(QXmlStreamWriter &stream, const QString &attrName, const QPointF &point);
    static void writeQPointFElt (QXmlStreamWriter &stream, const QString &eltName,  const QPointF &point);

private:
    static void writeQGradientBase(QXmlStreamWriter &stream, const QGradient &gradient);
};

#endif // DATAXMLWRITER_H
