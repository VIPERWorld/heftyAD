#include "DataXmlWriter.h"

#include <QXmlStreamWriter>
//
#include <QBrush>
#include <QBuffer>
#include <QFont>
#include <QPen>
#include <QPointF>
#include <QVariant>

void DataXmlWriter::writeQBrush(QXmlStreamWriter &stream, const QString &eltName, const QBrush &brush)
{
    stream.writeStartElement(eltName);
    {
        stream.writeAttribute("style", QString::number(brush.style()));
        writeQColor(stream, "color", brush.color());
        switch(brush.style()) {
        case Qt::TexturePattern:
            /*
             * For convenient reasons, we save the texture (a QPixmap) and not the texture image (a QImage),
             * since QPixmap is designed and optimized for showing images on screen (see doc).
             * Moreover QBrush::operator<< appears to do the same thing (read the article about Serializing Qt Data Types).
             */
            writeQPixmap(stream, "texture", brush.texture(), "png");
            break;

        case Qt::LinearGradientPattern:
        case Qt::RadialGradientPattern:
        case Qt::ConicalGradientPattern:
            writeQGradient(stream, "gradient", *brush.gradient());
            break;

        default:
            break;
        }
    }
    stream.writeEndElement();
}

void DataXmlWriter::writeQColor(QXmlStreamWriter &stream, const QString &eltName, const QColor &color)
{
    stream.writeStartElement(eltName);
    {
        stream.writeAttribute("rgb", color.name());
        stream.writeAttribute("a",   QString::number(color.alpha()));
    }
    stream.writeEndElement();
}

void DataXmlWriter::writeQFont(QXmlStreamWriter &stream, const QString &eltName, const QFont &font)
{
    stream.writeStartElement(eltName);
    {
        stream.writeAttribute("family",    font.family());
        stream.writeAttribute("pointSize", QVariant(font.pointSizeF()).toString());
        stream.writeAttribute("weight",    QVariant(font.weight()).toString());
        stream.writeAttribute("bold",      QVariant(font.bold()).toString());
        stream.writeAttribute("italic",    QVariant(font.italic()).toString());
        stream.writeAttribute("strikeout", QVariant(font.strikeOut()).toString());
        stream.writeAttribute("underline", QVariant(font.underline()).toString());
        stream.writeAttribute("overline",  QVariant(font.overline()).toString());
    }
    stream.writeEndElement();
}

void DataXmlWriter::writeQPen(QXmlStreamWriter &stream, const QString &eltName, const QPen &pen)
{
    stream.writeStartElement(eltName);
    {
        // Save same attributes as QPen::operator<< (read the document titled Serializing Qt Data Types)

        stream.writeAttribute("style", QString::number(pen.style()));
        stream.writeAttribute("width", QVariant(pen.widthF()).toString());
        writeQColor(stream, "color", pen.color());
    }
    stream.writeEndElement();
}

void DataXmlWriter::writeQPixmap(QXmlStreamWriter &stream, const QString &eltName, const QPixmap &pixmap, const char *format)
{
    stream.writeStartElement(eltName);
    {
        /*
         * Save the pixmap as a PNG image:
         *     see QPen::operator<< and read the document: Serializing Qt Data Types).
         *     see QPixmap::save(QIODevice *device, ...) doc too.
         */

        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, format); // writes pixmap into bytes in given format

        stream.writeAttribute("format", format);
        stream.writeAttribute("data",   bytes.toBase64());
    }
    stream.writeEndElement();
}

void DataXmlWriter::writeQGradient(QXmlStreamWriter &stream, const QString &eltName, const QGradient &gradient)
{
    stream.writeStartElement(eltName);
    {
        /*
         * Just to easier the reading process (by the reader class).
         * Indeed reading the key value will help determine the type of the gradient.
         * The appropriate gradient reader function could then be called.
         */
        stream.writeAttribute("key", QString::number(gradient.type()));

        switch(gradient.type()) {
        case QGradient::LinearGradient:  writeQLinearGradient (stream, reinterpret_cast<const QLinearGradient&> (gradient)); break;
        case QGradient::RadialGradient:  writeQRadialGradient (stream, reinterpret_cast<const QRadialGradient&> (gradient)); break;
        case QGradient::ConicalGradient: writeQConicalGradient(stream, reinterpret_cast<const QConicalGradient&>(gradient)); break;

        case QGradient::NoGradient: break;
        }
    }
    stream.writeEndElement();
}

void DataXmlWriter::writeQConicalGradient(QXmlStreamWriter &stream, const QConicalGradient &gradient)
{
    writeQGradientBase(stream, gradient);

    stream.writeStartElement("specific");
    {
        stream.writeAttribute("angle", QVariant(gradient.angle()).toString());
        writeQPointFAttr(stream, "center", gradient.center());
    }
    stream.writeEndElement();
}

void DataXmlWriter::writeQLinearGradient(QXmlStreamWriter &stream, const QLinearGradient &gradient)
{
    writeQGradientBase(stream, gradient);

    stream.writeStartElement("specific");
    {
        writeQPointFAttr(stream, "start", gradient.start());
        writeQPointFAttr(stream, "final", gradient.finalStop());
    }
    stream.writeEndElement();
}

void DataXmlWriter::writeQRadialGradient(QXmlStreamWriter &stream, const QRadialGradient &gradient)
{
    writeQGradientBase(stream, gradient);

    stream.writeStartElement("specific");
    {
        writeQPointFAttr(stream, "center", gradient.center());
        writeQPointFAttr(stream, "focal",  gradient.focalPoint());
        stream.writeAttribute("centerRadius", QVariant(gradient.centerRadius()).toString());
        stream.writeAttribute("focalRadius",  QVariant(gradient.focalRadius()).toString());
    }
    stream.writeEndElement();
}

void DataXmlWriter::writeQPointFAttr(QXmlStreamWriter &stream, const QString &attrName, const QPointF &point)
{
    stream.writeAttribute(attrName, QVariant(point.x()).toString()+"!"+QVariant(point.y()).toString());
}

void DataXmlWriter::writeQPointFElt(QXmlStreamWriter &stream, const QString &eltName, const QPointF &point)
{
    stream.writeStartElement(eltName);
    {
        stream.writeAttribute("x", QVariant(point.x()).toString());
        stream.writeAttribute("y", QVariant(point.y()).toString());
    }
    stream.writeEndElement();
}

void DataXmlWriter::writeQGradientBase(QXmlStreamWriter &stream, const QGradient &gradient)
{
    stream.writeStartElement("common");
    {
        stream.writeAttribute("type",      QString::number(gradient.type()));
        stream.writeAttribute("coordMode", QString::number(gradient.coordinateMode()));
        stream.writeAttribute("spread",    QString::number(gradient.spread()));
        stream.writeStartElement("stopPts");
        {
            const QGradientStops &stops(gradient.stops());

            stream.writeAttribute("count", QString::number(stops.size())); // not read when restoring gradient back
            for(int i=0; i<stops.size(); i++) {
                stream.writeStartElement("stopPt");
                {
                    stream.writeAttribute("pos", QVariant(stops[i].first).toString());
                    writeQColor(stream, "color", stops[i].second);
                }
                stream.writeEndElement();
            }
        }
        stream.writeEndElement();
    }
    stream.writeEndElement();
}
