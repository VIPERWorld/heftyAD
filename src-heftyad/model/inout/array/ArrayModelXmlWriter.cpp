#include "ArrayModel.h"
#include "ArrayModelItem.h"
#include "ArrayModelXmlWriter.h"

#include "ModelWriterXmlHelper.h"

#include <QXmlStreamWriter>

ArrayModelXmlWriter::ArrayModelXmlWriter(ArrayModel *arrayModel)
    : ModelWriter(arrayModel)
{
}

bool ArrayModelXmlWriter::write(QIODevice &out)
{
    auto *array = static_cast<ArrayModel*>(model());
    if(!array) {
        return false;
    }

    QXmlStreamWriter stream(&out);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("array");
    {
        writeItems(array, stream);
    }
    stream.writeEndElement();

    stream.writeEndDocument();

    return true;
}

void ArrayModelXmlWriter::writeItems(ArrayModel *model, QXmlStreamWriter &stream)
{
    stream.writeStartElement("items");
    {
        stream.writeAttribute("count", QString::number(model->size())); // not used when reading items back
        for(ArrayModelItem *item : model->items()) {
            stream.writeStartElement("item");
            {
                ModelWriterXmlHelper::writeArrayModelItemTo(stream, *item);
            }
            stream.writeEndElement();
        }
    }
    stream.writeEndElement();
}
