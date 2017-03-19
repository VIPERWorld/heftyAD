#ifndef ARRAYMODELXMLREADER_H
#define ARRAYMODELXMLREADER_H

#include "ModelReader.h"

class ArrayModel;

class QXmlStreamReader;

class ArrayModelXmlReader : public ModelReader
{
    Q_OBJECT

public:
    explicit ArrayModelXmlReader(ArrayModel *arrayModel);

    bool read(QIODevice &in) override;

private:
    void readItems(ArrayModel *model, QXmlStreamReader &stream);
};

#endif // ARRAYMODELXMLREADER_H
