#include "ModelReaderXmlHelper.h"

#include "ModelItem.h"
#include "ModelMultiShapeItem.h"
//
#include "ModelNode.h"
#include "ModelCurve.h"
//
#include "ArrayModelItem.h"
//
#include "GraphModelVertex.h"
#include "GraphModelEdge.h"
//
#include "dataserializer/DataXmlReader.h"

#include <QXmlStreamReader>

void ModelReaderXmlHelper::readModelItemFrom(QXmlStreamReader &stream, ModelItem &item)
{
    const QXmlStreamAttributes &attrs(stream.attributes());

    item.setValue(attrs.value("value").toString());
    item.setOpacity(attrs.value("opacity").toDouble());
    item.setX(attrs.value("posX").toDouble());
    item.setY(attrs.value("posY").toDouble());
    item.setValueEditorPos(DataXmlReader::readQPointFAttr(attrs, "editorPos"));
    item.setBrush(DataXmlReader::readQBrush(stream));
    item.setPen(DataXmlReader::readQPen(stream));
    item.setFont(DataXmlReader::readQFont(stream));
}

void ModelReaderXmlHelper::readModelMultiShapeItemFrom(QXmlStreamReader &stream, ModelMultiShapeItem &item)
{
    readModelItemFrom(stream, item);

    stream.readNextStartElement(); // reads "shape"
    {
        const QXmlStreamAttributes &attrs(stream.attributes());
        bool ok = false;
        int k = 0;

        ok = false; k = attrs.value("kind").toInt(&ok);
        item.switchToShapeKind(ok ? k : item.shapeKind());
        item.setRounded(attrs.value("rounded").toString() == "true");
        switch(item.shapeKind()) {
        case ModelMultiShapeItem::Circle:
            item.setDimension(attrs.value("diameter").toDouble());
            break;
        case ModelMultiShapeItem::Rectangle:
            item.setDimension(QSizeF(attrs.value("width").toDouble(), attrs.value("height").toDouble()));
            break;
        }
    }
    stream.skipCurrentElement();
}

void ModelReaderXmlHelper::readModelNodeFrom(QXmlStreamReader &stream, ModelNode &node)
{
    readModelMultiShapeItemFrom(stream, node);
}

void ModelReaderXmlHelper::readModelCurveFrom(QXmlStreamReader &stream, ModelCurve &curve)
{
    // Read ModelCurve attrs
    /*
     * Since this function rquires a curve,
     * any attribute required to create a curve should be read first,
     * outside of this function.
     */

    const QXmlStreamAttributes &attrs(stream.attributes());
    bool ok = false;
    int k = 0;

    // main attributes are supposed read (outside this function)

    curve.setArrowSize(attrs.value("arrowSize").toInt());
    for(int i=0; i<4; i++) {
        const QString &str(QString("point%1").arg(i+1));
        curve.setPointPos(i, DataXmlReader::readQPointFAttr(attrs, str));
    }
    ok = false; k = attrs.value("pointRole").toInt(&ok);
    curve.setPointRole(ok ? k : curve.pointRole());

    // Read other attributes

    stream.readNextStartElement(); // reads "common"
    {
        readModelItemFrom(stream, curve);
    }
    stream.skipCurrentElement();
}

void ModelReaderXmlHelper::readArrayModelItemFrom(QXmlStreamReader &stream, ArrayModelItem &item)
{
    readModelMultiShapeItemFrom(stream, item);
}

void ModelReaderXmlHelper::readGraphModelVertexFrom(QXmlStreamReader &stream, GraphModelVertex &vertex)
{
    readModelMultiShapeItemFrom(stream, vertex);
}

void ModelReaderXmlHelper::readGraphModelEdgeFrom(QXmlStreamReader &stream, GraphModelEdge &edge)
{
    readModelCurveFrom(stream, edge);
}
