#ifndef DATAXMLREADER_H
#define DATAXMLREADER_H

#include <QObject>

class QXmlStreamAttributes;
class QXmlStreamReader;
//
class QGradient;
class QConicalGradient;
class QLinearGradient;
class QRadialGradient;

class DataXmlReader : public QObject
{
    Q_OBJECT

public:
    /* Disable creation and copy (using C++11 syntax). */
    DataXmlReader() = delete;
    DataXmlReader(const DataXmlReader&) = delete;
    DataXmlReader& operator=(const DataXmlReader&) = delete;
    DataXmlReader(const DataXmlReader&&) = delete;

    static QBrush  readQBrush (QXmlStreamReader &stream);
    static QColor  readQColor (QXmlStreamReader &stream);
    static QFont   readQFont  (QXmlStreamReader &stream);
    static QPen    readQPen   (QXmlStreamReader &stream);
    static QPixmap readQPixmap(QXmlStreamReader &stream);

    static QConicalGradient readQConicalGradient(QXmlStreamReader &stream);
    static QLinearGradient  readQLinearGradient (QXmlStreamReader &stream);
    static QRadialGradient  readQRadialGradient (QXmlStreamReader &stream);

    static QPointF readQPointFAttr(const QXmlStreamAttributes &attrs, const QString &attrName);
    static QPointF readQPointFElt (QXmlStreamReader &stream);

private:
    static void readQGradientBase(QXmlStreamReader &stream, QGradient &gradient);
};

#endif // DATAXMLREADER_H
