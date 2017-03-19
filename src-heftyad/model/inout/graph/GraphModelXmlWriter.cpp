#include "GraphModel.h"
#include "GraphModelEdge.h"
#include "GraphModelVertex.h"
#include "GraphModelXmlWriter.h"

#include "ModelWriterXmlHelper.h"

#include <QXmlStreamWriter>

GraphModelXmlWriter::GraphModelXmlWriter(GraphModel *graphModel)
    : ModelWriter(graphModel)
{
}

bool GraphModelXmlWriter::write(QIODevice &out)
{
    auto *graph = static_cast<GraphModel*>(model());
    if(!graph) {
        return false;
    }

    QXmlStreamWriter stream(&out);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("graph");
    {
        writeVertices(graph, stream);
        writeEdges(graph, stream);
    }
    stream.writeEndElement();

    stream.writeEndDocument();

    return true;
}

void GraphModelXmlWriter::writeVertices(GraphModel *model, QXmlStreamWriter &stream)
{
    stream.writeStartElement("vertices");
    {
        stream.writeAttribute("count", QString::number(model->vertexCount()));
        for(GraphModelVertex *vertex : model->vertices()) {
            stream.writeStartElement("vertex");
            {
                ModelWriterXmlHelper::writeGraphModelVertexTo(stream, *vertex);
            }
            stream.writeEndElement();
        }
    }
    stream.writeEndElement();
}

void GraphModelXmlWriter::writeEdges(GraphModel *model, QXmlStreamWriter &stream)
{
    QMap<ModelNode*, int> vertexIndexes;
    const QVector<GraphModelVertex*> &v(model->vertices());
    for(int i=0; i<v.size(); i++) {
        vertexIndexes[v[i]] = i;
    }

    stream.writeStartElement("edges");
    {
        stream.writeAttribute("count", QString::number(model->edgeCount()));
        for(GraphModelEdge *edge : model->edges()) {
            stream.writeStartElement("edge");
            {
                ModelWriterXmlHelper::writeGraphModelEdgeTo(stream, *edge, vertexIndexes);
            }
            stream.writeEndElement();
        }
    }
    stream.writeEndElement();
}
