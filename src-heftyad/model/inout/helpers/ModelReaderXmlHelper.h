#ifndef MODELREADERXMLHELPER_H
#define MODELREADERXMLHELPER_H

class ModelItem;
class ModelMultiShapeItem;
//
class ModelNode;
class ModelCurve;
//
class ArrayModelItem;
//
class GraphModelVertex;
class GraphModelEdge;

class QXmlStreamAttributes;
class QXmlStreamReader;

#include <QObject>

/**
 * This class shouldn't normally exist.
 * Please take a look at the documentation in its associated ModelWriterXmlHelper class.
 */

class ModelReaderXmlHelper : public QObject
{
    Q_OBJECT

public:
    ModelReaderXmlHelper() = delete;

    static void readModelItemFrom(QXmlStreamReader &stream, ModelItem &item);
    static void readModelMultiShapeItemFrom(QXmlStreamReader &stream, ModelMultiShapeItem &item);

    static void readModelNodeFrom(QXmlStreamReader &stream, ModelNode &node);
    static void readModelCurveFrom(QXmlStreamReader &stream, ModelCurve &curve);

    static void readArrayModelItemFrom(QXmlStreamReader &stream, ArrayModelItem &item);

    static void readGraphModelVertexFrom(QXmlStreamReader &stream, GraphModelVertex &vertex);
    static void readGraphModelEdgeFrom(QXmlStreamReader &stream, GraphModelEdge &edge);
};

#endif // MODELREADERXMLHELPER_H
