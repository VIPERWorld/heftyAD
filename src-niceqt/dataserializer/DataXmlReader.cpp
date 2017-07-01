#include "DataXmlReader.h"

#include <QXmlStreamAttributes>
//
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QPointF>
#include <QVariant>

QBrush DataXmlReader::readQBrush(QXmlStreamReader &stream)
{
    QBrush brush;

    stream.readNextStartElement();
    {
        const QXmlStreamAttributes &attrs(stream.attributes());
        bool ssStyle = false; // ssStyle := should set style

        // the attributes we will read

        int style;
        QColor color;
        QPixmap texture;

        // read style and color

        bool validStyle = false;
        style = attrs.value("style").toInt(&validStyle);
        validStyle = validStyle && style>=Qt::NoBrush && style<=Qt::TexturePattern;
        if(!validStyle) {
            style = Qt::NoBrush; // style for default-constructed brushes
        }

        color = readQColor(stream);

        // read texture or gradient

        switch(style) {
        case Qt::TexturePattern:
            texture = readQPixmap(stream);
            break;

        // We first read the "gradient" element and then read the appropriate gradient data
        case Qt::LinearGradientPattern:  stream.readNextStartElement(); brush = readQLinearGradient(stream);  stream.skipCurrentElement(); break;
        case Qt::RadialGradientPattern:  stream.readNextStartElement(); brush = readQRadialGradient(stream);  stream.skipCurrentElement(); break;
        case Qt::ConicalGradientPattern: stream.readNextStartElement(); brush = readQConicalGradient(stream); stream.skipCurrentElement(); break;

        default:
            ssStyle = true;
            break;
        }

        // set attributes

        if(ssStyle) { // just to avoid Qt from issuing a warning message to the console
            brush.setStyle((Qt::BrushStyle) style);
        }
        brush.setColor(color);
        if(!texture.isNull()) {
            brush.setTexture(texture);
        }
    }
    stream.skipCurrentElement();

    return brush;
}

QColor DataXmlReader::readQColor(QXmlStreamReader &stream)
{
    QColor color;

    stream.readNextStartElement();
    {
        const QXmlStreamAttributes &attrs(stream.attributes());
        bool ok = false;

        // attributes we want to read

        QString rgb;
        int a;

        // read attributes

        rgb = attrs.value("rgb").toString();
        a = attrs.value("r").toInt(&ok);

        // set attributes

        color.setNamedColor(rgb);
        if(ok && a>=0 && a<=255) {
           color.setAlpha(a);
        }
    }
    stream.skipCurrentElement();

    return color;
}

QFont DataXmlReader::readQFont(QXmlStreamReader &stream)
{
    QFont font;

    stream.readNextStartElement();
    {
        const QXmlStreamAttributes &attrs(stream.attributes());
        bool ok = false;

        font.setFamily(attrs.value("family").toString());
        qreal pointSize = attrs.value("pointSize").toDouble(&ok);
        if(ok && pointSize>0) {
            font.setPointSizeF(pointSize);
        }
        font.setWeight(attrs.value("weight").toInt());
        font.setBold(attrs.value("bold").toString() == "true");
        font.setItalic(attrs.value("italic").toString() == "true");
        font.setStrikeOut(attrs.value("strikeout").toString() == "true");
        font.setUnderline(attrs.value("underline").toString() == "true");
        font.setOverline(attrs.value("overline").toString() == "true");
    }
    stream.skipCurrentElement();

    return font;
}

QPen DataXmlReader::readQPen(QXmlStreamReader &stream)
{
    QPen pen;

    stream.readNextStartElement();
    {
        const QXmlStreamAttributes &attrs(stream.attributes());
        bool ok = false;

        int penStyle = attrs.value("style").toInt(&ok);
        if(ok && penStyle>=Qt::NoPen && penStyle<=Qt::CustomDashLine) {
            pen.setStyle((Qt::PenStyle) penStyle);
        }
        qreal penWidth = attrs.value("width").toDouble(&ok);
        if(ok && penWidth>=0) {
            pen.setWidthF(penWidth);
        }
        pen.setColor(readQColor(stream));
    }
    stream.skipCurrentElement();

    return pen;
}

