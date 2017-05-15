#include "ModelWriterXmlHelper.h"

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
#include "dataserializer/DataXmlWriter.h"

#include <QXmlStreamWriter>

void ModelWriterXmlHelper::writeModelItemTo(QXmlStreamWriter &stream, const ModelItem &item)
{
    stream.writeAttribute("value",    item.value());
    stream.writeAttribute("opacity",  QVariant(item.opacity()).toString());
    stream.writeAttribute("posX",     QVariant(item.x()).toString());
    stream.writeAttribute("posY",     QVariant(item.y()).toString());
    DataXmlWriter::writeQPointFAttr(stream, "editorPos", item.valueEditorPos());
    DataXmlWriter::writeQBrush(stream, "brush", item.brush());
    DataXmlWriter::writeQPen  (stream, "pen",   item.pen());
    DataXmlWriter::writeQFont (stream, "font",  item.font());
}

void ModelWriterXmlHelper::writeModelMultiShapeItemTo(QXmlStreamWriter &stream, const ModelMultiShapeItem &item)
{
    writeModelItemTo(stream, item);

    stream.writeStartElement("shape");
    {
        const QVariant &dim(item.dimension());

        stream.writeAttribute("kind",    QVariant(item.shapeKind()).toString());
        stream.writeAttribute("rounded", QVariant(item.isRounded()).toString());
        switch(item.shapeKind()) {
        case ModelMultiShapeItem::Circle:
            stream.writeAttribute("diameter", dim.toString());
            break;
        case ModelMultiShapeItem::Rectangle:
            stream.writeAttribute("width",  QVariant(dim.toSizeF().width()).toString());
            stream.writeAttribute("height", QVariant(dim.toSizeF().height()).toString());
            break;
        }
    }
    stream.writeEndElement();
}

void ModelWriterXmlHelper::writeModelNodeTo(QXmlStreamWriter &stream, const ModelNode &node)
{
    writeModelMultiShapeItemTo(stream, node);
}

void ModelWriterXmlHelper::writeModelCurveTo(QXmlStreamWriter &stream, const ModelCurve &curve,
                                             const QMap<ModelNode*, int> &nodeIndexes)
{
    // Write ModelCurve attrs
    /*
     * Indeed we need to write the curve endpoints to the very beginning of the xml file,
     * so that we can access them for the reading purpose:
     * two nodes are needed to create an edge.
     */

    stream.writeAttribute("endpoint1", QString::number(nodeIndexes.value(&curve.node1())));
    stream.writeAttribute("endpoint2", QString::number(nodeIndexes.value(&curve.node2())));
    stream.writeAttribute("bidir",     QVariant(curve.isBidir()).toString());
    stream.writeAttribute("arrowSize", QVariant(curve.arrowSize()).toString());

    for(int i=0; i<4; i++) {
        const QString &str(QString("point%1").arg(i+1));
        DataXmlWriter::writeQPointFAttr(stream, str, curve.pointPos(i));
    }
    stream.writeAttribute("pointRole", QVariant(curve.pointRole()).toString());

    // Write other attributes

    stream.writeStartElement("common");
    {
        writeModelItemTo(stream, curve);
    }
    stream.writeEndElement();
}

void ModelWriterXmlHelper::writeArrayModelItemTo(QXmlStreamWriter &stream, const ArrayModelItem &item)
{
    writeModelMultiShapeItemTo(stream, item);
}

void ModelWriterXmlHelper::writeGraphModelVertexTo(QXmlStreamWriter &stream, const GraphModelVertex &vertex)
{
    writeModelNodeTo(stream, vertex);
}

void ModelWriterXmlHelper::writeGraphModelEdgeTo(QXmlStreamWriter &stream, const GraphModelEdge &edge,
                                                 const QMap<ModelNode*, int> &vertexIndexes)
{
    writeModelCurveTo(stream, edge, vertexIndexes);
}
