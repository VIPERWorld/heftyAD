#include "ArrayModel.h"
#include "ArrayModelItem.h"
#include "ArrayModelXmlReader.h"

#include "ModelReaderXmlHelper.h"

#include <QXmlStreamReader>

ArrayModelXmlReader::ArrayModelXmlReader(ArrayModel *arrayModel)
    : ModelReader(arrayModel)
{
}

bool ArrayModelXmlReader::read(QIODevice &in)
{
    auto *array = static_cast<ArrayModel*>(model());
    if(!array) {
        return false;
    }

    QXmlStreamReader stream(&in);

    stream.readNextStartElement();
    {
        stream.readNextStartElement(); // reads "array"
        {
            readItems(array, stream);
        }
    }

    return true;
}

void ArrayModelXmlReader::readItems(ArrayModel *model, QXmlStreamReader &stream)
{
    while(stream.readNextStartElement()) {
        if(stream.name().compare("item") == 0) {
            ArrayModelItem &item(model->addNewItem());
            ModelReaderXmlHelper::readArrayModelItemFrom(stream, item);

            stream.skipCurrentElement();
        }
    }
}