QPixmap DataXmlReader::readQPixmap(QXmlStreamReader &stream)
{
    QPixmap pixmap;

    stream.readNextStartElement();
    {
        const QXmlStreamAttributes &attrs(stream.attributes());

        const QString &format(attrs.value("format").toString());
        const QByteArray &data(attrs.value("data").toLatin1());

        QByteArray bytes = QByteArray::fromBase64(data);
        pixmap.loadFromData(bytes, format.toStdString().data());
    }
    stream.skipCurrentElement();

    return pixmap;
}

QConicalGradient DataXmlReader::readQConicalGradient(QXmlStreamReader &stream)
{
    QConicalGradient gradient;

    readQGradientBase(stream, gradient);
    stream.readNextStartElement(); // reads "specific"
    {
        const QXmlStreamAttributes &attrs(stream.attributes());

        gradient.setAngle(attrs.value("angle").toDouble());
        gradient.setCenter(readQPointFAttr(attrs, "center"));
    }
    stream.skipCurrentElement();

    return gradient;
}

QLinearGradient DataXmlReader::readQLinearGradient(QXmlStreamReader &stream)
{
    QLinearGradient gradient;

    readQGradientBase(stream, gradient);
    stream.readNextStartElement(); // reads "specific"
    {
        const QXmlStreamAttributes &attrs(stream.attributes());

        gradient.setStart    (readQPointFAttr(attrs, "start"));
        gradient.setFinalStop(readQPointFAttr(attrs, "final"));
    }
    stream.skipCurrentElement();

    return gradient;
}

QRadialGradient DataXmlReader::readQRadialGradient(QXmlStreamReader &stream)
{
    QRadialGradient gradient;

    readQGradientBase(stream, gradient);
    stream.readNextStartElement(); // reads "specific"
    {
        const QXmlStreamAttributes &attrs(stream.attributes());

        gradient.setCenter(readQPointFAttr(attrs, "center"));
        gradient.setFocalPoint(readQPointFAttr(attrs, "focal"));
        gradient.setCenterRadius(attrs.value("centerRadius").toDouble());
        gradient.setFocalRadius(attrs.value("focalRadius").toDouble());
    }
    stream.skipCurrentElement();

    return gradient;
}

QPointF DataXmlReader::readQPointFAttr(const QXmlStreamAttributes &attrs, const QString &attrName)
{
    QPointF retVal;

    const QString &val(attrs.value(attrName).toString());
    const QStringList &vals(val.split("!"));
    if(vals.size() >= 2) {
        retVal = QPointF(QVariant(vals[0]).toReal(), QVariant(vals[1]).toReal());
    }

    return retVal;
}

QPointF DataXmlReader::readQPointFElt(QXmlStreamReader &stream)
{
    stream.readNextStartElement();
    {
        const QXmlStreamAttributes &attrs(stream.attributes());
        return QPointF(attrs.value("x").toDouble(), attrs.value("y").toDouble());
    }
    stream.skipCurrentElement();
}

void DataXmlReader::readQGradientBase(QXmlStreamReader &stream, QGradient &gradient)
{
    stream.readNextStartElement(); // reads "common"
    {
        const QXmlStreamAttributes &attrs(stream.attributes());
        int val;
        bool ok = false;

        // reading attributes (NO NEED to read the type)

        val = attrs.value("coordMode").toInt(&ok);
        if(!ok || val<QGradient::LogicalMode || val>QGradient::ObjectBoundingMode) {
            val = QGradient::LogicalMode; // set to default mode
        }
        gradient.setCoordinateMode((QGradient::CoordinateMode) val);

        val = attrs.value("spread").toInt(&ok);
        if(!ok || val<QGradient::PadSpread || val>QGradient::RepeatSpread) {
            val = QGradient::PadSpread; // set to default spread
        }
        gradient.setSpread((QGradient::Spread) val);

        stream.readNextStartElement(); // reads "stopPts"
        {
            while(stream.readNextStartElement()) {
                if(stream.name().compare(QString("stopPt")) == 0) {
                    const QXmlStreamAttributes &attrs(stream.attributes());
                    bool ok = false;

                    qreal pos = attrs.value("pos").toDouble(&ok);
                    if(ok && pos>=0 && pos<=1) {
                        gradient.setColorAt(pos, readQColor(stream));
                    }
                }

                stream.skipCurrentElement();
            } // stream.name() now refers to "stopPts"
        }
        stream.skipCurrentElement();
    }
}
