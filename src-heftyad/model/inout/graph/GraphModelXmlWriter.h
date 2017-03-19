#ifndef GRAPHMODELXMLWRITER_H
#define GRAPHMODELXMLWRITER_H

#include "ModelWriter.h"

class GraphModel;

class QXmlStreamWriter;

class GraphModelXmlWriter : public ModelWriter
{
    Q_OBJECT

public:
    explicit GraphModelXmlWriter(GraphModel *graphModel);

    bool write(QIODevice &out) override;

private:
    void writeVertices(GraphModel *model, QXmlStreamWriter &stream);
    void writeEdges(GraphModel *model, QXmlStreamWriter &stream);
};

#endif // GRAPHMODELXMLWRITER_H
