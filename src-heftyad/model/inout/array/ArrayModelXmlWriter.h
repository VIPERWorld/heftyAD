#ifndef ARRAYMODELXMLWRITER_H
#define ARRAYMODELXMLWRITER_H

#include "ModelWriter.h"

class ArrayModel;

class QXmlStreamWriter;

class ArrayModelXmlWriter : public ModelWriter
{
    Q_OBJECT

public:
    explicit ArrayModelXmlWriter(ArrayModel *arrayModel);

    bool write(QIODevice &out) override;

private:
    void writeItems(ArrayModel *model, QXmlStreamWriter &stream);
};

#endif // ARRAYMODELXMLWRITER_H
