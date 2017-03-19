#ifndef GRAPHMODELXMLREADER_H
#define GRAPHMODELXMLREADER_H

#include "ModelReader.h"

class GraphModel;

class QXmlStreamReader;

class GraphModelXmlReader : public ModelReader
{
    Q_OBJECT

public:
    explicit GraphModelXmlReader(GraphModel *graphModel);

    bool read(QIODevice &in) override;

private:
    void readVertices(GraphModel *model, QXmlStreamReader &stream);
    void readEdges(GraphModel *model, QXmlStreamReader &stream);
};

#endif // GRAPHMODELXMLREADER_H
