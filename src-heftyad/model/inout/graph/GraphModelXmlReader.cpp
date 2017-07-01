#include "GraphModel.h"
#include "GraphModelVertex.h"
#include "GraphModelEdge.h"
#include "GraphModelXmlReader.h"

#include "ModelReaderXmlHelper.h"

#include <QXmlStreamReader>

GraphModelXmlReader::GraphModelXmlReader(GraphModel *graphModel)
    : ModelReader(graphModel)
{
}

bool GraphModelXmlReader::read(QIODevice &in)
{
    auto *graph = static_cast<GraphModel*>(model());
    if(!graph) {
        return false;
    }

    QXmlStreamReader stream(&in);

    stream.readNextStartElement(); // reads "graph"
    {
        stream.readNextStartElement(); // reads "vertices"
        {
            readVertices(graph, stream);
        }

        stream.readNextStartElement(); // reads "edges"
        {
            readEdges(graph, stream);
        }
        stream.skipCurrentElement();
    }

    return true;
}

void GraphModelXmlReader::readVertices(GraphModel *model, QXmlStreamReader &stream)
{
    while(stream.readNextStartElement()) {
        if(stream.name().compare(QString("vertex")) == 0) {
            GraphModelVertex &vertex(model->addNewVertex());
            ModelReaderXmlHelper::readGraphModelVertexFrom(stream, vertex);

            stream.skipCurrentElement();
        }
    }
}

void GraphModelXmlReader::readEdges(GraphModel *model, QXmlStreamReader &stream)
{
    while(stream.readNextStartElement()) {
        if(stream.name().compare(QString("edge")) == 0) {
            // First read main attributes (required to create an edge)

            const QXmlStreamAttributes &attrs(stream.attributes());
            bool ok1=false, ok2=false;
            // Here are the 03 main attributes
            int endpt1 = attrs.value("endpoint1").toInt(&ok1);
            int endpt2 = attrs.value("endpoint2").toInt(&ok2);
            bool bidir = attrs.value("bidir").toString() == "true";

            // Then create edge (using main attributes)

            const bool cond1 = ok1 && (endpt1>=0 && endpt1<model->vertexCount());
            const bool cond2 = ok2 && (endpt2>=0 && endpt2<model->vertexCount());
            if(cond1 && cond2) {
                const auto &list(model->vertices());
                GraphModelEdge &edge(model->addNewEdge(*list.at(endpt1), *list.at(endpt2), bidir));
                ModelReaderXmlHelper::readGraphModelEdgeFrom(stream, edge);
            }

            stream.skipCurrentElement();
        }
    }
}
